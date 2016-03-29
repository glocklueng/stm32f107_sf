/**
 * Company	: ThyssenKrupp PDC
 * @file	: config_test.c
 * @purpose     :
 * @brief	: c based Configuration Register test POST configure test 
 *                  IEC61508 verification on ETC-01 mainboard.
 * @version	: 1.0
 * @date	: 12-May-2014
 * @author	: Paul
*/

#include "stm32f10x.h"
#include "config_test.h"


GPIO_InitTypeDef   GPIO_InitStructure;
ErrorStatus HSEStartUpStatus;


/**
  * @brief  Configures the different system clocks.PLL (8MHz * 9=72MHz)
  * @param  None
  * @retval None
  */
void RCC_Configuration_72M(void) 
{ 

  /* ������������RCC�����ã����ƼĴ�����λ */ 
  RCC_DeInit(); 

  /* ʹ���ⲿ���پ��� */ 
  RCC_HSEConfig(RCC_HSE_ON); 

  /* �ȴ����پ����ȶ� */ 
  HSEStartUpStatus = RCC_WaitForHSEStartUp(); 

  if (HSEStartUpStatus == SUCCESS) 
  { 
     /* ʹ��flashԤ��ȡ������ */ 
     FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); 

     /* ��Flash���ڵȴ�״̬��2����Ը�Ƶʱ�ӵģ��������RCCûֱ�ӹ�ϵ�����������Թ� */ 
     FLASH_SetLatency(FLASH_Latency_2); 

     /* HCLK = SYSCLK ���ø�������ʱ��=ϵͳʱ��*/ 
     RCC_HCLKConfig(RCC_SYSCLK_Div1); 

     /* PCLK2 = HCLK ���õ�������2ʱ��=��������ʱ��*/ 
     RCC_PCLK2Config(RCC_HCLK_Div1); 

     /* PCLK1 = HCLK/2 ���õ�������1��ʱ��=����ʱ�ӵĶ���Ƶ*/ 
     RCC_PCLK1Config(RCC_HCLK_Div2); 

     /* ADCCLK = PCLK2/6 ����ADC����ʱ��=��������2ʱ�ӵ�����Ƶ*/ 
     RCC_ADCCLKConfig(RCC_PCLK2_Div6); 

     /* Set PLL clock output to 72MHz using HSE (8MHz) as entry clock */ 
     //�������������ȱʧ�ˣ���ȴ�ܹؼ� 
     /* �������໷���ⲿ8Mhz����9��Ƶ��72Mhz */ 
#ifdef GEC_SF_MASTER
    RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_9);
#else
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
#endif 

     /* Enable PLL ʹ�����໷*/ 
     RCC_PLLCmd(ENABLE); 

     /* Wait till PLL is ready �ȴ����໷����ȶ�*/ 
     while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) 
     {} 

     /* Select PLL as system clock source �����໷�������Ϊϵͳʱ�� */ 
     RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 

     /* Wait till PLL is used as system clock source �ȴ�У��ɹ�*/ 
     while (RCC_GetSYSCLKSource() != 0x08) 
     {} 
} 

  RCC_ClockSecuritySystemCmd(ENABLE);//ʹ�ܻ���ʧ��ʱ�Ӱ�ȫϵͳ
}

/**
  * @brief  Configures the different system clocks.HSE(8MHz)
  * @param  None
  * @retval None
  */
void RCC_Configuration_8M(void)
{
//  SystemInit();//Ƶ���趨��system_stm32f10x.c�ļ��еĺ궨�� ����ĳ���� ������SystemInit()ʱ�������ú�Ƶ��
 
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    //FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 0 wait state */
    //FLASH_SetLatency(FLASH_Latency_0);

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK */
    RCC_PCLK1Config(RCC_HCLK_Div1);

    /* Select HSE as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSE);

    /* Wait till HSE is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x04)
    {}
  }

  RCC_ClockSecuritySystemCmd(ENABLE);//ʹ�ܻ���ʧ��ʱ�Ӱ�ȫϵͳ
}

