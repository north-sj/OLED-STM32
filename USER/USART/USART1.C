#include"USART1.H"
//TX=PA9
//RX=PA10
void UART1GPIO_config()//串口IO口配置
{
GPIO_InitTypeDef GPIO_InitStructure;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);//打开GPIO时钟，复用功能

//GPIO_StructInit(&GPIO_InitStructure);					//用默认值初始化结构体为所有端口2M浮空输入
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				//PA9作为US1的TX端，负责发送数据
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出模式
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//2M
GPIO_Init(GPIOA,&GPIO_InitStructure);						//用以上值初始化PA9口

GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			//PA10作为US1的RX端，负责接收数据
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入模式
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//2M
GPIO_Init(GPIOA, &GPIO_InitStructure);					//用以上值初始化PA10口
}

void USART1_config()//串口初始化
{
USART_InitTypeDef USART_InitStructure;
//RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);		//打开串口1的时钟
USART_StructInit(&USART_InitStructure);											//将结构体设置为缺省状态，9600bps，8数据位，1停止位，不校验，硬件流控制失能
USART_InitStructure.USART_BaudRate =115200;									//波特率设置为115200
USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//一帧数据的宽度设置为8bits
USART_InitStructure.USART_StopBits = USART_StopBits_1;			//在帧结尾传输1个停止位
USART_InitStructure.USART_Parity = USART_Parity_No;					//奇偶失能模式，无奇偶校验
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//发送/接收使能
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制失能
USART_Init(USART1, &USART_InitStructure);										//设置串口1
//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);						//打开串口1的接收中断响应函数
USART_Cmd(USART1, ENABLE);//打开串口1
}

void UART1NVIC_config()//配置中断
{
NVIC_InitTypeDef NVIC_InitStructure;
//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//选择中断分组1
NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;						//选择串口1中断
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//抢占式中断优先级设置为0
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;					//响应式中断优先级设置为3
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//使能中断
NVIC_Init(&NVIC_InitStructure);
}

/* 
void USART1_IRQHandler(void)//中断方式的USART接收
{
if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  		//判断发生接收中断
{//USART_ClearITPendingBit(USART1,  USART_IT_RXNE);    			//清除中断标志,读接收到的数据时自动清除
USART_SendData(USART1, USART_ReceiveData(USART1));					//将数据回送至电脑
while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	//等待数据发送完毕
} 
}*/

/*
if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)  //判断接收标志；查询方式的USART接收
{
USART_ReceiveData(USART1);
}
USART_SendData(USART1, i);//将数据回送至电脑
while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);	//等待数据发送完毕
*/

 int fputc(int ch, FILE *f)//发送数据
   {
      USART_SendData(USART1, (unsigned char) ch);// USART1 可以换成 USART2 等
      while (!(USART1->SR & USART_FLAG_TXE));
      return (ch);
   }
 


 int fgetc(FILE *f) // 接收数据
  { 
      while (!(USART1->SR & USART_FLAG_RXNE));
      return ((int)(USART1->DR & 0x1FF));
   } 
//如果要使用printf函数除了#include<stdio.h>和重定义两个输入输出函数外还应该Target->Code Generation中勾选Use MicroLIB 
//char ch='E',str[10]={"FGHIJKLM"};
//int intd=12345;
//printf("The is a example!\n");
//printf("%c\n%s\n%d\n%c\n%s\n%d\n%d\n",'A',"BCD",0x23,ch,str,ch,intd);
//The is a example!
//A
//BCD
//35
//E
//FGHIJKLM
//69
//12345
/*******************************************************************************
* Function Name  : USART_Scanf_Name
* Description    : Gets Char values from the hyperterminal.
* Input          : None
* Output         : None
* Return         : Length
*******************************************************************************/
int USART_Scanf_Name(char * str)
{	
    u16 index = 0;
	USART_ClearFlag(USART1,USART_FLAG_RXNE);//清一下接收标志位
    while(1) 
    {
        /* Loop until RXNE = 1 */
        while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET){}//等待直到有数据的到来
        
		str[index++] = (USART_ReceiveData(USART1));
	    printf("%c",str[index - 1]);				//返回输入的内容
	    if(str[index - 1] == 0x0d)					//回车键\r
	    {  
	        index--;												//去掉回车符并以\0结尾
			str[index] ='\0';										//结束符号
	        return index;
        }
    }													
}
