﻿#include "UsbTmc.h"
#include "UsbTmcProtocol.h"

extern "C"
{
#pragma warning(push)
#pragma warning(disable : 4200)
#include <libusb/libusb.h>
#pragma warning(pop)
}

#include <array>

namespace OpenVisa
{
static std::string getStringDescriptor(libusb_device_handle* handle, uint8_t descIndex)
{
    uint8_t data[128];
    int r = libusb_get_string_descriptor(handle, descIndex, 0x0409, data, 128);
    if (r < 4)
    {
        if (libusb_get_string_descriptor_ascii(handle, descIndex, data, 128) > 0)
            return std::string(reinterpret_cast<char*>(data));
    }

    if (data[0] > r || data[1] != LIBUSB_DT_STRING)
        return {};

    int length = (r - 2) / 2;
    std::string descriptor;
    descriptor.resize(length);
    for (int i = 0; i < length; i++)
        descriptor[i] = data[i * 2 + 2];
    return descriptor;
}

struct UsbTmc::Impl
{
    libusb_context* context { nullptr };
    libusb_device_handle* handle { nullptr };
    uint8_t endpoint_in;
    uint8_t endpoint_out;
    uint8_t endpoint_interrupt_in { 0 };
    bool avalibe { false };

    uint8_t tag { 0 };
    int outMaxPacketSize { 0 };
    int inMaxPacketSize { 0 };
    std::chrono::milliseconds timeout;
};

UsbTmc::UsbTmc(Object::Attribute const& attr) : IOBase(attr), m_impl(std::make_unique<Impl>()) { init(); }

UsbTmc::~UsbTmc() { libusb_exit(m_impl->context); }

void UsbTmc::connect(const Address<AddressType::USB>& addr,
                     const std::chrono::milliseconds& openTimeout,
                     const std::chrono::milliseconds& commandTimeout)
{
    m_impl->timeout = commandTimeout;

    libusb_device** devs;

    auto count = libusb_get_device_list(m_impl->context, &devs);
    if (count < 0)
        throw std::exception(libusb_strerror(static_cast<enum libusb_error>(count)));

    auto devsScope = std::shared_ptr<int>(new int,
                                          [&](int* p)
                                          {
                                              delete p;
                                              libusb_free_device_list(devs, 1);
                                          });

    libusb_device* device = NULL;
    libusb_device_descriptor deviceDescription;
    for (int i = 0; i < count; ++i)
    {
        if (libusb_get_device_descriptor(devs[i], &deviceDescription) != LIBUSB_SUCCESS)
            continue;
        if (deviceDescription.idVendor == addr.vendorId() && deviceDescription.idProduct == addr.productId())
        {
            libusb_device_handle* h;
            if (libusb_open(devs[i], &h) != LIBUSB_SUCCESS)
                continue;
            auto sn = getStringDescriptor(h, deviceDescription.iSerialNumber);
            libusb_close(h);
            if (sn != addr.serialNumber())
                continue;
            device = devs[i];
            break;
        }
    }

    if (!device)
        throw std::exception("USB device not found.");

    if (auto code = libusb_open(device, &m_impl->handle); code != LIBUSB_SUCCESS)
        throw std::exception(libusb_strerror(code));

    libusb_config_descriptor* conf_desc;
    if (auto code = libusb_get_config_descriptor(device, 0, &conf_desc); code != LIBUSB_SUCCESS) // uint8_t config_index = 0;
        throw std::exception(libusb_strerror(code));

    auto confDescScope = std::shared_ptr<int>(new int,
                                              [&](int* p)
                                              {
                                                  delete p;
                                                  libusb_free_config_descriptor(conf_desc);
                                              });

    std::vector<uint8_t> ifs;
    for (int i = 0; i < conf_desc->bNumInterfaces; i++)
    {
        for (int j = 0; j < conf_desc->interface[i].num_altsetting; j++)
        {
            if (conf_desc->interface[i].altsetting[j].bInterfaceClass != 0xFE &&
                conf_desc->interface[i].altsetting[j].bInterfaceSubClass != 0x03 &&
                conf_desc->interface[i].altsetting[j].bInterfaceProtocol != 0x01) // USBTMC
                continue;
            ifs.push_back(i);
            for (int k = 0; k < conf_desc->interface[i].altsetting[i].bNumEndpoints; k++)
            {
                auto endpoint = &conf_desc->interface[i].altsetting[j].endpoint[k];
                if ((endpoint->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK) & (LIBUSB_TRANSFER_TYPE_BULK))
                { //只获取批量传输端点
                    if (endpoint->bEndpointAddress & LIBUSB_ENDPOINT_IN)
                    {
                        if (!m_impl->endpoint_in)
                        {
                            m_impl->endpoint_in     = endpoint->bEndpointAddress;
                            m_impl->inMaxPacketSize = endpoint->wMaxPacketSize;
                            libusb_clear_halt(m_impl->handle, m_impl->endpoint_in); //清除暂停标志
                        }
                    }
                    else
                    {
                        if (!m_impl->endpoint_out)
                        {
                            m_impl->endpoint_out     = endpoint->bEndpointAddress;
                            m_impl->outMaxPacketSize = endpoint->wMaxPacketSize;
                            libusb_clear_halt(m_impl->handle, m_impl->endpoint_out);
                        }
                    }
                }
                else if ((endpoint->bmAttributes & LIBUSB_TRANSFER_TYPE_MASK) & (LIBUSB_TRANSFER_TYPE_INTERRUPT))
                {
                    if (endpoint->bEndpointAddress & LIBUSB_ENDPOINT_IN)
                    {
                        m_impl->endpoint_interrupt_in = endpoint->bEndpointAddress;
                        libusb_clear_halt(m_impl->handle, m_impl->endpoint_interrupt_in);
                    }
                }
            }
        }
    }

    //获得通讯端点并声明内核接口
    {
        libusb_set_auto_detach_kernel_driver(m_impl->handle, 1);
        for (auto i : ifs)
        {
            auto r = libusb_claim_interface(m_impl->handle, i);
            if (r != LIBUSB_SUCCESS)
            {
                throw std::exception("open usb device failed.");
            }
        }
    }
}

void UsbTmc::send(const std::string& buffer) const
{
    BulkOut bo(m_impl->tag, USBTMC_MSGID_DEV_DEP_MSG_OUT, m_impl->outMaxPacketSize);
    bo.append(buffer);

    auto&& msgs = static_cast<std::vector<std::string>>(bo);

    for (auto&& msg : msgs)
    {
        if (auto code = libusb_bulk_transfer(m_impl->handle,
                                             m_impl->endpoint_out,
                                             reinterpret_cast<unsigned char*>(msg.data()),
                                             static_cast<int>(msg.size()),
                                             nullptr,
                                             static_cast<unsigned int>(m_impl->timeout.count()));
            code != LIBUSB_SUCCESS)
        {
            throw std::exception(libusb_strerror(static_cast<enum libusb_error>(code)));
        }
    }
}

std::string UsbTmc::readAll() const { return read(0xffffffff); }

std::string UsbTmc::read(size_t size) const
{
    {
        std::string buffer = BulkRequest(m_impl->tag, static_cast<unsigned int>(size));
        if (auto code = libusb_bulk_transfer(m_impl->handle,
                                             m_impl->endpoint_out,
                                             reinterpret_cast<unsigned char*>(buffer.data()),
                                             static_cast<int>(buffer.size()),
                                             nullptr,
                                             static_cast<unsigned int>(m_impl->timeout.count()));
            code != LIBUSB_SUCCESS)
        {
            throw std::exception(libusb_strerror(static_cast<enum libusb_error>(code)));
        }
    }

    std::string packs;
    BulkIn in;
    int transfered;
    do
    {
        std::string pack;
        pack.resize(m_impl->inMaxPacketSize);
        if (auto code = libusb_bulk_transfer(m_impl->handle,
                                             m_impl->endpoint_in,
                                             reinterpret_cast<unsigned char*>(pack.data()),
                                             static_cast<int>(pack.size()),
                                             &transfered,
                                             static_cast<unsigned int>(m_impl->timeout.count()));
            code != LIBUSB_SUCCESS)
        {
            throw std::exception(libusb_strerror(static_cast<enum libusb_error>(code)));
        }
        packs.append(pack.c_str(), transfered);
    } while (!in.parse(packs, size));
    m_impl->avalibe = !in.eom();
    return std::move(in);
}

void UsbTmc::close() noexcept
{
    if (m_impl->handle)
    {
        libusb_close(m_impl->handle);
        m_impl->handle = nullptr;
    }
}

bool UsbTmc::connected() const noexcept { return m_impl->handle; }

size_t UsbTmc::avalible() const noexcept { return 0; }

void UsbTmc::init() { libusb_init(&m_impl->context); }

} // namespace OpenVisa