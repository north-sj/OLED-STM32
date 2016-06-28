#ifndef _USART_H_
#define _USART_H_
#include"config.h"
#include<stdio.h>
void UART1GPIO_config(void);//串口IO口配置
void USART1_config(void);		//串口初始化
void UART1NVIC_config(void);//配置中断
//int fputc(int ch, FILE *f);	//发送数据
//int fgetc(FILE *f); 				//接收数据
int USART_Scanf_Name(char * str);
#endif
