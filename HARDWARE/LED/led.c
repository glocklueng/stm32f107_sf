/*******************************************************************************
* File Name          : led.c
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : 
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "led.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/*******************************************************************************
* Function Name  : LED_Init
* Description    : 
*                  
* Input          : None
*                  None
* Output         : None
* Return         : None
*******************************************************************************/
void LED_Init(void)
{

#ifdef GEC_SF_MASTER
      GPIO_InitTypeDef  GPIO_InitStructure;
      
      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
      
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				
      GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
      GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
      GPIO_Init(GPIOA, &GPIO_InitStructure);					 
      GPIO_SetBits(GPIOA,GPIO_Pin_2);	

#else      
       GPIO_InitTypeDef  GPIO_InitStructure;
 	
       RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 
       
       GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 
       GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
       GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
       GPIO_Init(GPIOA, &GPIO_InitStructure);					 
       GPIO_SetBits(GPIOA,GPIO_Pin_1);						 
#endif 
}

/******************************  END OF FILE  *********************************/



