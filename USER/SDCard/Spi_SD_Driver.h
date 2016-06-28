/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : Spi_SD_Driver.h
* Author             : ~风中的叶~
* Version            : V3.0.1
* Date               : 03/12/2012
* Description        : Header for Spi_SD_Driver.c file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SD_SPI_FLASH_H
#define __SD_SPI_FLASH_H

/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x_lib.h"
#include "config.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run the example */
///typedef struct SD_STRUCT
///{	
///	u8 card_insert;				/*卡是否插入；插入为1，否者为0*/
///	u8 card_type;               /* 卡型 */	
///	u32 block_num;				/* 卡中块的数量 */
///	u32 block_len;				/* 卡的块长度(单位:字节)，未用 */
///	u32 erase_unit;				/* 一次可擦除的块个数，未用 */
///	u8 errer;				    /* SD卡的错误信息 */
///}sd_struct;
///extern sd_struct sd;
//#define NO_CARD      99//FatFS错误标志：没有插入磁盘
/* Private define ------------------------------------------------------------*/
/* SD卡类型定义 */
#define SD_TYPE_MMC     0
#define SD_TYPE_V1      1
#define SD_TYPE_V2      2
#define SD_TYPE_V2HC    4

/* SPI总线速度设置*/
#define SPI_SPEED_LOW   0
#define SPI_SPEED_HIGH  1

/* SD传输数据结束后是否释放总线宏定义 */
#define NO_RELEASE      0
#define RELEASE         1

/* SD卡指令表 */
#define CMD0    0       //卡复位
#define CMD9    9       //命令9 ，读CSD数据
#define CMD10   10      //命令10，读CID数据
#define CMD12   12      //命令12，停止数据传输
#define CMD16   16      //命令16，设置SectorSize 应返回0x00
#define CMD17   17      //命令17，读sector
#define CMD18   18      //命令18，读Multi sector
#define ACMD23  23      //命令23，设置多sector写入前预先擦除N个block
#define CMD24   24      //命令24，写sector
#define CMD25   25      //命令25，写Multi sector
#define ACMD41  41      //命令41，应返回0x00
#define CMD55   55      //命令55，应返回0x01
#define CMD58   58      //命令58，读OCR信息
#define CMD59   59      //命令59，使能/禁止CRC，应返回0x00

#define SD_Insert_Status()    GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2)//卡插入检测  

/* Exported macro ------------------------------------------------------------*/
/* Select SPI FLASH: Chip Select pin low  */
#define SD_CS_ENABLE()       GPIO_ResetBits(GPIOA, GPIO_Pin_3)
/* Deselect SPI FLASH: Chip Select pin high */
#define SD_CS_DISABLE()      GPIO_SetBits(GPIOA, GPIO_Pin_3)

/* Exported functions ------------------------------------------------------- */
/*----- High layer function -----*/
/* Private function prototypes -----------------------------------------------*/
void SDCard_SpiInit(void);
void SPI_SetSpeed(u8 SpeedSet);

u8 SPI_ReadWriteByte(u8 TxData);                //SPI总线读写一个字节
u8 SD_WaitReady(void);                          //等待SD卡就绪
u8 SD_SendCommand(u8 cmd, u32 arg, u8 crc);     //SD卡发送一个命令
u8 SD_SendCommand_NoDeassert(u8 cmd, u32 arg, u8 crc);
u8 SD_Init(void);                               //SD卡初始化
                                                //
u8 SD_ReceiveData(u8 *data, u16 len, u8 release);//SD卡读数据
u8 SD_GetCID(u8 *cid_data);                     //读SD卡CID
u8 SD_GetCSD(u8 *csd_data);                     //读SD卡CSD
u32 SD_GetCapacity(void);                       //取SD卡容量

u8 SD_ReadSingleBlock(u32 sector, u8 *buffer);  //读一个sector
u8 SD_WriteSingleBlock(u32 sector,const u8 *buffer); //写一个sector
u8 SD_ReadMultiBlock(u32 sector,u8 *buffer,u8 count); //读多个sector
u8 SD_WriteMultiBlock(u32 sector,const u8 *data,u8 count);  //写多个sector

#endif /* __SD_SPI_FLASH_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
