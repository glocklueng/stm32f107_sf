#include "sys.h"


void NVIC_Configuration(void)
{
  
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	
    
    INTX_ENABLE();

}




u32 GetLockCode(void)
{
  
   u32 CpuID[3];
   u32 Lock_Code;  
  
  /** acquire the cpu unique ID **/
  CpuID[0]=*(vu32*)(0x1ffff7e8);
  CpuID[1]=*(vu32*)(0x1ffff7ec);
  CpuID[2]=*(vu32*)(0x1ffff7f0);
  
  /** encryption **/
  Lock_Code=(CpuID[0]>>1)+(CpuID[1]>>2)+(CpuID[2]>>3);
   
  return Lock_Code;
}


//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM("WFI");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{
	__ASM("CPSID I");    		  
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM("CPSIE I");		  
}
//����ջ����ַ
//addr:ջ����ַ
void MSR_MSP(u32 addr) 
{
    __ASM("MSR MSP, r0"); 			//set Main Stack value
    __ASM("BX r14");
}

