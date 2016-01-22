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
#include "spi.h"
#include "can.h"
 

USB_OTG_CORE_HANDLE USB_OTG_dev;
extern vu8 USB_STATUS_REG;		//USB״̬
extern vu8 bDeviceState;		//USB���� ���


void usb_device_test(void)
{
  	u8 offline_cnt=0;
	u8 tct=0;
	u8 USB_STA;
	u8 Divece_STA;
 	        
 
 	printf("USB Connecting...\n");//��ʾ���ڽ������� 	    
	USBD_Init(&USB_OTG_dev,USB_OTG_FS_CORE_ID,&USR_desc,&USBD_MSC_cb,&USR_cb);
        

//        delay_ms(200);
// 	GPIO_SetBits(GPIOA,GPIO_Pin_10);  	//USB�ȶϿ�
//        
//	delay_ms(300);
//   	GPIO_ResetBits(GPIOA,GPIO_Pin_10);	//USB�ٴ�����

        
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
//			LED=1;
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

void spi1_test(void)
{
    u8 Master_Temp =0;
    
    SPI1_Init();
    SPI1_SetSpeed(SPI_BaudRatePrescaler_256);

   while(1)
   { 
#if 0
       SPI1_ReadWriteByte(0x55); 
       Master_Temp = SPI1_ReadWriteByte(0x00);
#else
       SPI1_WriteByte(0x55); 
       Master_Temp = SPI1_ReadByte(0x00);
#endif
       printf("Master_Temp =%x\r\n",Master_Temp);
       delay_ms(100); 
   }

}


void can_test(void)
 {	 
	u8 i=0,t=0;
	u8 cnt=0;
	u8 canbuf[8];
	u8 res;
        u8 can_rcv;
	u8 mode=CAN_Mode_LoopBack;//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ

	 	
   
	CAN_Mode_Init(CAN1,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);//CAN��ʼ������ģʽ,������500Kbps    

	
 	while(1)
	{

		if(mode==CAN_Mode_LoopBack)//KEY0����,����һ������
		{
			for(i=0;i<8;i++)
			{
				canbuf[i]=cnt+i;//��䷢�ͻ�����
				if(i<4)printf("%s",canbuf[i]);	//��ʾ����
				else printf("%s",canbuf[i]);	//��ʾ����
 			}
			res=Can_Send_Msg(CAN1,canbuf,8);//����8���ֽ� 
			if(res)printf("Failed");		//��ʾ����ʧ��
			else printf("OK    ");	 		//��ʾ���ͳɹ�								   
		}else if(mode==CAN_Mode_Normal)//WK_UP���£��ı�CAN�Ĺ���ģʽ
		{	   
//			mode=!mode;
  			CAN_Mode_Init(CAN1,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_Normal);//CAN��ͨģʽ��ʼ��, ������500Kbps 

			if(mode==0)//��ͨģʽ����Ҫ2��������
			{
				printf("Nnormal Mode ");	    
			}else //�ػ�ģʽ,һ��������Ϳ��Բ�����.
			{
 				printf("LoopBack Mode");
			}

		}		 
		can_rcv=Can_Receive_Msg(CAN1,canbuf);
		if(can_rcv)//���յ�������
		{			
			
 			for(i=0;i<can_rcv;i++)
			{									    
				if(i<4)printf("%s",canbuf[i]);	//��ʾ����
				else printf("%s",canbuf[i]);	//��ʾ����
 			}
		}
		t++; 
		delay_ms(10);
		if(t==20)
		{
			LED=!LED;//��ʾϵͳ��������	
			t=0;
			cnt++;
			printf("%d",cnt);	//��ʾ����
		}		   
	}
}

int main(void)
{        
        //����ϵͳ�ж����ȼ�����2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        //��ʼ����ʱ����
	delay_init();  
        
        //��ʼ�����ڲ�����Ϊ115200
	uart_init(115200);		
        
        //��ʼ��LED  
	LED_Init();					
        
        //�ⲿ���Ź���ʼ��
//        EWDT_Drv_pin_config();
//        power_on_bsp_check();        
  
        //MB85RCXX��ʼ��
//        eep_init();
//        if(MB85RCXX_Check())
//        {
//            printf("MB85RCXX_Checkʧ��\n");
//          
//        }  
        
#if 0
        //SPI˫��ͨ�Ų���
        spi1_test();
#else
  #if 1
        //USB Device����
        usb_device_test();
  #else
        //cat test
        can_test();
  #endif
#endif
        
}




