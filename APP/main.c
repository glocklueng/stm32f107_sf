#include "sys.h"
#include "delay.h"    
#include "led.h"
#include "mb85rcxx.h"
#include "hw_test.h"
#include "can.h"

#include "usbd_cdc_core.h"
#include "usbd_usr.h"
#include "usb_conf.h"
#include "usbd_desc.h"


void can1_can2_test(void);


__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;

//send 
extern uint32_t APP_Rx_ptr_in;
extern uint8_t APP_Rx_Buffer   [APP_RX_DATA_SIZE];

//recvice
extern uint32_t USB_Recive_length; 
extern uint8_t USB_Rx_Buffer   [CDC_DATA_MAX_PACKET_SIZE];

extern uint32_t APP_Rx_ptr_out;
extern uint32_t APP_Rx_length;

extern u8 sflag,inputnum;
 
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
              ;
                   
        }  

#if 1        
        HW_TEST_INIT();
//        HW_TEST();

        
        AUX1_CTR = 0;
        SF_RL1_CTR = 0;
        SF_RL1_WDT = 1;
#endif


#if 1

	u8 a=0,t=0;
	u8 cnt=0;
	u8 canbuf_send[8],canbuf_recv[8];
	u8 res;
        u8 can_rcv;
	u8 mode=CAN_Mode_Normal;//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ

	 	
   
	CAN_Mode_Init(CAN1,CAN_SJW_2tq,CAN_BS2_5tq,CAN_BS1_3tq,20,mode);//CAN��ʼ������ģʽ,������200Kbps    
        CAN_Mode_Init(CAN2,CAN_SJW_2tq,CAN_BS2_5tq,CAN_BS1_3tq,20,mode);//CAN��ʼ������ģʽ,������200Kbps            

        
          USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS 
            USB_OTG_HS_CORE_ID,
#else            
            USB_OTG_FS_CORE_ID,
#endif  
            &USR_desc, 
            &USBD_CDC_cb, 
            &USR_cb);
          
          
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
                        
                          for(i = 0; i < USB_Recive_length; i++)
                          {
                              APP_Rx_Buffer[APP_Rx_ptr_in] = USB_Rx_Buffer[i];
                              APP_Rx_ptr_in++;
                              
                              if(APP_Rx_ptr_in >= APP_RX_DATA_SIZE - 50)
                              {
                                APP_Rx_ptr_in = 0;
                              }
                          }
                          USB_Recive_length = 0;
                        
                          LED=!LED;
                          SF_RL1_WDT=!SF_RL1_WDT;
                          tt=0;
                      } 
                      
                      // CAN ÿ�뷢��һ�Σ���ͨ��VCP���͸�����
                      if(aa == 1000)
                      {
#if 1                        
                          for(i=0;i<8;i++)
                          {
                            canbuf_send[i]= 'a' + i;//��䷢�ͻ�����
                            
//                            printf("%s",canbuf_send[i]);	//��ʾ����
                          }
                          res=Can_Send_Msg(CAN1,canbuf_send,8);//����8���ֽ� 
                          
                          if(res)
                          {
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = 'C';
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = 'A';
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = 'N';
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = 'T';
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = 'X';
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = ':';   
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = 'f';
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = 'a';
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = 'i';
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = 'l';                              
                              
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = '\r';
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = '\n';
                              
//                            printf("Failed");		//��ʾ����ʧ��
                          }
                          else 
                          {
//                            printf("OK    ");	 		//��ʾ���ͳɹ�	 
                            
                          
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = 'C';
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = 'A';
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = 'N';
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = 'T';
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = 'X';
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = ':';

                              for(i = 0; i < 8; i++)
                              {
                                  APP_Rx_Buffer[APP_Rx_ptr_in] = canbuf_send[i];
                                  APP_Rx_ptr_in++;
                              }  
                              
                              can_rcv=Can_Receive_Msg(CAN2,canbuf_recv);
                              if(can_rcv)//���յ�������
                              {			

                                APP_Rx_Buffer[APP_Rx_ptr_in++] = 'C';
                                APP_Rx_Buffer[APP_Rx_ptr_in++] = 'A';
                                APP_Rx_Buffer[APP_Rx_ptr_in++] = 'N';
                                APP_Rx_Buffer[APP_Rx_ptr_in++] = 'R';
                                APP_Rx_Buffer[APP_Rx_ptr_in++] = 'X';
                                APP_Rx_Buffer[APP_Rx_ptr_in++] = ':';   
                                
                                for(i=0;i<can_rcv;i++)
                                {									    
//                                    printf("%s",canbuf_recv[i]);	//��ʾ����
                                    APP_Rx_Buffer[APP_Rx_ptr_in] = canbuf_recv[i];
                                    APP_Rx_ptr_in++;
                                }
                              }                              
                              
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = '\r';
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = '\n';
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = '\r';
                              APP_Rx_Buffer[APP_Rx_ptr_in++] = '\n';                              
                              
                                if(APP_Rx_ptr_in >= APP_RX_DATA_SIZE - 50)
                                {
                                  APP_Rx_ptr_in = 0;
                                }
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