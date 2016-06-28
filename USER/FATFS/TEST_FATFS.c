/*--------------File Info-------------------------------------------------------
** 文   件   名:  FATFS_Function.c
** 作        者:
** 最后修改日期:  2010.01.18
** 版        本:  V1.0
** 描        述:  FATFS常用功能测试函数 .连接PC机串口，观察超级终端输出
**------------------------------------------------------------------------------
** Created   by:  
** Created date:  2009.03.04
*******************************************************************************/ 
#include "TEST_FATFS.h"
#include "string.h"

#ifdef TEST_FATFS_EN



//检测磁盘是否插好
BOOL disk_detect_OK(void)
{
    if( disk_status(0)==STA_NODISK )/* Physical drive nmuber (0..) */
	{
	    printf("\r\n\r\n%48s\r\n","<未检测到磁盘,请检查SD卡是否插好...>");
		return FALSE;
	}
	return TRUE;
}



//测试函数执行结果分析
void die(FRESULT res)
{
    switch(res)
	{
	    case FR_OK:	//The function succeeded. 
		{
		    printf("\r\nThe function succeeded!\r\n");
			break;
		}
		case FR_NOT_READY://The disk drive cannot work due to no medium in the drive or any other reason
		{
		    printf("\r\nThe disk drive cannot work due to no medium in the drive or any other reason!\r\n");
			break;
		}
		case FR_NO_FILE://Could not find the file.
		{
		    printf("\r\nCould not find the file!\r\n");
			break;
		}
		case FR_NO_PATH://Could not find the path
		{
		    printf("\r\nCould not find the path!\r\n");
			break;
		}
		case FR_INVALID_NAME://The path name is invalid
		{
		    printf("\r\nThe path name is invalid!\r\n");
			break;
		}
		case FR_INVALID_DRIVE://The drive number is invalid
		{
		    printf("\r\nThe drive number is invalid!\r\n");
			break;
		}
		case FR_DENIED://The directory cannot be created due to directory table or disk is full. 
		{
		    printf("\r\nThe directory cannot be created due to directory table or disk is full!\r\n");
			break;
		}
		case FR_EXIST://A file or directory that has same name is already existing
		{
		    printf("\r\nA file or directory that has same name is already existing!\r\n");
			break;
		}
//		case FR_RW_ERROR://The function failed due to a disk error or an internal error
/*
		case FR_RW_ERROR://The function failed due to a disk error or an internal error
		{
		    printp("\r\nThe function failed due to a disk error or an internal error!\r\n");
			break;
		}
*/
		case FR_WRITE_PROTECTED://The medium is write protected
		{
		    printf("\r\nThe medium is write protected!\r\n");
			break;
		}
		case FR_NOT_ENABLED://The logical drive has no work area
		{
		    printf("\r\nThe logical drive has no work area!\r\n");
			break;
		}
		case FR_NO_FILESYSTEM://There is no valid FAT partition on the disk
		{
		    printf("\r\nThere is no valid FAT partition on the disk!\r\n");
			break;
		}
		case FR_INVALID_OBJECT://The file object is invalid
		{
		    printf("\r\nThe file object is invalid!\r\n");
			break;
		}
       
	    //The function aborted before start in format due to a reason as follows. 
        //The disk size is too small. 
        //Invalid parameter was given to any parameter. 
        //Not allowable cluster size for this drive. This can occure when number of clusters becomes around 0xFF7 and 0xFFF7. 
		case FR_MKFS_ABORTED://
		{
		    printf("\r\nThe function aborted before start in format!\r\n");
			break;
		}
		
		default:
		{
		    printf("\r\nerror!\r\n");
			break;
		}	
	}
	return;
}
void Test_f_getfree(void)//获取卡的总容量及剩余容量
{
    FATFS fs;
		FATFS *pfs;
    DWORD clust;
		FRESULT res;         // FatFs function common result code

	//检测磁盘是否插好
	if( disk_detect_OK()==FALSE ) return;

	pfs=&fs;//指向
	// Register a work area for logical drive 0
    f_mount(0, &fs);//安装FATFS，就是给FATFS分配空间

    // Get free clusters
    res = f_getfree("/", &clust, &pfs);//必须是根目录，默认磁盘0;"/"或者"0:/"
    if ( res==FR_OK ) 
    {
	    // Get free space
        printf("\r\n%d MB total disk space.\r\n%d MB available on the disk.\r\n",
                (int)(DWORD)(pfs->n_fatent - 2) * pfs->csize /2/1024,//总的磁盘空间M	=（总簇数-2）*每簇的扇区数/2/1024=可用簇数*每簇的扇区数/2/1024
                 (int)clust * pfs->csize /2/1024);//空闲的磁盘空间M=剩余簇数*每簇的扇区数/2/1024
	}
	else die(res);//测试函数执行结果分析
	
	// Unregister a work area before discard it
    f_mount(0, NULL);//卸载FATFS，就是释放FATFS结构体所占空间
}

void Test_f_read(void)//读文件的数据，如果没有此文件则返回错误
{
    FATFS fs;            // Work area (file system object) for logical drive
    FIL fsrc;            // file objects
    BYTE buffer[512];     // file copy buffer
    FRESULT res;         // FatFs function common result code
    UINT br;             // File R count
		u16 i;
	

	char path[20];

	//检测磁盘是否插好
	if( disk_detect_OK()==FALSE ) return;

    // Register a work area for logical drive 0
    f_mount(0, &fs);

	printf("\r\nread file:>");
	USART_Scanf_Name(path);//通过串口输入文件路径名/dir/file.txt或者0:dir/file.txt或者0:/dir/file.txt

	//Open source file
    res = f_open(&fsrc, path, FA_OPEN_EXISTING | FA_READ);//打开存在的文件，如果没有则返回错误
	die(res);
	
	//buffer空间设大一点，会提高读的速度。
	//如果文件实际大小512byte，
	//设为buffer[512]时，只需要循坏一次，如果设为buffer[1]，需要循坏512次。
	//下面两行主要是去除1s误差。

	for (;;) 
	{
		for(i=0;i<sizeof(buffer);i++) buffer[i]='\0';//清除缓存

		res = f_read(&fsrc, buffer, sizeof(buffer), &br);
        if (res ||(br == 0)) break;   // error or eof	
		
		printf("%s",buffer);	
    }

			    
    // Close all files
    f_close(&fsrc);
    // Unregister a work area before discard it
    f_mount(0, NULL);
}

void Test_f_write(void)//写数据到文件，如果没有此文件则创建文件
{
    FATFS fs;            // Work area (file system object) for logical drive
    FRESULT res;         // FatFs function common result code
		FIL Make_file;
    char file_name[20];
    char Storage_buffer[] ="0";
	
	UINT bw;
	//检测磁盘是否插好
	if( disk_detect_OK()==FALSE ) return;
	 printf("\r\n inaert_ok:>");
    // Register a work area for logical drive 0
    f_mount(0, &fs);

	printf("\r\n Make file Name:>");
	USART_Scanf_Name(file_name);//通过串口输入源文件路径名/dir/file.txt或者0:dir/file.txt或者0:/dir/file.txt

    res = f_open(&Make_file, file_name,  FA_OPEN_ALWAYS | FA_WRITE); //可写方式打开 没有文件则创建 
	 printf("\r\n open_ok:>");
	 die(res);
    res = f_lseek(&Make_file, Make_file.fsize); //指针移到文件最后  
      printf("\r\n seek_ok:>");
	 die(res);
	res = f_write(&Make_file, Storage_buffer, (sizeof (Storage_buffer))-1 , &bw); //每次需要写入的数据字节数，去掉最后的\0所以-1  
     printf("\r\n write_ok:>");
	 die(res);
	res = f_lseek(&Make_file, Make_file.fsize); //指针移到文件最后  
	f_close(&Make_file);//关闭文件
	printf("\r\n close_ok:>");

	printf("\r\n写文件测试OK!\r\n");

	// Unregister a work area before discard it
    f_mount(0, NULL);
}

//The f_read function reads data from a file.
//在RAM里面调试这个程序的时候，总出现莫名其妙的错误，最后怀疑是堆寨溢出了，证明果然是这样
//把Stack_Size   EQU   0x00000400  改为 Stack_Size   EQU  0x00000800就正常了
//所以以后要特别注意这个问题。

u8 StrToData(u8 * data, u8 len)
{
	u8 ltemp;
	if(len == 1)
	{
		ltemp = data[0]-0x30;

	}
	else if(len == 2)
	{
		ltemp = (data[0]-0x30)*10 + (data[1]-0x30);

	 }
	//else if(len == 3)
		//ltemp = (data[0]-0x30)*100 + (data[1]-0x30)*10 + [data[2] - 0x30];

	return ltemp;
	
}


#endif


/*
int main(void)
{
///////////////////////////////////////////////////////////
UART1GPIO_config();//串口IO口配置
USART1_config();//串口初始化波特率为19200
//UART1NVIC_config();//配置中断
///////////////////////////////////////////////////////////
SDCard_SpiInit();//SD卡的IO口及SPI模块初始化，PA2插入检测（H—>L）;PA3 CD_CS片选；PA5 SCK；PA6 MISO；PA7 MOSI；低速模式
//SD_Init();//SD卡初始化，如果使用的FATFS则可以不用这句，因为FATFS在打开文件等操作时已经调用了它
Test_f_getfree();//获取SD卡的容量和剩余容量
Test_f_read();//SD卡读文件测试
Test_f_write();//SD卡写文件测试
/////////////////////////////////////////////////////////// 
    while (1)
    {
	//printf("好的");
    }
}*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FRESULT Test_f_mkfs(BYTE drv)//对卡进行格式化，驱动器号一般为0
{
    FATFS fs;            // Work area (file system object) for logical drive
    FRESULT res;         // FatFs function common result code

	//检测磁盘是否插好
	//if(disk_detect_OK()==FALSE ) return ;
	//printf("\r\n inaert_ok:>");
	// Register a work area for logical drive 0
	f_mount(drv, &fs);
	res=f_mkfs(drv, 0, 0);/* Create a file system on the drive 在驱动器创建一个文件系统（驱动器号，分区规则（0:FDISK, 1:SFD），分配的单元大小）*/
	//die(res);
	f_mount(0, NULL);
	return res;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FRESULT Test_f_mkdir(const TCHAR* path)//创建一个新目录,输入目录的路径Test_f_mkdir("/dir");只能一级一级的建立目录	
{
    FATFS fs;            // Work area (file system object) for logical drive
    FRESULT res;         // FatFs function common result code

	//检测磁盘是否插好
	//if(disk_detect_OK()==FALSE ) return ;
	//printf("\r\n inaert_ok:>");
	// Register a work area for logical drive 0
	f_mount(0, &fs);
	res=f_mkdir(path);//创建一个新目录
	//die(res);
	f_mount(0, NULL);
	return res;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FRESULT Test_f_deldir(const TCHAR* path)//删除一个目录,输入目录的路径;目录里的内容是空的才能删除Test_f_deldir("/dir");	
{
    FATFS fs;            // Work area (file system object) for logical drive
	//DIR	dir;
    FRESULT res;         // FatFs function common result code

	//检测磁盘是否插好
	//if(disk_detect_OK()==FALSE ) return ;
	//printf("\r\n inaert_ok:>");
	// Register a work area for logical drive 0
	f_mount(0, &fs);
	//f_opendir (&dir,path);
	res=f_unlink(path);//删除一个目录
	//die(res);
	f_mount(0, NULL);
	return res;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FRESULT Test_f_readdir(const TCHAR* path,char (*filename)[13])//获取目录中的文件,输入目录的路径;输出文件名和文件夹名Test_f_readdir("/DIR",name);char name[5][13]	
{
    FATFS fs;            // Work area (file system object) for logical drive
		DIR	dir;
		FILINFO finf;
    FRESULT res;         // FatFs function common result code

	//检测磁盘是否插好
	//if(disk_detect_OK()==FALSE ) return ;
	//printf("\r\n inaert_ok:>");
	// Register a work area for logical drive 0
	f_mount(0, &fs);
	f_opendir (&dir,path);

	while(((res=f_readdir(&dir,&finf))==FR_OK)&&(finf.fname[0]))//获取目录中的文件	
	{strcpy(*(filename++),finf.fname);
	//printf("%s",finf.fname);
	}
	//die(res);
	f_mount(0, NULL);
	return res;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FRESULT Test_f_creat(const TCHAR* path)//建立文件，输入文件路径和文件名
{
    FATFS fs;            // Work area (file system object) for logical drive
		FIL file;
    FRESULT res;         // FatFs function common result code

	//检测磁盘是否插好
	//if(disk_detect_OK()==FALSE ) return ;
	//printf("\r\n inaert_ok:>");
	// Register a work area for logical drive 0
	f_mount(0, &fs);
	res=f_open(&file,path,FA_CREATE_NEW); //创建一个新文件。如果文件已存在，则创建失败//FA_CREATE_NEW创建一个新文件。如果文件已存在，则创建失败。
                                                                                      //FA_CREATE_ALWAYS	//创建一个新文件。如果文件已存在，则它将被截断并覆盖。
                                                                                      //FA_OPEN_ALWAYS    //如果文件存在，则打开；否则，创建一个新文件。
	//die(res);
	f_close(&file);//关闭文件
	f_mount(0, NULL);
	return res;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FRESULT Test_f_delfile(const TCHAR* path)//删除一个文件,输入文件的路径;Test_f_delfile("/dir");	
{
    FATFS fs;            // Work area (file system object) for logical drive
	//DIR	dir;
    FRESULT res;         // FatFs function common result code

	//检测磁盘是否插好
	//if(disk_detect_OK()==FALSE ) return ;
	//printf("\r\n inaert_ok:>");
	// Register a work area for logical drive 0
	f_mount(0, &fs);
	//f_opendir (&dir,path);
	res=f_unlink(path);//删除一个文件
	//die(res);
	f_mount(0, NULL);
	return res;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FRESULT Test_f_readfile(const TCHAR* path,char *buff,DWORD ofs,UINT strl)//读文件的数据，如果没有此文件则返回错误;输入文件的路径名，内容缓存，读取开始偏移指针，读取的字节数
{													//Test_f_readfile("/dir/r.txt",str,0,sizeof(strr));
    FATFS fs;            // Work area (file system object) for logical drive
    FIL file;            // file objects
    FRESULT res;         // FatFs function common result code
    UINT br;             // File R count 文件读回的字节计数
	//u16 i;
	//检测磁盘是否插好
	//if( disk_detect_OK()==FALSE ) return;
  // Register a work area for logical drive 0
	//for(i=0;i<sizeof(buff);i++) buff[i]='\0';
    f_mount(0, &fs);
	//Open source file
    res = f_open(&file, path, FA_OPEN_EXISTING | FA_READ);//打开存在的文件，如果没有则返回错误
	//die(res);
	res = f_lseek(&file,ofs); //指针移到文件ofs个字节处
	//buffer空间设大一点，会提高读的速度。
	//如果文件实际大小512byte，
	//设为buffer[512]时，只需要循坏一次，如果设为buffer[1]，需要循坏512次。
	//for (;;) 
	{
		//for(i=0;i<sizeof(buff);i++) buff[i]='\0';//清除缓存

		res = f_read(&file, buff, strl, &br);
        //if (res ||(br == 0)) break;   // error or eof如果错误或者到文件末尾则退出
		
		//printf("%s",buff);	
    }		    
    // Close all files
    f_close(&file);
    // Unregister a work area before discard it
    f_mount(0, NULL);
	return res;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FRESULT Test_f_writefile(const TCHAR* path,char *buff,DWORD ofs,UINT strl)//写数据到文件，如果没有此文件则创建文件;输入文件的路径名，内容缓存，写开始偏移指针，写的字节数
{																		  //Test_f_writefile("/dir/r.txt",strw,0,strlen(strw));
    FATFS fs;            // Work area (file system object) for logical drive
    FRESULT res;         // FatFs function common result code
	FIL file;
	UINT bw;             //文件写入的字节计数
	//检测磁盘是否插好
	//if( disk_detect_OK()==FALSE ) return;
    // Register a work area for logical drive 0
    f_mount(0, &fs);
    res = f_open(&file, path,  FA_OPEN_ALWAYS | FA_WRITE); //可写方式打开 没有文件则创建 
	 //die(res);
    res = f_lseek(&file, ofs); //指针移到文件ofs处；输入file.fsize指针移到文件最后 
	 //die(res);
	res = f_write(&file, buff, strl, &bw); //每次需要写入的数据字节数;如果btr为strlen(buff)则写入所有的字符
	 //die(res);
	//res = f_lseek(&file, Make_file.fsize); //指针移到文件最后,一边再一次的写入  
	f_close(&file);//关闭文件
	// Unregister a work area before discard it
    f_mount(0, NULL);
	return res;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
FRESULT Test_f_getfreem(DWORD *freem)//获取卡的剩余容量单位为M
{
    FATFS fs;
	FATFS *pfs;
	FRESULT res;         // FatFs function common result code
	//检测磁盘是否插好
	//if( disk_detect_OK()==FALSE ) return;
	// Register a work area for logical drive 0
	pfs=&fs;//指向
    f_mount(0, &fs);//安装FATFS，就是给FATFS分配空间
    // Get free clusters
    res = f_getfree("/",freem,&pfs);//必须是根目录，默认磁盘0;"/"或者"0:/"
	//die(res);
	if ( res==FR_OK ) 
    {
	    // Get free space
        //printf("\r\n%d MB total disk space.\r\n%d MB available on the disk.\r\n",
               //(DWORD)(pfs->n_fatent - 2) * (pfs->csize) /2/1024,//总的磁盘空间M	=（总簇数-2）*每簇的扇区数/2/1024=可用簇数*每簇的扇区数/2/1024
                 //(*freem)*(pfs->csize)/2/1024);//空闲的磁盘空间M=剩余簇数*每簇的扇区数/2/1024
	 *freem=((*freem)*pfs->csize)/2/1024;
	}
	//else die(res);//测试函数执行结果分析
	// Unregister a work area before discard it
    f_mount(0, NULL);//卸载FATFS，就是释放FATFS结构体所占空间
	return res;
}



