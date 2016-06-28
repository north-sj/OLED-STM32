
/* 防止重定义 ----------------------------------------------------------------*/
#ifndef _CONFIG_H
#define _CONFIG_H

/* Includes ------------------------------------------------------------------*/
/* 当不需要使用以下功能时，注释掉相关头文件，删除相应的C源文件即可------------*/
/*	*/
#include "stm32f10x.h"//中包含#include "stm32f10x_conf.h"
#include "USART1.H"
#include "Spi_SD_Driver.h"
#include "integer.h"
#include "diskio.h"
#include "ff.h"
#include "TEST_FATFS.h"

#endif /* _CONFIG_H */
/***********************************文件结束***********************************/
