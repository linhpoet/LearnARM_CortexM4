/*
 *
 *
 *  Created on: Jan 23, 2022
 *  Author: Linh
 */

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

#include<stdint.h>
#include<stdio.h>

void UsageFault_Handler_c(uint32_t *pMSP);

int main(void)
{
	//1. enable all configurable exceptions : usage fault, mem manage fault and bus fault

	uint32_t *pSHCSR = (uint32_t*)0xE000ED24;

	*pSHCSR |= ( 1 << 16); //mem manage
	*pSHCSR |= ( 1 << 17); //bus fault
	*pSHCSR |= ( 1 << 18); //usage fault


	//3. lets force the processor to execute some undefined instruction
	uint32_t *pSRAM = (uint32_t*)0x20010000;

	/*This is an undefined instruction value */
	*pSRAM = 0xFFFFFFFF;

   /* This is a function pointer variable */
	void (*some_address) (void);

   /* initialize  the function pointer variable to some address */
	some_address = (void*)0x20010001;

   /* change PC to jump to location 0x20010000 */
	some_address();

	//4. analyze the fault

	for(;;);
}

//2. implement the fault handlers
void HardFault_Handler(void)
{
	printf("Exception : Hardfault\n");
	while(1);
}


void MemManage_Handler(void)
{
	printf("Exception : MemManage\n");
	while(1);
}

void BusFault_Handler(void)
{
	printf("Exception : BusFault\n");
	while(1);
}


__attribute__ ((naked)) void UsageFault_Handler(void)
{
	//here we extracted the value of MSP which happens to be the
	//base address of the stack frame(thread mode) which got saved during the exception entry
	//from thread mode to handler mode
	__asm ("MRS r0,MSP");
	__asm ("B UsageFault_Handler_c");
}


/*
 * *pBaseStackFrame nhận giá trị ở r0, mà r0 = MSP ở trên
 */
void UsageFault_Handler_c(uint32_t *pBaseStackFrame)
{
	uint32_t *pUFSR = (uint32_t*)0xE000ED2A;
	printf("Exception : UsageFault\n");
	printf("UFSR = %lx\n",(*pUFSR) & 0xFFFF);
	printf("pBaseStackFrame = %p\n",pBaseStackFrame);
	printf("Value of R0 = %lx\n", pBaseStackFrame[0]);
	printf("Value of R1 = %lx\n", pBaseStackFrame[1]);
	printf("Value of R2 = %lx\n", pBaseStackFrame[2]);
	printf("Value of R3 = %lx\n", pBaseStackFrame[3]);
	printf("Value of R12 = %lx\n", pBaseStackFrame[4]);
	printf("Value of LR = %lx\n", pBaseStackFrame[5]);
	printf("Value of PC = %lx\n", pBaseStackFrame[6]);
	printf("Value of XPSR = %lx\n", pBaseStackFrame[7]);
	while(1);
}

