/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h" 
#include "stm32f10x_it.h"  
#include "stm32f10x_gpio.h"  
#include "stm32f10x_tim.h"
#include "oled.h"


int k=1;																									//1输出缓冲区G_Bin1数据，-1输出缓冲区G_Bin2数据
extern unsigned char G_Bin1[1024];
extern unsigned char G_Bin2[1024];
int k_1=1;
int k_2=0;

 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}




void TIM3_IRQHandler(void)  
{  
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  
  //中断执行代码请写下面
	if(k==1)
		{
			//LED_OFF;
			//OLED_Clear();
			OLED_DrawBMP(0,0,128,8,G_Bin1);
			k=k*(-1);
			k_1=0;
			//LED_ON;
			//GPIO_SetBits(GPIOA,GPIO_Pin_0);  
		}
	else
		{
			//LED_OFF;
			//OLED_Clear();
			OLED_DrawBMP(0,0,128,8,G_Bin2);
			k=k*(-1);
			k_2=0;
			//LED_ON;
			//GPIO_ResetBits(GPIOA,GPIO_Pin_0);  
		}
	
#if 1
  if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==Bit_RESET)  
		{  
			GPIO_SetBits(GPIOA,GPIO_Pin_0);  
		}
	else
		{
			GPIO_ResetBits(GPIOA,GPIO_Pin_0);  
		}
#endif		
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
