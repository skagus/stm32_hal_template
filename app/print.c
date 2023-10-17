#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include "stm32f1xx.h"

extern UART_HandleTypeDef huart1;

int puts(const char* szString)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)szString, strlen(szString), 100);
	return 0;
}

#define MAX_BUF_SIZE		(64)
int myPrintf(const char* szFmt, ...)
{
	char buffer[MAX_BUF_SIZE];
	va_list ap;
	int len;

	va_start(ap, szFmt);
	len = vsprintf(buffer, szFmt, ap);
	va_end(ap);

	HAL_UART_Transmit(&huart1, (uint8_t*)buffer, len, 100);
	return len;
}

int myPuts(const char* szString)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)szString, strlen(szString), 100);
	return 0;
}

