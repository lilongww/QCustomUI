# OpenVisa

# 简介

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

TODO
