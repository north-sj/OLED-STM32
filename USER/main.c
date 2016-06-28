//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : north-sj
//  生成日期   : 2016-0101
//  最近修改   : 
//  功能描述   : OLED SPI4接口演示例程(STM32系列)
//              说明: 主程序
//              ----------------------------------------------------------------
//              GND  接电源地
//              VCC  接5V或3.3v电源
//              D0   接PB13（SCL）
//              D1   接PB15（SDA）
//              RES  接PB11
//              DC   接PB10
//              CS   接PB12               
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 2016
// 作    者   : north-sj
// 修改内容   : 创建文件
// 
//
//
//******************************************************************************/

#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "usart.h"
#include "config.h"
#include "string.h"
#include "integer.h"
#include "TEST_FATFS.h"
/* 
*说明: 
*PA0:LED1;PA1:LED2; 
*PA9:USART1_TX;PA10:USART1_RX 
*/  
#include "stm32f10x.h"  
#include "stm32f10x_rcc.h"  
#include "stm32f10x_gpio.h"  
#include "stm32f10x_tim.h"  
#include "stm32f10x_exti.h"  
#include "system_stm32f10x.h"  
#include "misc.h" 

/* Private variables ---------------------------------------------------------*/
FATFS fs;         				/* Work area (file system object) for logical drive */
FIL fsrc;         				/* file objects */   
FRESULT res;

UINT br;									//FatFs
/* Private function prototypes -----------------------------------------------*/
uint8_t textFileBuffer[] = "Thank you for using STM32F103 ！ \r\n";	//测试写入字符串
char path[128]="0:";																								//路径存储
uint32_t file_byte;																									//存储Rt.Dat文件大小
uint32_t ls_move=0;																									//存储文件指针移动
unsigned char G_Bin1[1024];																					//BIN图形显存1
unsigned char G_Bin2[1024];																					//BIN图形显存2
extern int k_1;
extern int k_2;

int SD_TotalSize(void);
void USART_Configuration(void);
FRESULT scan_files (char* path);


void RCC_Configuration(void);  
void GPIO_Configuration(void);   
void TIM3_Configuration(void);  
void NVIC_Configuration(void);



void Read_Bin1(void)
{
		if(ls_move>file_byte-1024)
			{
				ls_move=0;
				printf(" \n");
				printf("播放完毕，返回开头 \r\n");
				printf(" \n");
			}
			br=1;
			res=f_lseek(&fsrc,ls_move);
			res=f_read(&fsrc,G_Bin1,sizeof(G_Bin1),&br);//读取Bin数据放入G_Bin1数组中
			ls_move=ls_move+1024;
}


void Read_Bin2(void)
{
		if(ls_move>file_byte-1024)
			{
				ls_move=0;
				printf(" \n");
				printf("播放完毕，返回开头 \r\n");
				printf(" \n");
			}
			br=1;
			res=f_lseek(&fsrc,ls_move);
			res=f_read(&fsrc,G_Bin2,sizeof(G_Bin2),&br);//读取Bin数据放入G_Bin2数组中
			ls_move=ls_move+1024;
}



 int main(void)
 {	
		//uint8_t t;
		//char strw[512]={"qwertyuiopasdfasddfghjk"},strr[512];
		//DWORD free=0;
	 
		/*******************************************************/
		uart_init(115200);		//初始化串口，并设置波特率为115200
		/*******************************************************/
		SDCard_SpiInit();				//SD卡的IO口及SPI模块初始化，PA2插入检测（H—>L）;PA3 CD_CS片选；PA5 SCK；PA6 MISO；PA7 MOSI；低速模式
		//												//SD_Init();//SD卡初始化，如果使用的FATFS则可以不用这句，因为FATFS在打开文件等操作时已经调用了它
		//Test_f_getfreem(&free);	//获取SD卡的剩余容量单位为M
		//printf("TF卡剩余空间为 %d\n MB\n",(int)free);
		/*******************************************************/ 

	 	//printf("\r\n\r\n%48s\r\n","<未检测到磁盘,请检查SD卡是否插好...>");

	 
	 
		if( SD_Insert_Status() == 0 )
			{
				printf("---- SD卡已经检测到 \r\n");
			}
			else
			{
				printf("---- 请连接SD卡 \r\n");
				//while( SD_Insert_Status != 0 );
				printf("---- 检测到SD卡已连接 \r\n");
				delay_ms(1864);
			}

		f_mount(0,&fs);	

			//res = f_open( &fsrc , "0:/Demo.TXT" , FA_CREATE_ALWAYS | FA_WRITE);
				res = f_open( &fsrc , "0:/Demo.TXT" , FA_CREATE_NEW | FA_WRITE);	
					
		
    if ( res == FR_OK )
			{ 
				/* Write buffer to file */
				res = f_write(&fsrc, textFileBuffer, sizeof(textFileBuffer), &br);     
 
				printf("Demo.TXT 创建成功        \r\n");

				/*close file */
				f_close(&fsrc);      
			}
			else if ( res == FR_EXIST )
			{
				printf("Demo.TXT 已存在磁盘      \r\n");
			}

		scan_files(path);	//遍历根目录下文件
		SD_TotalSize();		//计算剩余空间大小

/********************* 读取badapple.bin代码  *********************/
		
			 res = f_open( &fsrc , "0:/badapple.bin" , FA_READ);	
			 file_byte=f_size(&fsrc);
		   if (res==FR_OK)
				{
					printf("在根目录下找到badapple.bin文件  \r\n");
					printf("badapple.bin文件大小为%d字节 \r\n",file_byte);//显示文件大小
				  //res=f_read(&fsrc,testRead,sizeof(testRead),&br);		//读取
				  //printf("\r\n %s ",testRead);
				 
				}
				else if(res==FR_NO_FILE)
					{
						printf("根目录下找不到badapple.bin数据文件  \r\n");
						printf(" \n");
					}

/****************************************************************/					
					
										
	 
		delay_init();							//延时函数初始化	  
		//NVIC_Configuration(); 	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
		//LED_Init();			     		//LED端口初始化
		OLED_Init();					   	//初始化OLED  
		OLED_Clear(); 						//清屏	
		//t=' ';



		Read_Bin1();							//读取第一帧
		Read_Bin2();							//读取第二帧

/************************测试OLED显示*************************/
		OLED_Clear();
		LED_ON;
		
		//OLED_ShowCHinese(0,0,0);	//意
		//OLED_ShowCHinese(25,0,1);	//法
		//OLED_ShowCHinese(50,0,2);	//半
		//OLED_ShowCHinese(75,0,3);	//导
		//OLED_ShowCHinese(100,0,4);//体			
		//OLED_ShowString(0,3,(u8*)"0.96' OLED TEST");
		////OLED_ShowString(8,2,"ZHONGJINGYUAN");  
	  ////	OLED_ShowString(20,4,"2014/05/01");  
		//OLED_ShowString(0,6,(u8*)"ASCII:");  
		//OLED_ShowString(63,6,(u8*)"CODE:");  
		//OLED_ShowChar(48,6,t);		//显示ASCII字符	   
		//t++;
		//if(t>'~')t=' ';
		//OLED_ShowNum(103,6,t,3,16);//显示ASCII字符的码值 	

		delay_ms(1000);
		LED_OFF;
		OLED_DrawBMP(0,0,128,8,BMP1);//图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		OLED_DrawBMP(0,0,128,8,gImage_1);
		delay_ms(1000);
		//OLED_DrawBMP(0,0,128,8,G_Bin1);
		//delay_ms(1864);

/*************************************************************/	

/***********************中断配置及初始化**********************/
    SystemInit();  
    RCC_Configuration();  
    GPIO_Configuration();   
    TIM3_Configuration();  
    NVIC_Configuration(); 											//中断频率20Hz
/*************************************************************/
		GPIO_Configuration();
	while(1) 
	{		
		if(k_1==0)
			{
				Read_Bin1();
				k_1=1;
			}
		if(k_2==0)
			{
				Read_Bin2();
				k_2=1;
			}

	}	  
	
}




void RCC_Configuration(void)//RCC配置函数
{     
  //USART2和USART3都在APB1上而USART1是在APB2上的
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);     
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);     
}   
  
void GPIO_Configuration(void)//GPIO配置函数
{    
  GPIO_InitTypeDef GPIO_InitStructure;    
    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;    			//定义PA0
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
}  
  
void TIM3_Configuration(void)//TIM3配置函数
{  
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
  TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  
  TIM_TimeBaseStructure.TIM_Period = 82;						//频率:72MHz  per/2000Hz=72000000/36000  
  TIM_TimeBaseStructure.TIM_Prescaler = 35999;			//36000-1=35999  时钟频率=72/(时钟预分频+1)
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	//时钟分频
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  
     
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE );  
  TIM_Cmd(TIM3,ENABLE);

}  

 

/*******************************************************************************
* Function Name  : scan_files
* Description    : 搜索文件目录下所有文件
* Input          : - path: 根目录
* Output         : None
* Return         : FRESULT
* Attention		   : 不支持长文件名
*******************************************************************************/
FRESULT scan_files (char* path)
{
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;
#if _USE_LFN
    static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif

    res = f_opendir(&dir, path);
    if (res == FR_OK) {
        i = strlen(path);
        for (;;) {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) break;
            if (fno.fname[0] == '.') continue;
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (fno.fattrib & AM_DIR) {
                sprintf(&path[i], "/%s", fn);
                res = scan_files(path);
                if (res != FR_OK) break;
                path[i] = 0;
            } else {
                printf("%s/%s \r\n", path, fn);
            }
        }
    }

    return res;
}




/*******************************************************************************
* Function Name  : SD_TotalSize
* Description    : 文件空间占用情况
* Input          : None
* Output         : None
* Return         : 返回1成功 返回0失败
* Attention		   : None
*******************************************************************************/
int SD_TotalSize(void)
{
    FATFS *fs;
    DWORD fre_clust;        

    res = f_getfree("0:", &fre_clust, &fs);  /* 必须是根目录，选择磁盘0 */
	
    if ( res==FR_OK ) 
    {
	  /* Print free space in unit of MB (assuming 512 bytes/sector) */
 			
#if 0			//
			printf("\r\n%d MB total drive space.\r\n"
           "%d MB available.\r\n",
           ( (fs->n_fatent - 2) * fs->csize ) / 2 /1024 , (fre_clust * fs->csize) / 2 /1024 );     
#endif			
			
			printf("\r\n%d MB 总容量。\r\n"
						 "%d MB 可用空间。\r\n",
						(uint32_t)( (fs->n_fatent - 2) * fs->csize ) / 2 /1024 , (uint32_t)(fre_clust * fs->csize) / 2 /1024 );
		
			return ENABLE;
		}	
			else 
				return DISABLE;   
}	 



