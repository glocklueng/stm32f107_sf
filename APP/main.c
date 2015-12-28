#include "sys.h"
#include "delay.h"   
#include "usart.h"
#include "led.h"
#include "malloc.h" 
#include "usbd_msc_core.h"
#include "usbd_usr.h"
#include "usbd_desc.h"
#include "usb_conf.h" 
#include "mb85rcxx.h"
#include "ewdt.h"

 

USB_OTG_CORE_HANDLE USB_OTG_dev;
extern vu8 USB_STATUS_REG;		//USB״̬
extern vu8 bDeviceState;		//USB���� ���


void usb_device_test(void)
{
  	u8 offline_cnt=0;
	u8 tct=0;
	u8 USB_STA;
	u8 Divece_STA;
  
        //USB Device��Դʹ��
        GPIO_ResetBits(GPIOA,GPIO_Pin_10);

 
 	printf("USB Connecting...\n");//��ʾ���ڽ������� 	    
	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
	delay_ms(1800);	
	while(1)
	{	
		delay_ms(1);				  
		if(USB_STA!=USB_STATUS_REG)//״̬�ı��� 
		{	 						   		  	   
			if(USB_STATUS_REG&0x01)//����д		  
			{
				LED=0;
				printf("USB Writing...\n");//��ʾUSB����д������	 
			}
			if(USB_STATUS_REG&0x02)//���ڶ�
			{
				LED=0;
				printf("USB Reading...\n");//��ʾUSB���ڶ�������  		 
			}	 										  
			if(USB_STATUS_REG&0x04)
                          printf("USB Write Err \n");//��ʾд�����
	  
			if(USB_STATUS_REG&0x08)
                          printf("USB Read  Err \n");//��ʾ��������
  
			USB_STA=USB_STATUS_REG;//��¼����״̬
		}
		if(Divece_STA!=bDeviceState) 
		{
			if(bDeviceState==1)
                          printf("USB Connected    \n");//��ʾUSB�����Ѿ�����
			else 
                          printf("USB DisConnected \n");//��ʾUSB���γ���
                        
			Divece_STA=bDeviceState;
		}
		tct++;
		if(tct==200)
		{
			tct=0;
			LED=1;
			LED=!LED;//��ʾϵͳ������
                        
//                        EWDT_TOOGLE();
                        
			if(USB_STATUS_REG&0x10)
			{
				offline_cnt=0;//USB������,�����offline������
				bDeviceState=1;
			}else//û�еõ���ѯ 
			{
				offline_cnt++;  
				if(offline_cnt>10)bDeviceState=0;//2s��û�յ����߱��,����USB���γ���
			}
			USB_STATUS_REG=0;
		}
	};  
  

}


int main(void)
{        

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();  //��ʼ����ʱ����
//	uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
	LED_Init();					//��ʼ��LED  
        
        //�ⲿ���Ź���ʼ��
//        EWDT_Drv_pin_config();
//        power_on_bsp_check();        
  
        //MB85RCXX��ʼ��
        eep_init();
        if(MB85RCXX_Check())
        {
            printf("MB85RCXX_Checkʧ��\n");
          
        }  
        
        usb_device_test();
}




