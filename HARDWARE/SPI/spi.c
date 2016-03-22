/*******************************************************************************
* File Name          : spi.c
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : 
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "spi.h"
#include "esc_error_process.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define SPI1_DR_Addr ( (u32)0x4001300C )

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

u8 SPI1_TX_Buff[buffersize] = { 0 };
u8 SPI1_RX_Buff[buffersize] = { 0 };
u8 SPI_DMA_RECEIVE_FLAG = 0;



/*******************************************************************************
* Function Name  : SPI1_Init
* Description    : 
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
        
        SPI_InitTypeDef  SPI_InitStructure;
  
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_SPI1, ENABLE );	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
                
 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
        
        SPI_I2S_DeInit(SPI1);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;      //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
#ifdef GEC_SF_MASTER
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                //����SPI����ģʽ:����Ϊ��SPI
#else
        SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
#endif
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		        //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		                //ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	                        //���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		                //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;	//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	                        //CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);                                     
 
        //DMA 
        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
        SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
      
//        SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE, ENABLE);
        
        //CRC
        SPI_CalculateCRC(SPI1, ENABLE);
        
	SPI_Cmd(SPI1, ENABLE); 
        
        #ifdef GEC_SF_MASTER
            //����ʱ�ӣ���ʼ����
//            SPI1_ReadWriteByte(0xff);        
        #endif
}    


/*******************************************************************************
* Function Name  : SPI1_DMA_Configuration
* Description    : ����SPI1_RX��DMAͨ��2��SPI1_TX��DMAͨ��3
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void SPI1_DMA_Configuration( void )
{  
      DMA_InitTypeDef     DMA_InitStructure;
      
      NVIC_InitTypeDef NVIC_InitStructure;
      
      RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
      
      DMA_DeInit(DMA1_Channel2);
      DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_DR_Addr;//���������ַ��ע��PSIZE
      DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SPI1_RX_Buff;//����DMA�洢����ַ��ע��MSIZE
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //Ŀ��
      DMA_InitStructure.DMA_BufferSize = buffersize; //������������Ϊbuffersize��
      
      DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//��ִ�������ַ����ģʽ
      DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢����ַ����ģʽ
      DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݿ��8bit
      DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //�洢�����ݿ��8bit
      DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //ִ��ѭ������
      DMA_InitStructure.DMA_Priority = DMA_Priority_High;//ͨ�����ȼ���
      DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //�Ǵ洢�����洢��ģʽ
      DMA_Init(DMA1_Channel2, &DMA_InitStructure);  //
      
      DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
      
      DMA_DeInit(DMA1_Channel3);
      DMA_InitStructure.DMA_PeripheralBaseAddr = SPI1_DR_Addr;
      DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SPI1_TX_Buff;
      DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; //Ŀ��
      DMA_InitStructure.DMA_BufferSize = buffersize; //���泤��
      
      DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//һ������
      DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�����ַ����
      DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�ֽڴ���
      DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
      DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
      DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
      DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //
      DMA_Init(DMA1_Channel3, &DMA_InitStructure);  //  
      
      DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);
      
      
      
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
      NVIC_Init(&NVIC_InitStructure);      
      
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
      NVIC_Init(&NVIC_InitStructure);   
  
  
#ifndef GEC_SF_MASTER   
//  DMA_Cmd(DMA1_Channel2, DISABLE);
//  DMA1_Channel2->CNDTR = 0x0000;	
//  DMA1_Channel2->CNDTR = 512;
//  DMA_ClearFlag(DMA1_FLAG_GL2|DMA1_FLAG_TC2|DMA1_FLAG_HT2|DMA1_FLAG_TE2);
//  SPI1->DR ;
//  DMA_Cmd(DMA1_Channel2, ENABLE);
//            while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//          DMA_Cmd(DMA1_Channel2, DISABLE);
//          DMA_Cmd(DMA1_Channel3, DISABLE);
//          SPI1_DMA_ReceiveSendByte(512);
#endif
}

/*******************************************************************************
* Function Name  : SPI1_DMA_ReceiveSendByte
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_DMA_ReceiveSendByte( u16 num )
{
  
      while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
      
      DMA_Cmd(DMA1_Channel2, DISABLE);
      DMA_Cmd(DMA1_Channel3, DISABLE);      
      
      DMA1_Channel2->CNDTR = 0x0000;	
      DMA1_Channel2->CNDTR = num;
      DMA1_Channel3->CNDTR = 0x0000;	
      DMA1_Channel3->CNDTR = num;      
      
      
      DMA_ClearFlag(DMA1_FLAG_GL3|DMA1_FLAG_TC3|DMA1_FLAG_HT3|DMA1_FLAG_TE3);
      DMA_ClearFlag(DMA1_FLAG_GL2|DMA1_FLAG_TC2|DMA1_FLAG_HT2|DMA1_FLAG_TE2);
      
      SPI1->DR ;						//����ǰ��һ��SPI1->DR����֤���ջ�����Ϊ��
      
      while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
      
      DMA_Cmd(DMA1_Channel3, ENABLE);
      DMA_Cmd(DMA1_Channel2, ENABLE);
      
//      while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//      
//      DMA_Cmd(DMA1_Channel3, DISABLE);
//      DMA_Cmd(DMA1_Channel2, DISABLE);
     
}

/*******************************************************************************
* Function Name  : DMA1_Channel2_IRQHandler
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel2_IRQHandler(void)
{

      if ( ( DMA_GetITStatus( DMA1_IT_TC2 ) ) != RESET )
      {
          DMA_ClearFlag(DMA1_FLAG_GL2|DMA1_FLAG_TC2|DMA1_FLAG_HT2|DMA1_FLAG_TE2);
          
          if( SPI_I2S_GetFlagStatus(SPI1, SPI_FLAG_CRCERR) != RESET)
          {
              SPI_I2S_ClearFlag(SPI1, SPI_FLAG_CRCERR);
              //SPI CRC ERROR
              EN_ERROR_SYS4++;
              if(EN_ERROR_SYS4 > 3)
              {
                ESC_SPI_Error_Process();
              }
          }
          else
          {
              EN_ERROR_SYS4 = 0;
          }
      #ifdef GEC_SF_MASTER
          while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
          DMA_Cmd(DMA1_Channel2, DISABLE);
          DMA_Cmd(DMA1_Channel3, DISABLE);
          
      #else

//          while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//          DMA_Cmd(DMA1_Channel2, DISABLE);
//          DMA_Cmd(DMA1_Channel3, DISABLE);         
//          
//          
//          SPI1_DMA_ReceiveSendByte(512);
          
      #endif
          
          SPI_DMA_RECEIVE_FLAG = 1;
      }

}
/*******************************************************************************
* Function Name  : DMA1_Channel3_IRQHandler
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel3_IRQHandler(void)
{
      if ( ( DMA_GetITStatus( DMA1_IT_TC3 ) ) != RESET )
      {
        DMA_ClearFlag(DMA1_FLAG_GL3|DMA1_FLAG_TC3|DMA1_FLAG_HT3|DMA1_FLAG_TE3);

//          while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
  
//          DMA_Cmd(DMA1_Channel3, DISABLE);
//          DMA_Cmd(DMA1_Channel2, DISABLE);   
          
          if( SPI_I2S_GetFlagStatus(SPI1, SPI_FLAG_CRCERR) != RESET)
          {
              SPI_I2S_ClearFlag(SPI1, SPI_FLAG_CRCERR);

          }
          else
          {

          }
          
          

      }

}



/*******************************************************************************/
/*******************************************************************************/
/*******************************************************************************/


/** SPI SpeedSet:
SPI_BaudRatePrescaler_2   2 division   (SPI 36M@sys 72M)
SPI_BaudRatePrescaler_8   8 division   (SPI 9M@sys 72M)
SPI_BaudRatePrescaler_16  16 division  (SPI 4.5M@sys 72M)
SPI_BaudRatePrescaler_256 256 division (SPI 281.25K@sys 72M)
**/  
void SPI1_SetSpeed(u8 SpeedSet)
{
        SPI_InitTypeDef  SPI_InitStructure;
  
	SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
  	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
} 

/** SPIx read and write a byte
TxData:Bytes to be written
return :Read bytes
**/
u8 SPI1_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)          //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData);                                         //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)         //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1);                                       //����ͨ��SPIx������յ�����					    
}


void SPI1_WriteByte(u8 TxData)
{  
     u8 retry=0;     
     while((SPI1->SR&1<<1)==0) 
     {
      retry++;
      if(retry>200)return;
     }     
     SPI1->DR=TxData;            
}


u8 SPI1_ReadByte(u8 TxData)
{  
     u8 retry=0;     

     while((SPI1->SR&1<<0)==0)  
     {
      retry++;
      if(retry>200)return 0;
     }             
     return SPI1->DR;                
}

extern u8 Master_Temp[10];

u8 Recive_buf[20] = {0x00};
u8 cnt,flag1,recv_len= 0;

void SPI1_IRQHandler(void)
{
  
      if (SPI_I2S_GetITStatus(SPI1,SPI_I2S_IT_RXNE) != RESET)     
      {
          SPI_I2S_ClearITPendingBit(SPI1,SPI_I2S_IT_RXNE);
          
                while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
      

#ifdef GEC_SF_MASTER

#else

#endif
                
      }  
}

void SPI1_NVIC(void)
{
  
      NVIC_InitTypeDef NVIC_InitStructure;
      
      NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
      NVIC_Init(&NVIC_InitStructure);	
           
}






/******************************  END OF FILE  *********************************/
















