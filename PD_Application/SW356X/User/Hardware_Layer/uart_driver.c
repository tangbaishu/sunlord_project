#include "uart_driver.h"
#include "zr_uart.h"
#include "zr_rcc.h"
#include <stdio.h>

void UART_Driver_Init(void)
{
	gpio_config_t uart_gpio_config;
	uart_config_t uart_config;

	Rcc_Module_Clk_Enable(GPIO_ID);
	Gpio_Deinit(UART_TX_PIN | UART_RX_PIN);

	uart_gpio_config.direction = GPIO_DIRECTION_OUTPUT;
	uart_gpio_config.gpioFunc = UART_TX_AF_MODE;
	uart_gpio_config.pullup = PULLUP_EN;
	Gpio_Init(UART_TX_PIN, &uart_gpio_config);
	uart_gpio_config.direction = GPIO_DIRECTION_INPUT;
	uart_gpio_config.gpioFunc = UART_RX_AF_MODE;
	Gpio_Init(UART_RX_PIN, &uart_gpio_config);

	Rcc_Module_Rst(UART_ID);
	Rcc_Module_Clk_Enable(UART_ID);
	Rcc_Module_Rst_Release(UART_ID);
	
	uart_config.baud = 115200;
	uart_config.mode = MODE_TXRX;
	Uart_Init(&uart_config);
}

