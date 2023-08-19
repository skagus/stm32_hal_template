// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.

#include <string.h>
#include "stm32f1xx.h"
#include "main.h"

extern char* gpVersion;

#define MSEC(x) 	(x)
#define SEC(x)		(MSEC((x)*1000))

void _mydelay(uint32_t nCycles)
{
	while(nCycles-- > 0)
	{
		__asm volatile("nop");
	}
}

void SysTick_Handler(void)
{
	HAL_IncTick();
}

void _Blink(int nPeriod, uint32_t nCnt)
{
	while(--nCnt > 0)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		HAL_Delay(nPeriod);
	}
}

void _GpioInit(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void _ClockInit(void)
{
	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitTypeDef oscinitstruct = {0};
	oscinitstruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	oscinitstruct.HSEState = RCC_HSE_ON;
	oscinitstruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	oscinitstruct.PLL.PLLState = RCC_PLL_ON;
	oscinitstruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	oscinitstruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
	{
		/* Initialization Error */
		while(1) {}
	}

	/* Select PLL as system clock source and configure the HCLK (AHB), PCLK1 (APB1), PCLK2 (APB2) clocks */
	RCC_ClkInitTypeDef clkinitstruct = {0};
	clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; /* 72 MHz */
	clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1; /* AHB at 72 MHz*/
	clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2; /* APB1 at 36 MHz*/
	clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1; /* APB2 at 72 MHz */

	if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
	{
		/* Initialization Error */
		while(1) {}
	}
}

#pragma GCC push_options
#pragma GCC optimize ("O0")
int _TrigFault(int nA, int nB)
{
	UNUSED(nA); UNUSED(nB);

//	*(int*)(_Blink) = nA;	// Bus Fault: Write read only flash.
//	return (nA / nB);  // Usage Fault: Devide by Zero.
//	asm volatile(".word	0xFFAACCBB"); // Usage Fault: Inst decode failure.

	// Unaligned access.
	uint8_t aByte[8] = {0,};
	printf("Ch Value: %2X %2X %2X %2X\n",
		aByte[0], aByte[1], aByte[2], aByte[3]);
	*((uint32_t*)(aByte + 1)) = 10;
	printf("Ch Value: %2X %2X %2X %2X\n",
		aByte[0], aByte[1], aByte[2], aByte[3]);

	return 0;
}
#pragma GCC pop_options

int main(int argc, char* argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	HAL_Init();

	_GpioInit();

	_ClockInit();
//	_Blink(1000, 10);

	_Blink(MSEC(500), 10);
	print_init();
	printf("\n\nFW build %s\n", gpVersion);
	int nCnt = 0;
	while(1)
	{
		printf("Loop: %8X\n", nCnt++);
		_TrigFault(20, 0);
		_Blink(MSEC(200), 10);
	}
	return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
