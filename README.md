# OpenVisa

[TOC]

## 简介

测试测量仪器通信库，包含TCP，Serial Port，USB，VXI11等通信协议。

## 一、编写本项目的目的

1. 常用的NI-VISA是一个商业软件，并没有多少开源免费软件可以替代；
2. 我只是想要简简单单地和设备通信而已，受够了每次安装我们的软件，都要携带几个G的通信库安装包。

## 二、开发计划
- [x] TCP socket Ascii通信
- [x] 串口 Ascii通信
- [x] USBTMC
- [x] VXI-11
- [ ] GPIB
- [ ] HiSLIP

由于个人精力原因，上述开发仅实现了基本通信功能，并未实现全功能VISA，如VISA的Event等功能，虽然未来计划是跨平台的，但目前部分功能仅实现了Windows端。

## 三、编译

本项目基于C++20标准编写，在VS2022下编译通过。

### 1. 第三方库

* BOOST库
* LIBUSB

### 2. 环境变量设置

* 添加 ```BOOST_ROOT``` 变量，并设置值为 BOOST库根目录，比如：C:\local\boost_1_78_0
* 添加 ```BOOST_LIBRARYDIR``` 变量，并设置值为 BOOST lib库所在目录，比如：%BOOST_ROOT%\lib64-msvc-14.3
* 添加 ```BOOST_INCLUDEDIR``` 变量，并设置值为 BOOST 头文件目录，比如 %BOOST_ROOT%

### 3. CMake设置

* 设置 ```LIBUSB_INCLUDEDIR``` 为 libusb.h 头文件所在目录的上一级，比如 libusb.h 的所在目录为：F:\3rd\libusb\libusb\libusb.h，则将 ```LIBUSB_INCLUDEDIR``` 设为 ```F:/3rd/libusb```

* 设置 ```LIBUSB_LIBRARY``` 为 libusb-1.0.lib 文件所在的路径，如：```F:/3rd/libusb/x64/Release/lib/libusb-1.0.lib```

如果编译时没有找到 libusb.h，请检查```LIBUSB_INCLUDEDIR```的设置是否正确，如果编译动态库时出现链接错误，请检查```LIBUSB_LIBRARY``` 
