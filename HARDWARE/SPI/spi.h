#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

 
 				  	    													  
void SPI1_Init(void);			 //��ʼ��SPI��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI���߶�дһ���ֽ�

void SPI1_WriteByte(u8 TxData);
u8 SPI1_ReadByte(u8 TxData);
void SPI1_NVIC(void);


void SPI1_Configuration( void );
void SPI1_DMA_Configuration( void );
void SPI1_ReceiveSendByte( u16 num );

#define buffersize 512

#define SPI1_DR_Addr ( (u32)0x4001300C )

#endif

