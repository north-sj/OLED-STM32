#include "SPI1.H"
#include "stm32f10x_spi.h"
void SPI1GPIOA_config()//对SPI1的IO进行配置
{
 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);					//使能GPIOA时钟
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//MOSI，MISO，SCK复用
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;								//复用推挽输出
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;							//50MHZ
 GPIO_Init(GPIOA,&GPIO_InitStructure);												//应用于PA口
 /*
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;				//SPI_CS，PA2作为从机片选控制
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	//推挽输出
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
 GPIO_Init(GPIOA,&GPIO_InitStructure);
 */
 }
 void SPI1_config()//配置SPI1的相关寄存器
 {
 SPI_InitTypeDef SPI_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);								//使能SPI1时钟
 SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;		//设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
 SPI_InitStructure.SPI_Mode=SPI_Mode_Master;												//设置SPI工作模式:设置为主SPI
 SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;										//设置SPI的数据大小:SPI发送接收8位帧结构
 SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low; 													//选择了串行时钟的稳态:时钟悬空低
 SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;													//数据捕获于第一个时钟沿
 SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;														//SPI_NSS_Soft;//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
 SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256; //定义波特率预分频的值:波特率预分频值为256,fpclk/256
 SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB; 									//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
 //SPI_InitStructure.SPI_CRCPolynomial=7;														//CRC值计算的多项式
 SPI_Init(SPI1,&SPI_InitStructure);																	//根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 SPI_Cmd(SPI1,ENABLE);																							//使能SPI外设  
}




