#include "SPI1.H"
#include "stm32f10x_spi.h"
void SPI1GPIOA_config()//��SPI1��IO��������
{
 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);					//ʹ��GPIOAʱ��
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//MOSI��MISO��SCK����
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;								//�����������
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;							//50MHZ
 GPIO_Init(GPIOA,&GPIO_InitStructure);												//Ӧ����PA��
 /*
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;				//SPI_CS��PA2��Ϊ�ӻ�Ƭѡ����
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	//�������
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
 GPIO_Init(GPIOA,&GPIO_InitStructure);
 */
 }
 void SPI1_config()//����SPI1����ؼĴ���
 {
 SPI_InitTypeDef SPI_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);								//ʹ��SPI1ʱ��
 SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;		//����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
 SPI_InitStructure.SPI_Mode=SPI_Mode_Master;												//����SPI����ģʽ:����Ϊ��SPI
 SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;										//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
 SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low; 													//ѡ���˴���ʱ�ӵ���̬:ʱ�����յ�
 SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;													//���ݲ����ڵ�һ��ʱ����
 SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;														//SPI_NSS_Soft;//NSS�ź���Ӳ����NSS�ܽţ�����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
 SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_256; //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256,fpclk/256
 SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB; 									//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
 //SPI_InitStructure.SPI_CRCPolynomial=7;														//CRCֵ����Ķ���ʽ
 SPI_Init(SPI1,&SPI_InitStructure);																	//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 SPI_Cmd(SPI1,ENABLE);																							//ʹ��SPI����  
}



