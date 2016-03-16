#include "initial_devices.h"
#include "esc_error_process.h"

#ifdef GEC_SF_MASTER

u32 TimingDelay = 0;

u8 canbuf_recv[8];
u8 res;
u8 can_rcv;

#else

#endif
 


void LED_indicator(void)
{
	static u32 led_idr_cnt=0;	 
	
	led_idr_cnt++;
	
	if(led_idr_cnt >= 300 && data_error <= 0)
	{
                led_idr_cnt = 0;
		LED=!LED;                
	}   
}


void Task_Loop(void)
{ 
  
      static u16 Tms25Counter=0,Tms50Counter=0,Tms100Counter=0,Tms500Counter=0,Tms1000Counter=0;
      static u32 comm_timeout = 0;
      
      delay_ms(1);
      Tms25Counter++;
      Tms50Counter++;
      Tms100Counter++;
      Tms500Counter++; 
      Tms1000Counter++;   

      if(Tms25Counter>=25) Tms25Counter=0;
      if(Tms50Counter>=50) Tms50Counter=0;
      if(Tms100Counter>=100) Tms100Counter=0;
      if(Tms500Counter>=500) Tms500Counter=0;
      if(Tms1000Counter>=1000) Tms1000Counter=0;      
  
#ifdef GEC_SF_MASTER  
  
      
//      if(count3 == 25)
//      {
//          PLUSE_OUT = 1;
//      }
//      
//      if(count3 == 44)
//      {
//          PLUSE_OUT = 0;
//          count3 = 0;
//      }      

//      if( onetime == 0)
//      {
//          onetime++;
//        
//          if(SF_RL1_FB)
//            SPI1_TX_Buff[0] = 0x01;
//          else
//            SPI1_TX_Buff[0] = 0x00;
//                           
//          SPI1_DMA_ReceiveSendByte(num);
//      }
      if(Tms25Counter == 0)      
      {
          CPU_Exchange_Data_Check();
      }
      
      if(Tms50Counter == 0)
      {                       
          Input_Check();                   
          
          USB_VCP_RecvBufandSend();
          
          SF_EWDT_TOOGLE();
          EWDT_TOOGLE();
          
      } 
      
      if(Tms100Counter == 0)
      {         
          SF_CTR_Check();
      }
      
      
      if(Tms1000Counter == 0)
      {                  
          /** CAN1 send data **/
          res=Can_Send_Msg(CAN1,canbuf_send,4);                          
          if(res)
          {                             
              USB_VCP_SendBuf("CAN1TX:fail\r\n", 13);                             
          }
          else 
          {	 
                                       
              USB_VCP_SendBuf(canbuf_send, 4); 
              
              delay_ms(1);
              
              /** CAN1 receive data **/
              can_rcv=Can_Receive_Msg(CAN1,canbuf_recv);
              if(can_rcv)
              {			                                                                 
                  USB_VCP_SendBuf(canbuf_recv, can_rcv);                                                                                               
              }                                                                       
            
          }                      
        
      }
      
      if(Tms500Counter == 0)
      {
          Comm_DisplayBoard();      
      }
      
#else

      
      if( onetime == 0)
      {
          onetime++;
                          
          SPI1_DMA_ReceiveSendByte(512);
      }
      if( Tms25Counter == 0 )
      {
          comm_timeout++;
          if(comm_timeout > 100)
          {
              ESC_SPI_Error_Process();
          }
          if( SPI_DMA_RECEIVE_FLAG == 1 )
          {
                comm_timeout = 0;
                
                CPU_Exchange_Data_Check();              
          }
      }
      
      if( Tms50Counter == 0 )
      {                       
          Input_Check();   
                              
          SF_EWDT_TOOGLE();
          EWDT_TOOGLE();
      } 
      
      if( Tms100Counter == 0 )
      {         
          SF_CTR_Check();
      }      
      
#endif
}


int main(void)
{        
    
    /** hardware init **/
    Bsp_Init();

    while(1)
    {
//      spi1_test();
        Task_Loop();
        LED_indicator();
           
    }          
          
}
