#include <stdarg.h>
#include "stm32f1xx.h"

UART_HandleTypeDef ghUART1;

void print_init(void)
{
	ghUART1.Instance = USART1;
	ghUART1.Init.BaudRate = 115200;
	ghUART1.Init.WordLength = UART_WORDLENGTH_8B;
	ghUART1.Init.StopBits = UART_STOPBITS_1;
	ghUART1.Init.Parity = UART_PARITY_NONE;
	ghUART1.Init.Mode = UART_MODE_TX_RX;
	ghUART1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	if(HAL_OK !=  HAL_UART_Init(&ghUART1))
	{
		_Blink(20, 0);
	}
}

int printf(const char* szFmt, ...)
{
	char buffer[128];
	va_list ap;
	int len;

	va_start(ap, szFmt);
	len = vsprintf(buffer, szFmt, ap);
	va_end(ap);

	HAL_UART_Transmit(&ghUART1, (uint8_t*)buffer, len, 100000);
	return 0;
}
