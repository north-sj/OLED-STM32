	

#ifndef __TEST_FATFS_h
#define __TEST_FATFS_h

#define TEST_FATFS_EN
#ifdef TEST_FATFS_EN

#include "config.h" 
#include "integer.h"
//#include "diskio.h" 
#define TRUE 1
#define FALSE 0
//#define NULL (void*)0

typedef	unsigned char  BOOL;

FRESULT Test_f_mkfs(BYTE drv);//对卡进行格式化，驱动器号一般为0
FRESULT Test_f_mkdir(const TCHAR* );//创建一个新目录,输入目录的路径
FRESULT Test_f_deldir(const TCHAR* path);//删除一个目录,输入目录的路径	
FRESULT Test_f_readdir(const TCHAR* path,char (*filename)[13]);//获取目录中的文件,输入目录的路径
FRESULT Test_f_creat(const TCHAR* path);//建立文件输入文件路径和文件名
FRESULT Test_f_delfile(const TCHAR* path);//删除一个文件,输入目录的路径
FRESULT Test_f_readfile(const TCHAR* path,char *buff,DWORD ofs,UINT strl);//读文件的数据，如果没有此文件则返回错误;输入文件的路径名，内容缓存，读取开始偏移指针，读取的字节数										
FRESULT Test_f_writefile(const TCHAR* path,char *buff,DWORD ofs,UINT strl);//写数据到文件，如果没有此文件则创建文件;输入文件的路径名，内容缓存，写开始偏移指针，写的字节数
FRESULT Test_f_getfreem(DWORD *freem);//获取卡的剩余容量单位为M
//The f_getfree function gets number of the free clusters
void Test_f_getfree(void);
//The f_read function reads data from a file.
void Test_f_read(void);
//The f_write writes data to a file.
void Test_f_write(void);
//The f_mkdir function creates a new directory
//void Test_f_mkdir(void);
//The f_readdir function reads directory entries.
//void Test_f_readdir(void);
//Rename file or directory
//void Test_f_rename(void);
//The f_unlink removes file or directory
//void Test_f_unlink(void);
//The f_mkfs fucntion creates a file system on the drive.


extern BYTE disk_detect_OK(void);
u8 StrToData(u8 * data, u8 len);

#endif

#endif


