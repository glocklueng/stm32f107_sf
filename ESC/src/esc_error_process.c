#include "esc_error_process.h"
#include "hw_test.h"
#include "sys.h"

int EscBuff[10] = {0};

/** �ڹ��ϴ����ӳ�������ѭ�����Ͽ���ȫ�̵���
����ֹͣ���У��������״̬���ȴ������˹���λ��**/
void ESC_EWDT_Error_Process(void)
{
#ifdef GEC_SF_MASTER  
    /** �Ͽ���ȫ�̵��� **/
    SF_RL1_CTR = 0;
    SF_RL1_WDT = 0;
#endif    
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
#ifdef GEC_SF_MASTER  
    /** �Ͽ���ȫ�̵��� **/
    SF_RL1_CTR = 0;
    SF_RL1_WDT = 0;    
#endif
}


/** ���ϴ����ӳ��򣬶Ͽ���ȫ�̵�����
����ֹͣ���У��������״̬, �ȴ��˹��������� **/
void ESC_Fram_Error_Process(void)
{
#ifdef GEC_SF_MASTER  
    /** �Ͽ���ȫ�̵��� **/
    SF_RL1_CTR = 0;
    SF_RL1_WDT = 0;
#endif    

}