/*******************************************************************************
* File Name          : esc_error_process.c
* Author             : lison
* Version            : V1.0
* Date               : 03/22/2016
* Description        : 
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "esc_error_process.h"
#include "hw_test.h"
#include "sys.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


int EscBuff[10] = {0};



/*******************************************************************************
* Function Name  : ESC_EWDT_Error_Process
* Description    : 
*                  
* Input          : 
*                 
* Output         : None
* Return         : None
*******************************************************************************/
/** �ڹ��ϴ����ӳ�������ѭ�����Ͽ���ȫ�̵���
����ֹͣ���У��������״̬���ȴ������˹���λ��**/
void ESC_EWDT_Error_Process(void)
{
 
    /** �Ͽ���ȫ�̵��� **/
    SF_RL_CTR = 0;
    SF_RL_WDT = 0;
   
    while(1)
    {
        /** �ڹ��ϴ����ӳ�������ѭ�� **/
          /**  �ȴ������˹���λ **/
          if( IN10 )
          {
                __set_FAULTMASK(1);   
                /** ��λ **/
                NVIC_SystemReset();
          
          }
    }

}

/** �ڹ��ϴ����ӳ�������ѭ�����ȴ����Ź���λ
�Ͽ���ȫ�̵���������ֹͣ���У��������״̬���ȴ����ϸ�λ **/
void ESC_Flash_Error_Process(void)
{
 
    /** �Ͽ���ȫ�̵��� **/
    SF_RL_CTR = 0;
    SF_RL_WDT = 0;  

}


/** ���ϴ����ӳ��򣬶Ͽ���ȫ�̵�����
����ֹͣ���У��������״̬, �ȴ��˹��������� **/
void ESC_Fram_Error_Process(void)
{
#ifdef GEC_SF_MASTER  
    /** �Ͽ���ȫ�̵��� **/
    SF_RL_CTR = 0;
    SF_RL_WDT = 0;    
#endif    

}

/** ������ϴ����ӳ��򣬶Ͽ���ȫ�̵�����
����ֹͣ���У��������״̬ **/
void ESC_SPI_Error_Process(void)
{
  
    /** �Ͽ���ȫ�̵��� **/
    SF_RL_CTR = 0;
    SF_RL_WDT = 0;
    printf("ESC_SPI_Error_Process \r\n");
}


/** ������ϴ����ӳ��򣬶Ͽ���ȫ�̵�����
����ֹͣ���У��������״̬ **/
void ESC_SafeRelay_Error_Process(void)
{
 
    /** �Ͽ���ȫ�̵��� **/
    SF_RL_CTR = 0;
    SF_RL_WDT = 0;
    printf("ESC_SafeRelay_Error_Process \r\n");
}