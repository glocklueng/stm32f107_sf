#include "sys.h"
#include "delay.h"    
#include "led.h"
#include "mb85rcxx.h"
#include "hw_test.h"
 
 
int main(void)
{        
  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();  //��ʼ����ʱ����
	LED_Init();					//��ʼ��LED  


  
        //MB85RCXX��ʼ��
        eep_init();
        if(MB85RCXX_Check())
        {
              ;
                   
        }  
        
        HW_TEST_INIT();
        HW_TEST();

}




