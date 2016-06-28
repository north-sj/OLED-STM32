#ifndef _SPI1_H_
#define _SPI1_H_
#include"config.h"
void SPI1GPIOA_config(void);
void SPI1_config(void);
#endif
   /*
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);//查询标志位是否发送完毕
   SPI_I2S_SendData(SPI1, data);//发送数据
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);//查询标志是否接收完毕
   SPI_I2S_ReceiveData(SPI1);//接收数据
   */

