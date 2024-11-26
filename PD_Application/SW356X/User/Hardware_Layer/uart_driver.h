#ifndef _UART_DRIVER_H_
#define _UART_DRIVER_H_

#include "zr_gpio.h"

#define UART_TX_PIN				GPIO6					//	GPIO TX IO口
#define UART_TX_AF_MODE			FUNCTION_SEL6			//	GPIO6_FUNC_SEL_UART_TX 	//	GPIO TX复用模式

#define UART_RX_PIN				GPIO7					//	GPIO RX IO口
#define UART_RX_AF_MODE			FUNCTION_SEL6			//	GPIO7_FUNC_SEL_UART_RX	//	GPIO RX复用模式


#endif
