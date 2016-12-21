#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//
//系统中断分组设置化		   
//修改日期:2012/9/10
//版本：V1.4
//********************************************************************************  
void NVIC_Configuration(void)
{
    
  NVIC_InitTypeDef NVIC_InitStructure;    //配置中断优先级
      
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    
      
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;    
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;    
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;    
  NVIC_Init(&NVIC_InitStructure);    

  //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级

}
