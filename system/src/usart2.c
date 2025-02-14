#include "usart2.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
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

// 串口发送缓存区
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; // 发送缓冲,最大USART2_MAX_SEND_LEN字节
// 串口接收缓存区
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; // 接收缓冲,最大USART2_MAX_RECV_LEN个字节.

// 在配置模式下： timer=10ms
// 通过判断接收连续2个字符之间的时间差不大于timer来决定是不是一次连续的数据.
// 如果2个字符接收间隔超过timer,则认为不是1次连续数据.也就是超过timer没有接收到
// 任何数据,则表示此次接收完毕.
// 接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
u16 USART2_RX_STA = 0;
// 初始化IO 串口3
// pclk1:PCLK1时钟频率(Mhz)
// bound:波特率
void usart2_init(u32 bound)
{
  NVIC_InitTypeDef  NVIC_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // 串口2时钟使能

  // USART2_TX   PA2
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2; // PA2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; // 复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);           // 初始化PA2

  // USART2_RX	  PA3
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);                // 初始化PA3

  // 设置中断优先级
  NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;      // 抢占优先级3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;      // 子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE; // IRQ通道使能
  NVIC_Init(&NVIC_InitStructure);                                // 根据指定的参数初始化VIC寄存器

  // USART2 初始化设置
  USART_InitStructure.USART_BaudRate            = bound;                          // 波特率设置
  USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            // 字长为8位数据格式
  USART_InitStructure.USART_StopBits            = USART_StopBits_1;               // 一个停止位
  USART_InitStructure.USART_Parity              = USART_Parity_No;                // 无奇偶校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件数据流控制
  USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;  // 收发模式

  USART_Init(USART2, &USART_InitStructure);      // 初始化串口	2
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 开启中断
  USART_Cmd(USART2, ENABLE);                     // 使能串口
}

//        //t1
//
//        /*接收一个字节数据*/
// unsigned char UART2GetByte(unsigned char* GetData)
//{
//        if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
//        {  return 0;//没有收到数据
//		}
//        *GetData = USART_ReceiveData(USART2);
//        return 1;//收到数据
//}
///*接收一个数据，马上返回接收到的这个数据*/
// void UART2Test(void)
//{
//        unsigned char i = 0;

//       while(1)
//       {
//		 while(UART2GetByte(&i))
//        {
//         USART_SendData(USART2,i);
//        }
//       }
//}
//        //t1

// t1

///*接收一个字节数据*/
// unsigned char UART1GetByte(unsigned char* GetData)
//{
//         if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
//         {  return 0;//没有收到数据
//		}
//         *GetData = USART_ReceiveData(USART2);
//         return 1;//收到数据
// }
///*接收一个数据，马上返回接收到的这个数据*/
// void UART1Test(void)
//{
//        unsigned char i = 0;

//       while(1)
//       {
//		 while(UART1GetByte(&i))
//        {
//         USART_SendData(USART1,i);
//        }
//       }
//}

#ifdef USART2_RX_EN // 如果使能了接收
void USART2_IRQHandler(void)
{
  u8 Res;
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // 接收中断（接收到的数据必须是0x0d  0x0a结尾）
  {
    Res = USART_ReceiveData(USART2);  //(USART1->DR);	//读取接收到的数据
                                      //		UART1Test();
    if((USART2_RX_STA & 0x8000) == 0) // 接收未完成
    {
      if(USART2_RX_STA & 0x4000) // 接收到了0x0d
      {
        if(Res != 0x0a)
          USART2_RX_STA = 0; // 接收错误，重新开始
        else
          USART2_RX_STA |= 0x8000; // 接收完成了
      }
      else // 没收到0X0D
      {
        if(Res == 0x0d)
          USART2_RX_STA |= 0x4000;
        else
        {
          USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = Res;
          USART2_RX_STA++;
          if(USART2_RX_STA > (USART2_MAX_RECV_LEN - 1))
            USART2_RX_STA = 0; // 接收数据错误，重新开始接收
        }
      }
    }
  }
}
#endif

// 串口2,printf 函数
// 确保一次发送数据不超过USART2_MAX_SEND_LEN字节
void u2_printf(char *fmt, ...)
{
  u16     i, j;
  va_list ap;
  va_start(ap, fmt);
  vsprintf((char *)USART2_TX_BUF, fmt, ap);
  va_end(ap);
  i = strlen((const char *)USART2_TX_BUF); // 此次发送数据的长度

  for(j = 0; j < i; j++) // 循环发送数据
  {
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
      ; // 等待上次传输完成

    USART_SendData(USART2, (uint8_t)USART2_TX_BUF[j]); // 发送数据到串口2
    USART_SendData(USART1, (uint8_t)USART2_TX_BUF[j]);
  }
}
