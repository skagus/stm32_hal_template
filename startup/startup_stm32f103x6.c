/* Generated by startup_generator */

#include <stm32f1xx.h>
#include <core_cm3.h>

#define EN_UNALIGNED_EXCEPTION		(0)

typedef void (*IsrFunc)(void);

extern unsigned long _sidata;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;

const IsrFunc gaVectorTable[];

extern int main(void);
extern void _estack(void); // to force type checking

void _InitFault(void)
{
	SCB->CCR |= 
//		SCB_CCR_STKALIGN_Msk |		// Stack alignment 4 --> 8 bytes
//		SCB_CCR_BFHFNMIGN_Msk |
#if EN_UNALIGNED_EXCEPTION
		SCB_CCR_UNALIGN_TRP_Msk |	// raise Un-algined memory access.
#endif
		SCB_CCR_DIV_0_TRP_Msk;		// raise Div 0 Exception.

	// raise each handler.
	// SCB->SHCSR |=  SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk;
}

void default_handler(void)
{
	printf("Default Handler\n");
	while (1);
}

void __attribute__((weak)) __libc_init_array(void) {}

// Linker supplied pointers


void __attribute__((naked)) Reset_Handler(void);
void Reset_Handler(void)
{

	unsigned long *src, *dst;
	src = &_sidata;
	dst = &_sdata;

	// Copy data initializers
	while (dst < &_edata)
	{
		*(dst++) = *(src++);
	}
	// Zero bss
	dst = &_sbss;
	while (dst < &_ebss)
	{
		*(dst++) = 0;
	}

	SystemInit();
//	__libc_init_array(); // Should call this if C++
	SCB->VTOR = gaVectorTable;
	SystemCoreClockUpdate();
	_InitFault();
	main();
	while (1);
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void __attribute__((naked)) Fault_Handler(void)  
{
	unsigned nSP;
	unsigned nLR;
	/**
	 * LR == 0xFFFFFFF1 / 0xFFFFFFF9 : MSP is valid.
	 * LR == 0xFFFFFFFD : PSP is valid.
	*/
	__asm volatile
	(
		"tst    lr, #4; "
		"ite    eq;     "
		"mrseq  %[sp], msp;"
		"mrsne  %[sp], psp;"
		"mov    %[lr], lr; "
		: [sp] "=r" (nSP), [lr] "=r" (nLR):
	);
	printf("\n==== Fault ====\nSP: %8X, LR: %8X\n", nSP, nLR);
	unsigned* pStk = (unsigned*)nSP;
	printf("R0:  %8X\n", pStk[0]);
	printf("R1:  %8X\n", pStk[1]);
	printf("R2:  %8X\n", pStk[2]);
	printf("R3:  %8X\n", pStk[3]);
	printf("R12: %8X\n", pStk[4]);
	printf("LR:  %8X\n", pStk[5]);
	printf("PC:  %8X\n", pStk[6]);
	printf("PSR: %8X\n", pStk[7]);

	printf("\n==== SCB dump ====\nSCB->HFSR: %08lX\n", SCB->HFSR);
	printf("  SCB->CFSR: %08lX\n", SCB->CFSR);
	unsigned* pDwSCB = (unsigned*)SCB;
	for(unsigned i = 0; i < (sizeof(*SCB) / 4); i++)
	{
		if(0 == (i % 4))
		{
			printf("\nOff:%4X: ", i * 16);
		}
		printf("%08X ", pDwSCB[i]);
	}

	while(1);
}

/* Vector Table */

void NMI_Handler(void) __attribute__((weak, alias("default_handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Fault_Handler")));
void MemMange_Handler(void) __attribute__((weak, alias("Fault_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Fault_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Fault_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("default_handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("default_handler")));
void PendSV_Handler(void) __attribute__((weak, alias("default_handler")));
void SysTick_Handler(void) __attribute__((weak, alias("default_handler")));
void WWDG_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void PVD_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TAMPER_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void RTC_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void FLASH_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void RCC_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void EXTI0_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void EXTI1_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void EXTI2_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void EXTI3_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void EXTI4_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void DMA1_Channel1_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void DMA1_Channel2_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void DMA1_Channel3_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void DMA1_Channel4_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void DMA1_Channel5_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void DMA1_Channel6_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void DMA1_Channel7_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void ADC1_2_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void USB_HP_CAN1_TX_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void USB_LP_CAN1_RX0_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void CAN1_RX1_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void CAN1_SCE_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void EXTI9_5_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM1_BRK_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM1_UP_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM1_TRG_COM_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM1_CC_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM2_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM3_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM4_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void I2C1_EV_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void I2C1_ER_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void I2C2_EV_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void I2C2_ER_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void SPI1_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void SPI2_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void USART1_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void USART2_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void USART3_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void EXTI15_10_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void RTCAlarm_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void USBWakeUp_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM12_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM13_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM14_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void ADC3_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void FSMC_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM5_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void SPI3_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void UART4_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void UART5_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM6_DAC_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void TIM7_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void DMA2_Channel1_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void DMA2_Channel2_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void DMA2_Channel3_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void DMA2_Channel4_5_IRQHandler(void) __attribute__((weak, alias("default_handler")));
void DMA2_Channel5_IRQHandler(void) __attribute__((weak, alias("default_handler")));

__attribute__((section(".entry"), aligned(256)))
const IsrFunc gaEntry[] = 
{
	_estack,
	Reset_Handler,
};

__attribute__((aligned(256)))
const IsrFunc gaVectorTable[] = 
{
	_estack,
	Reset_Handler,
	NMI_Handler,
	HardFault_Handler,
	MemMange_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0, 0, 0, 0,
	SVC_Handler,
	DebugMon_Handler,
	0,
	PendSV_Handler,
	SysTick_Handler,
	WWDG_IRQHandler,
	PVD_IRQHandler,
	TAMPER_IRQHandler,
	RTC_IRQHandler,
	FLASH_IRQHandler,
	RCC_IRQHandler,
	EXTI0_IRQHandler,
	EXTI1_IRQHandler,
	EXTI2_IRQHandler,
	EXTI3_IRQHandler,
	EXTI4_IRQHandler,
	DMA1_Channel1_IRQHandler,
	DMA1_Channel2_IRQHandler,
	DMA1_Channel3_IRQHandler,
	DMA1_Channel4_IRQHandler,
	DMA1_Channel5_IRQHandler,
	DMA1_Channel6_IRQHandler,
	DMA1_Channel7_IRQHandler,
	ADC1_2_IRQHandler,
	USB_HP_CAN1_TX_IRQHandler,
	USB_LP_CAN1_RX0_IRQHandler,
	CAN1_RX1_IRQHandler,
	CAN1_SCE_IRQHandler,
	EXTI9_5_IRQHandler,
	TIM1_BRK_IRQHandler,
	TIM1_UP_IRQHandler,
	TIM1_TRG_COM_IRQHandler,
	TIM1_CC_IRQHandler,
	TIM2_IRQHandler,
	TIM3_IRQHandler,
	0,
	I2C1_EV_IRQHandler,
	I2C1_ER_IRQHandler,
	0, 0,
	SPI1_IRQHandler,
	0,
	USART1_IRQHandler,
	USART2_IRQHandler,
	0,
	EXTI15_10_IRQHandler,
	RTCAlarm_IRQHandler,
	USBWakeUp_IRQHandler,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0,
#if defined(STM32F10X_LD_VL) || (defined STM32F10X_MD_VL)
	[0x1CC / 4] = (void(*))0xF108F85F
#elif defined(STM32F10X_HD_VL)
	[0x1E0 / 4] = (void(*))0xF108F85F
#endif
};
