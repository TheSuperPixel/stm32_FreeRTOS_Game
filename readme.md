# stm32-FreeRTOS游戏机
## 简介
这是一个支持运行FreeRTOS操作系统的stm32游戏机，系统自带4个游戏程序，可以通过菜单进行选择，除了普通的游戏功能之外，系统还支持后台运行游戏以及多线程功能。游戏机板载stlink，可以直接通过USB电线连接电脑进行在线烧录程序和调试，自带锂电池管理电路，支持使用锂电池充电和供电。

## 演示视频
B站链接：https://www.bilibili.com/video/BV1Vy4UeHEuF/

## 目前存在的主要问题
目前游戏机还有一些小bug，在进入FreeRTOS管理页面时，系统容易崩溃卡死，另外锂电池管理电路稳定性不佳，电路运行时间久后板子的3.3V电源有可能会出现问题，导致stlink无法连接电脑

## 项目主要参考资料
1.stm32硬件设计 https://oshwhub.com/gbn2333/stm32f103-KFB
2.锂电池管理电路设计 https://oshwhub.com/hbozyq/dan-li-zi-dong-sheng-xiang-ya-zhuai-3.3v-dai-chong-dian-guan-li-he-li-dian-chi-bao-hu
3.被移植游戏程序来源 https://space.bilibili.com/189153447
4.STM32F103C8T6最小单片机的freertos移植实验模板 作者：ZSF