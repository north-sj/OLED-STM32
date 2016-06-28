/*---------------------------------------------------------------------------/
/  FatFs - FAT file system module include file  R0.09     (C)ChaN, 2011
/----------------------------------------------------------------------------/
/ FatFs module is a generic FAT file system module for small embedded systems.
/ This is a free software that opened for education, research and commercial
/ developments under license policy of following trems.
/
/  Copyright (C) 2011, ChaN, all right reserved.
/
/ * The FatFs module is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/----------------------------------------------------------------------------*/

#ifndef _FATFS
#define _FATFS	6502	/* Revision ID */

#ifdef __cplusplus
extern "C" {
#endif

#include "integer.h"	/* Basic integer types */
#include "ffconf.h"		/* FatFs configuration options */

#if _FATFS != _FFCONF
#error Wrong configuration file (ffconf.h).
#endif



/* Definitions of volume management */

#if _MULTI_PARTITION		/* Multiple partition configuration 该变量定义为1时，支持一个磁盘的多个分区*/
typedef struct {
	BYTE pd;	/* Physical drive number */
	BYTE pt;	/* Partition: 0:Auto detect, 1-4:Forced partition) */
} PARTITION;
extern PARTITION VolToPart[];	/* Volume - Partition resolution table */
#define LD2PD(vol) (VolToPart[vol].pd)	/* Get physical drive number 获得磁盘对应的物理磁盘 */
#define LD2PT(vol) (VolToPart[vol].pt)	/* Get partition index 获得磁盘对应的分区*/

#else						/* Single partition configuration */
#define LD2PD(vol) (vol)	/* Each logical drive is bound to the same physical drive number */
#define LD2PT(vol) 0		/* Always mounts the 1st partition or in SFD */

#endif



/* Type of path name strings on FatFs API */

#if _LFN_UNICODE			/* Unicode string */
#if !_USE_LFN
#error _LFN_UNICODE must be 0 in non-LFN cfg.
#endif
#ifndef _INC_TCHAR
typedef WCHAR TCHAR;
#define _T(x) L ## x
#define _TEXT(x) L ## x
#endif

#else						/* ANSI/OEM string */
#ifndef _INC_TCHAR
typedef char TCHAR;
#define _T(x) x
#define _TEXT(x) x
#endif

#endif



/* File system object structure (FATFS)文件系统的结构体变量 */

typedef struct {
	BYTE	fs_type;		/* FAT sub-type (0:Not mounted) 文件系统类型（0为无文件系统）*/
	BYTE	drv;			/* Physical drive number 物理驱动号；一般为0； f_chdrive();影响此值*/
	BYTE	csize;			/* Sectors per cluster (1,2,4...128) 每簇的扇区数*/
	BYTE	n_fats;			/* Number of FAT copies (1,2) FATFS的副本数 */
	BYTE	wflag;			/* win[] dirty flag (1:must be written back) 文件是否改动的标志，为1时要回写*/
	BYTE	fsi_flag;		/* fsinfo dirty flag (1:must be written back) 文件系统信息是否改动过标识*/
	WORD	id;				/* File system mount ID 文件系统加载ID*/
	WORD	n_rootdir;		/* Number of root directory entries (FAT12/16) 根目录区目录项的数目*/
#if _MAX_SS != 512
	WORD	ssize;			/* Bytes per sector (512, 1024, 2048 or 4096) 每扇区的字节数*/
#endif
#if _FS_REENTRANT
	_SYNC_t	sobj;			/* Identifier of sync object 允许重入，则定义同步对象*/
#endif
#if !_FS_READONLY//文件为可写
	DWORD	last_clust;		/* Last allocated cluster 最后分配的的一个簇*/
	DWORD	free_clust;		/* Number of free clusters 空簇的数目*/
	DWORD	fsi_sector;		/* fsinfo sector (FAT32) 文件系统信息扇区*/
#endif
#if _FS_RPATH
	DWORD	cdir;			/* Current directory start cluster (0:root) 当前目录的开始簇与f_chdir("/dir1");函数相关 */
#endif
	DWORD	n_fatent;		/* Number of FAT entries (= number of clusters + 2)，簇的总量;此值-2就是磁盘的可用簇数目 */
	DWORD	fsize;			/* Sectors per FAT 每个FAT的扇区数*/
	DWORD	fatbase;		/* FAT start sector FAT开始的扇区 */
	DWORD	dirbase;		/* Root directory start sector (FAT32:Cluster#)FAT32的根目录开始扇区 */
	DWORD	database;		/* Data start sector数据开始扇区 */
	DWORD	winsect;		/* Current sector appearing in the win[] 目前的扇区在win[]里面*/
	BYTE	win[_MAX_SS];	/* Disk access window for Directory, FAT (and Data on tiny cfg)  磁盘缓存*/
} FATFS;



/* File object structure (FIL) */

typedef struct {
	FATFS*	fs;				/* Pointer to the owner file system object 指向相应文件系统对象*/
	WORD	id;				/* Owner file system mount ID 文件系统加载ID*/
	BYTE	flag;			/* File status flags 文件状态标志*/
	BYTE	pad1;
	DWORD	fptr;			/* File read/write pointer (0 on file open) 文件的读写指针*/
	DWORD	fsize;			/* File size 文件的大小*/
	DWORD	sclust;			/* File start cluster (0 when fsize==0) 文件开始的簇号*/
	DWORD	clust;			/* Current cluster 当前的簇号*/
	DWORD	dsect;			/* Current data sector 当前数据的所在扇区*/
#if !_FS_READONLY
	DWORD	dir_sect;		/* Sector containing the directory entry 该文件目录项对应所在的扇区*/
	BYTE*	dir_ptr;		/* Ponter to the directory entry in the window 目录的指针*/
#endif
#if _USE_FASTSEEK
	DWORD*	cltbl;			/* Pointer to the cluster link map table (null on file open)指向簇的链路映射表 */
#endif
#if _FS_SHARE
	UINT	lockid;			/* File lock ID (index of file semaphore table) 文件锁定号（索引文件信号量表）*/
#endif
#if !_FS_TINY
	BYTE	buf[_MAX_SS];	/* File data read/write buffer 文件读写的缓存*/
#endif
} FIL;



/* Directory object structure (DIR) */

typedef struct {
	FATFS*	fs;				/* Pointer to the owner file system object  指向相应文件系统对象*/
	WORD	id;				/* Owner file system mount ID 文件系统加载ID*/
	WORD	index;			/* Current read/write index number 当前读写的索引号*/
	DWORD	sclust;			/* Table start cluster (0:Root dir) 开始的簇*/
	DWORD	clust;			/* Current cluster 当前簇*/
	DWORD	sect;			/* Current sector 当前扇区*/
	BYTE*	dir;			/* Pointer to the current SFN entry in the win[] */
	BYTE*	fn;				/* Pointer to the SFN (in/out) {file[8],ext[3],status[1]} */
#if _USE_LFN
	WCHAR*	lfn;			/* Pointer to the LFN working buffer 指向长文件名缓存区*/
	WORD	lfn_idx;		/* Last matched LFN index number (0xFFFF:No LFN) */
#endif
} DIR;



/* File status structure (FILINFO) */

typedef struct {
	DWORD	fsize;			/* File size 文件大小*/
	WORD	fdate;			/* Last modified date 最后修改的数据*/
	WORD	ftime;			/* Last modified time 最后修改的时间*/
	BYTE	fattrib;		/* Attribute 属性*/
	TCHAR	fname[13];		/* Short file name (8.3 format) 短文件名*/
#if _USE_LFN
	TCHAR*	lfname;			/* Pointer to the LFN buffer 指向长文件名缓存*/
	UINT 	lfsize;			/* Size of LFN buffer in TCHAR 长文件名的缓存大小*/
#endif
} FILINFO;



/* File function return code (FRESULT) */

typedef enum {
	FR_OK = 0,				/* (0) Succeeded */
	FR_DISK_ERR,			/* (1) A hard error occured in the low level disk I/O layer */
	FR_INT_ERR,				/* (2) Assertion failed */
	FR_NOT_READY,			/* (3) The physical drive cannot work */
	FR_NO_FILE,				/* (4) Could not find the file */
	FR_NO_PATH,				/* (5) Could not find the path */
	FR_INVALID_NAME,		/* (6) The path name format is invalid */
	FR_DENIED,				/* (7) Acces denied due to prohibited access or directory full */
	FR_EXIST,				/* (8) Acces denied due to prohibited access */
	FR_INVALID_OBJECT,		/* (9) The file/directory object is invalid */
	FR_WRITE_PROTECTED,		/* (10) The physical drive is write protected */
	FR_INVALID_DRIVE,		/* (11) The logical drive number is invalid */
	FR_NOT_ENABLED,			/* (12) The volume has no work area */
	FR_NO_FILESYSTEM,		/* (13) There is no valid FAT volume */
	FR_MKFS_ABORTED,		/* (14) The f_mkfs() aborted due to any parameter error */
	FR_TIMEOUT,				/* (15) Could not get a grant to access the volume within defined period */
	FR_LOCKED,				/* (16) The operation is rejected according to the file shareing policy */
	FR_NOT_ENOUGH_CORE,		/* (17) LFN working buffer could not be allocated */
	FR_TOO_MANY_OPEN_FILES,	/* (18) Number of open files > _FS_SHARE */
	FR_INVALID_PARAMETER	/* (19) Given parameter is invalid */
} FRESULT;



/*--------------------------------------------------------------*/
/* FatFs module application interface                           */

FRESULT f_mount (BYTE, FATFS*);						/* Mount/Unmount a logical drive 安装/卸载一个逻辑驱动器 （被安装或卸载的逻辑驱动器号，指向一个新的文件系统对象）*/
FRESULT f_open (FIL*, const TCHAR*, BYTE);			/* Open or create a file打开或创建一个文件（指向一个空白的文件指针，指向文件名，访问模式和文件打开模式标志） */
FRESULT f_read (FIL*, void*, UINT, UINT*);			/* Read data from a file从文件中读数据（文件结构体（文件打开函数中得到），文件读取缓冲区，读取的字节数，指针读取的字节数） */
FRESULT f_lseek (FIL*, DWORD);						/* Move file pointer of a file object 移动文件指针（文件对象，从文件顶部开始的文件指针）*/
FRESULT f_close (FIL*);								/* Close an open file object 关闭打开的文件结构体*/
FRESULT f_opendir (DIR*, const TCHAR*);				/* Open an existing directory 打开一个存在的目录（指向一个创建的目录结构体，指向目录路径）*/
FRESULT f_readdir (DIR*, FILINFO*);					/* Read a directory item 读取一个目录项（指向打开的目录结构体，指向返回的文件信息的结构体）*/
FRESULT f_stat (const TCHAR*, FILINFO*);			/* Get file status 获取文件状态（指向文件路径，指向返回的文件信息结构体）*/
FRESULT f_write (FIL*,const void*, UINT, UINT*);	/* Write data to a file 写数据到文件（指向文件结构体，指向被写数据，被写数据的数目，指向被写数据数目的指针）*/
FRESULT f_getfree (const TCHAR*, DWORD*, FATFS**);	/* Get number of free clusters on the drive 获取驱动器的空闲簇号（指向驱动器号（根目录），指向返回空簇号的变量，指针指向返回相应的文件系统的结构体）*/
FRESULT f_truncate (FIL*);							/* Truncate file 截短文件（指向文件的结构体）*/
FRESULT f_sync (FIL*);								/* Flush cached data of a writing file 刷新写入文件的缓存数据（指向文件的结构体）*/
FRESULT f_unlink (const TCHAR*);					/* Delete an existing file or directory 删除存在的文件或目录（指向文件或目录的路径）*/
FRESULT	f_mkdir (const TCHAR*);						/* Create a new directory 创建一个新目录（指向目录的路径）*/
FRESULT f_chmod (const TCHAR*, BYTE, BYTE);			/* Change attriburte of the file/dir 改变文件或目录的属性（指向目录的路径，属性位，去改变的属性掩码）*/
FRESULT f_utime (const TCHAR*, const FILINFO*);		/* Change timestamp of the file/dir 改变文件或目录的时间戳（指向文件或目录名，指向被设置的时间戳）*/
FRESULT f_rename (const TCHAR*, const TCHAR*);		/* Rename/Move a file or directory 重命名或移动一个文件或目录（指向旧的文件名，指向新的文件名）*/
FRESULT f_chdrive (BYTE);							/* Change current drive 改变当前驱动（逻辑驱动器号）*/
FRESULT f_chdir (const TCHAR*);						/* Change current directory 改变当前目录（指向目录路径）*/
FRESULT f_getcwd (TCHAR*, UINT);					/* Get current directory 获取当前目录（指向目录路径，路径的大小）*/
FRESULT f_forward (FIL*, UINT(*)(const BYTE*,UINT), UINT, UINT*);	/* Forward data to the stream 数据流（指向文件的结构体，指向流函数，向前的字节数，指向向前的字节数）*/
FRESULT f_mkfs (BYTE, BYTE, UINT);					/* Create a file system on the drive 在驱动器创建一个文件系统（驱动器号，分区规则（0:FDISK, 1:SFD），分配的单元大小）*/
FRESULT	f_fdisk (BYTE, const DWORD[], void*);		/* Divide a physical drive into some partitions 将物理驱动器分区（物理驱动器号，指向每一个分区大小的尺寸表，指向缓存）*/
int f_putc (TCHAR, FIL*);							/* Put a character to the file 放一个字符到文件（被输出的字符，指向文件结构体）*/
int f_puts (const TCHAR*, FIL*);					/* Put a string to the file 放一个字符串到文件（指向被输出的字符串，指向文件结构体）*/
int f_printf (FIL*, const TCHAR*, ...);				/* Put a formatted string to the file 放有格式的字符串到文件（指向文件结构体，指向字符串，可选参数）*/
TCHAR* f_gets (TCHAR*, int, FIL*);					/* Get a string from the file 从文件中获取一个字符串（指向字符串的缓存，字符串缓存的大小，指向文件结构体）*/

#define f_eof(fp) (((fp)->fptr == (fp)->fsize) ? 1 : 0)
#define f_error(fp) (((fp)->flag & FA__ERROR) ? 1 : 0)
#define f_tell(fp) ((fp)->fptr)
#define f_size(fp) ((fp)->fsize)

#ifndef EOF
#define EOF (-1)
#endif




/*--------------------------------------------------------------*/
/* Additional user defined functions                            */

/* RTC function */
#if !_FS_READONLY//只读模式
DWORD get_fattime (void);
#endif

/* Unicode support functions */ //使用长文件名
#if _USE_LFN						/* Unicode - OEM code conversion */
WCHAR ff_convert (WCHAR, UINT);		/* OEM-Unicode bidirectional conversion */
WCHAR ff_wtoupper (WCHAR);			/* Unicode upper-case conversion */
#if _USE_LFN == 3					/* Memory functions */
void* ff_memalloc (UINT);			/* Allocate memory block */
void ff_memfree (void*);			/* Free memory block */
#endif
#endif

/* Sync functions */
#if _FS_REENTRANT   //可重入性
int ff_cre_syncobj (BYTE, _SYNC_t*);/* Create a sync object 创建同步对象*/
int ff_req_grant (_SYNC_t);			/* Lock sync object 锁定同步对象*/
void ff_rel_grant (_SYNC_t);		/* Unlock sync object 解锁同步对象*/
int ff_del_syncobj (_SYNC_t);		/* Delete a sync object 删除同步对象*/
#endif




/*--------------------------------------------------------------*/
/* Flags and offset address                                     */


/* File access control and file status flags (FIL.flag) 文件访问控制及文件状态标识*/

#define	FA_READ				0x01 //读文件
#define	FA_OPEN_EXISTING	0x00 //打开文件。如果文件不存在，则打开失败。(默认)
#define FA__ERROR			0x80

#if !_FS_READONLY
#define	FA_WRITE			0x02 //写文件
#define	FA_CREATE_NEW		0x04 //创建一个新文件。如果文件已存在，则创建失败。
#define	FA_CREATE_ALWAYS	0x08 //创建一个新文件。如果文件已存在，则它将被截断并覆盖。
#define	FA_OPEN_ALWAYS		0x10 //如果文件存在，则打开；否则，创建一个新文件。
#define FA__WRITTEN			0x20
#define FA__DIRTY			0x40
#endif


/* FAT sub type (FATFS.fs_type) */

#define FS_FAT12	1//文件系统的类型
#define FS_FAT16	2
#define FS_FAT32	3


/* File attribute bits for directory entry 文件属性*/

#define	AM_RDO	0x01	/* Read only */
#define	AM_HID	0x02	/* Hidden */
#define	AM_SYS	0x04	/* System */
#define	AM_VOL	0x08	/* Volume label */
#define AM_LFN	0x0F	/* LFN entry */
#define AM_DIR	0x10	/* Directory */
#define AM_ARC	0x20	/* Archive */
#define AM_MASK	0x3F	/* Mask of defined bits */


/* Fast seek feature */
#define CREATE_LINKMAP	0xFFFFFFFF



/*--------------------------------*/
/* Multi-byte word access macros  */

#if _WORD_ACCESS == 1	/* Enable word access to the FAT structure */
#define	LD_WORD(ptr)		(WORD)(*(WORD*)(BYTE*)(ptr))
#define	LD_DWORD(ptr)		(DWORD)(*(DWORD*)(BYTE*)(ptr))
#define	ST_WORD(ptr,val)	*(WORD*)(BYTE*)(ptr)=(WORD)(val)
#define	ST_DWORD(ptr,val)	*(DWORD*)(BYTE*)(ptr)=(DWORD)(val)
#else					/* Use byte-by-byte access to the FAT structure */
#define	LD_WORD(ptr)		(WORD)(((WORD)*((BYTE*)(ptr)+1)<<8)|(WORD)*(BYTE*)(ptr))
#define	LD_DWORD(ptr)		(DWORD)(((DWORD)*((BYTE*)(ptr)+3)<<24)|((DWORD)*((BYTE*)(ptr)+2)<<16)|((WORD)*((BYTE*)(ptr)+1)<<8)|*(BYTE*)(ptr))
#define	ST_WORD(ptr,val)	*(BYTE*)(ptr)=(BYTE)(val); *((BYTE*)(ptr)+1)=(BYTE)((WORD)(val)>>8)
#define	ST_DWORD(ptr,val)	*(BYTE*)(ptr)=(BYTE)(val); *((BYTE*)(ptr)+1)=(BYTE)((WORD)(val)>>8); *((BYTE*)(ptr)+2)=(BYTE)((DWORD)(val)>>16); *((BYTE*)(ptr)+3)=(BYTE)((DWORD)(val)>>24)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _FATFS */
