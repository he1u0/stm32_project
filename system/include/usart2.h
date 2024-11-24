#ifndef __USART2_H
#define __USART2_H
#include "stdio.h"
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
// ������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
// ALIENTEK��ӢSTM32������V3
// ����3��ʼ������
// ����ԭ��@ALIENTEK
// ������̳:www.openedv.com
// �޸�����:2015/3/14
// �汾��V1.0
// ��Ȩ���У�����ؾ���
// Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
// All rights reserved
//********************************************************************************
// �޸�˵��
// ��
//////////////////////////////////////////////////////////////////////////////////

#define USART2_MAX_RECV_LEN 600 // �����ջ����ֽ���
#define USART2_MAX_SEND_LEN 600 // ����ͻ����ֽ���
#define USART2_RX_EN        1   // 0,������;1,����.

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; // ���ջ���,���USART3_MAX_RECV_LEN�ֽ�
extern u16 USART2_RX_STA;                      // ��������״̬

void          usart2_init(u32 bound);
void          u2_printf(char *fmt, ...);
unsigned char UART1GetByte(unsigned char *GetData);
void          UART1Test(void);
#endif
