#include "usb_bsp.h" 
#include "sys.h"  
#include "delay.h" 
//#include "usart.h" 
   

//USB主机电源控制口
//#define USB_HOST_PWRCTRL 	PAout(15)	//PA15
   
//USB OTG 底层IO初始化
//pdev:USB OTG内核结构体指针
void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
  RCC_OTGFSCLKConfig(RCC_OTGFSCLKSource_PLLVCO_Div3);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_OTG_FS, ENABLE) ;
  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
  
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
    GPIO_Init(GPIOA, &GPIO_InitStructure);	
  
    GPIO_SetBits(GPIOA,GPIO_Pin_10);  
  
}
//USB OTG 中断设置,开启USB FS中断
//pdev:USB OTG内核结构体指针
void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = OTG_FS_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;//子优先级3
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能通道
  NVIC_Init(&NVIC_InitStructure);//配置  
}
//USB OTG 中断设置,开启USB FS中断
//pdev:USB OTG内核结构体指针
void USB_OTG_BSP_DisableInterrupt(void)
{ 
}
//USB OTG 端口供电设置(本例程未用到)
//pdev:USB OTG内核结构体指针
//state:0,断电;1,上电
void USB_OTG_BSP_DriveVBUS(USB_OTG_CORE_HANDLE *pdev, uint8_t state)
{ 
}
//USB_OTG 端口供电IO配置(本例程未用到)
//pdev:USB OTG内核结构体指针
void  USB_OTG_BSP_ConfigVBUS(USB_OTG_CORE_HANDLE *pdev)
{   
} 
//USB_OTG us级延时函数
//本例程采用SYSTEM文件夹的delay.c里面的delay_us函数实现
//官方例程采用的是定时器2来实现的.
//usec:要延时的us数.
void USB_OTG_BSP_uDelay (const uint32_t usec)
{ 
   	delay_us(usec);
}
//USB_OTG ms级延时函数
//本例程采用SYSTEM文件夹的delay.c里面的delay_ms函数实现
//官方例程采用的是定时器2来实现的.
//msec:要延时的ms数.
void USB_OTG_BSP_mDelay (const uint32_t msec)
{  
	delay_ms(msec);
}
   




