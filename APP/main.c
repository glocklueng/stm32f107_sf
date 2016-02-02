#include "sys.h"
#include "delay.h"    
#include "led.h"
#include "mb85rcxx.h"
#include "hw_test.h"
#include "can.h"
#include "spi.h"

#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"
#include "usbd_cdc_vcp.h"


void can1_can2_test(void);


__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;

extern u8 sflag,inputnum;

u8 Master_Temp =0;

void spi1_test(void)
{
    
    u8 t;
    
    SPI1_Init();
//    SPI1_NVIC();
//    SPI1_SetSpeed(SPI_BaudRatePrescaler_256);

   while(1)
   { 
     
//     HW_TEST();
#if 0
       SPI1_ReadWriteByte(0x55); 
       Master_Temp = SPI1_ReadWriteByte(0x00);
#else
       SPI1_WriteByte(0x66); 
//       delay_ms(1);
//       Master_Temp = SPI1_ReadByte(0x00);
#endif
       //       printf("Master_Temp =%x\r\n",Master_Temp);
       

       delay_ms(10); 
       
       t++;
       if(t == 50)
       {
             t = 0;
             LED =! LED;
             
            Usb_Vcp_SendBuf(&Master_Temp, 1);                  
         
       }
   }

}





u8 canbuf_send[8];

int main(void)
{        
  u8 tt,i = 0;
  u32 aa = 0;
  
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();  //��ʼ����ʱ����
	LED_Init();					//��ʼ��LED  


  
        //MB85RCXX��ʼ��
        eep_init();
        if(MB85RCXX_Check())
        {
              delay_ms(1);
                   
        }  

#if 1        
        HW_TEST_INIT();
//        HW_TEST();

        
        AUX1_CTR = 0;
        SF_RL1_CTR = 0;
        SF_RL1_WDT = 1;
#endif
        

#if 1

	u8 a='a',t=0;
	u8 cnt=0;
	u8 canbuf_recv[8];
	u8 res;
        u8 can_rcv;
	u8 mode=CAN_Mode_Normal;//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ
          
	 	
   
	CAN_Mode_Init(CAN1,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,8,mode);//CAN��ʼ������ģʽ,������250Kbps    
//        CAN_Mode_Init(CAN2,CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,8,mode);//CAN��ʼ������ģʽ,������250Kbps            

        
          USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS 
            USB_OTG_HS_CORE_ID,
#else            
            USB_OTG_FS_CORE_ID,
#endif  
            &USR_desc, 
            &USBD_CDC_cb, 
            &USR_cb);
          
          
//          spi1_test();
          
          while(1)
          {
                    delay_ms(1);
                      tt++;
                      aa++;
                      
                      //VCP�ѽ��յ������ݷ��ظ�����
                      if(tt==200)
                      {
//                          for(i = 0; i < 8; i++)
//                          {
//                              APP_Rx_Buffer[APP_Rx_ptr_in] = i + '0';
//                              APP_Rx_ptr_in++;
//                          }      
                        
                          HW_TEST();
                                                 
                          Usb_Vcp_RecvBufandSend();
                        
                          LED=!LED;
                          SF_RL1_WDT=!SF_RL1_WDT;
                          tt=0;
                      } 
                      
                      // CAN ÿ�뷢��һ�Σ���ͨ��VCP���͸�����
                      if(aa == 1000)
                      {
#if 1                     

//                          for(i=0;i<8;i++)
//                          {
//                            canbuf_send[i]= a;//��䷢�ͻ�����
//                            
////                            printf("%s",canbuf_send[i]);	//��ʾ����
//                          }
                          
//                          a++;
//                          if(a > 'z')
//                          {
//                              a = 'a';
//                          }
                          res=Can_Send_Msg(CAN1,canbuf_send,4);//����8���ֽ� 
                          
                          if(res)
                          {
                             
                              Usb_Vcp_SendBuf("CAN1TX:fail\r\n", 13);
                              
//                            printf("Failed");		//��ʾ����ʧ��
                          }
                          else 
                          {
//                            printf("OK    ");	 		//��ʾ���ͳɹ�	 
                            
                                                        
                              Usb_Vcp_SendBuf("CAN1TX:", 7);
                              
                              Usb_Vcp_SendBuf(canbuf_send, 4); 
                              
                              delay_ms(1);
                              
//                              can_rcv=Can_Receive_Msg(CAN2,canbuf_recv);
                              can_rcv=Can_Receive_Msg(CAN1,canbuf_recv);
                              if(can_rcv)//���յ�������
                              {			                                 
                                  Usb_Vcp_SendBuf("CAN1RX:", 7);
                                  
                                  Usb_Vcp_SendBuf(canbuf_recv, can_rcv);                                                                                               
                              }                              
                              
                              Usb_Vcp_SendBuf("\r\n\r\n", 4);                             
                              
//                                if(APP_Rx_ptr_in >= APP_RX_DATA_SIZE - 50)
//                                {
//                                  APP_Rx_ptr_in = 0;
//                                }
                          }
#endif                      
                      aa = 0;
                      
                      }
          
          }

          
#endif          
          
}



void can1_can2_test(void)
 {	 
	u8 i=0,t=0;
	u8 cnt=0;
	u8 canbuf_send[8],canbuf_recv[8];
	u8 res;
        u8 can_rcv;
	u8 mode=CAN_Mode_Normal;//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ

	 	
   
	CAN_Mode_Init(CAN1,CAN_SJW_2tq,CAN_BS2_5tq,CAN_BS1_3tq,20,mode);//CAN��ʼ������ģʽ,������200Kbps    
        CAN_Mode_Init(CAN2,CAN_SJW_2tq,CAN_BS2_5tq,CAN_BS1_3tq,20,mode);//CAN��ʼ������ģʽ,������200Kbps    

	
 	while(1)
	{
                //CAN1����
                
                for(i=0;i<8;i++)
                {
                  canbuf_send[i]=cnt+i;//��䷢�ͻ�����
                  
                  printf("%s",canbuf_send[i]);	//��ʾ����
                }
                res=Can_Send_Msg(CAN1,canbuf_send,8);//����8���ֽ� 
                
                if(res)
                  printf("Failed");		//��ʾ����ʧ��
                else 
                  printf("OK    ");	 		//��ʾ���ͳɹ�								   


                //CAN2����  
		can_rcv=Can_Receive_Msg(CAN2,canbuf_recv);
		if(can_rcv)//���յ�������
		{			
			
 			for(i=0;i<can_rcv;i++)
			{									    
                              printf("%s",canbuf_recv[i]);	//��ʾ����
 			}
		}                               
		   
	}

}