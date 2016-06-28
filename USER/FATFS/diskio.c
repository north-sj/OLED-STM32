/*-----------------------------------------------------------------------*/
/* Glue functions for FatFs - MCI/NAND drivers            (C)ChaN, 2010  */
/*-----------------------------------------------------------------------*/

#include "diskio.h"


DSTATUS disk_initialize (BYTE drv)//输入的是哪个卡，0为第一个卡，返回的是卡的状态 初始化成功，无卡，初始化失败
{
u8 state;

    if(drv)
    {
        return STA_NOINIT;  //仅支持磁盘0的操作  0x01
    }

    state = SD_Init();////////////////////////////////////////////////////
    if(state == STA_NODISK)
    {
        return STA_NODISK; //0x02
    }
    else if(state != 0)
    {
        return STA_NOINIT;  //其他错误：初始化失败
    }
    else
    {
        return 0;           //初始化成功
    }
}



DSTATUS disk_status (BYTE drv)//输入的是哪个卡，0为第一个卡，其他暂不支持；返回的是卡的状态 初始化成功，无卡，初始化失败
{
    if(drv)
    {
        return STA_NOINIT;  //仅支持磁盘0操作
    }

    //检查SD卡是否插入
    if(SD_Insert_Status())
    {
        return STA_NODISK;
    }
    return 0;
}



DRESULT disk_read (BYTE drv,BYTE* buff,DWORD sector,BYTE count)//哪个卡（一般为0），读出的数据存储的地方，块地址，块个数
{
	u8 res=0;
    if (drv || !count)
    {    
        return RES_PARERR;  //仅支持单磁盘操作，count不能等于0，否则返回参数错误
    }
    if(SD_Insert_Status())
    {
        return RES_NOTRDY;  //没有检测到SD卡，报NOT READY错误
    }

    
	
    if(count==1)            //1个sector的读操作      
    {                                                
        res = SD_ReadSingleBlock(sector, buff);      
    }                                                
    else                    //多个sector的读操作     
    {                                                
        res = SD_ReadMultiBlock(sector, buff, count);
    }                                                
	/*
    do                           
    {                                          
        if(SD_ReadSingleBlock(sector, buff)!=0)
        {                                      
            res = 1;                           
            break;                             
        }                                      
        buff+=512;                             
    }while(--count);                                         
    */
    //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
    if(res == 0x00)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR;
    }
}


#if _READONLY == 0 //是否为只读模式

DRESULT disk_write (BYTE drv,const BYTE* buff,DWORD sector,BYTE count)
{
u8 res;

    if (drv || !count)
    {    
        return RES_PARERR;  //仅支持单磁盘操作，count不能等于0，否则返回参数错误
    }
  	if(SD_Insert_Status())
    {
        return RES_NOTRDY;  //没有检测到SD卡，报NOT READY错误
    }

    // 读写操作
    if(count == 1)
    {
        res = SD_WriteSingleBlock(sector, buff);
    }
    else
    {
        res = SD_WriteMultiBlock(sector, buff, count);
    }
    // 返回值转换
    if(res == 0)
    {
        return RES_OK;
    }
    else
    {
        return RES_ERROR;
    }
}

#endif /* _READONLY */

DRESULT disk_ioctl (BYTE drv,BYTE ctrl,void* buff)//哪个盘，控制命令（0刷新缓存，3获取块大小，1获取磁盘容量），块大小和磁盘容量存放的指针
{
    DRESULT res;
    if (drv)
    {    
        return RES_PARERR;  //仅支持单磁盘操作，否则返回参数错误
    }
    
    //FATFS目前版本仅需处理CTRL_SYNC，GET_SECTOR_COUNT，GET_BLOCK_SIZ三个命令
    switch(ctrl)
    {
    case CTRL_SYNC:	//刷新磁盘缓存
   
		SD_CS_ENABLE();
        if(SD_WaitReady()==0)
        {
            res = RES_OK;
        }
        else
        {
            res = RES_ERROR;
        }
       
		SD_CS_DISABLE();
        break;
        
    case GET_BLOCK_SIZE://获取块的大小
        *(WORD*)buff = 512;
        res = RES_OK;
        break;

    case GET_SECTOR_COUNT: //获取磁盘的容量
        *(DWORD*)buff = SD_GetCapacity();
        res = RES_OK;
        break;
    default:
        res = RES_PARERR;
        break;
    }

    return res;
}

DWORD get_fattime (void)
{ /*
    struct tm t;
    DWORD date;
    t = Time_GetCalendarTime();
    t.tm_year -= 1980;		//年份改为1980年起
    t.tm_mon++;         	//0-11月改为1-12月
    t.tm_sec /= 2;      	//将秒数改为0-29
    
    date = 0;
    date = (t.tm_year << 25)|(t.tm_mon<<21)|(t.tm_mday<<16)|\
            (t.tm_hour<<11)|(t.tm_min<<5)|(t.tm_sec);

    return date;*/
	return 0;
}

//得到文件Calendar格式的建立日期,是DWORD get_fattime (void) 逆变换							
//struct tm
void get_Calendarfattime (DWORD date,DWORD time)
{  	/*
    struct tm t;
	DWORD time_t;

	t.tm_year=(date>>9)&0x007F;  //bit 9-15
	t.tm_mon=(date>>5)&0x000F;   //bit 5-8
	t.tm_mday=date&0x001F;       //bit 0-4

	t.tm_hour=(time>>11)&0x001F; //bit 11-15
	t.tm_min=(time>>5)&0x003F;   //bit  5-10
	t.tm_sec=(time&0x001F)<<1;   //bit  0-4

	t.tm_year+=1980;		     //年份改为1980年起，文件FAT记录格式
	t.tm_mon--;					 		//FAT记录格式1-12,Calendar对应0-11

	//下面两个函数主要目的是要得到日期对应的星期数，比较巧妙。
    time_t=Time_ConvCalendarToUnix(t);
	t=Time_ConvUnixToCalendar(time_t);

    return t; */

}


