#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
// 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// ALIENTEK精英STM32开发板V3
// 串口3初始化代码
// 正点原子@ALIENTEK
// 技术论坛:www.openedv.com
// 修改日期:2015/3/14
// 版本：V1.0
// 版权所有，盗版必究。
// Copyright(C) 广州市星翼电子科技有限公司 2014-2024
// All rights reserved
//********************************************************************************
// 修改说明
// 无
//////////////////////////////////////////////////////////////////////////////////

#define USART2_MAX_RECV_LEN 600 // 最大接收缓存字节数
#define USART2_MAX_SEND_LEN 600 // 最大发送缓存字节数
#define USART2_RX_EN        1   // 0,不接收;1,接收.

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; // 接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u16 USART2_RX_STA;                      // 接收数据状态

void          usart2_init(u32 bound);
void          u2_printf(char *fmt, ...);
unsigned char UART1GetByte(unsigned char *GetData);
void          UART1Test(void);
#endif
