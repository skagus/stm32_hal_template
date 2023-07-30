// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.

#include <string.h>
#include "stm32f1xx.h"
#include "main.h"

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


int main(int argc, char* argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	HAL_Init();

	_GpioInit();
	_Blink(500, 10);

	_ClockInit();
	_Blink(1000, 10);

	print_init();

	char* szHello = "Hello\n";
	int nCnt = 0;
	while(1)
	{
		printf("Loop: %5d\n", nCnt++);
		_Blink(100, 10);
	}
	return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
