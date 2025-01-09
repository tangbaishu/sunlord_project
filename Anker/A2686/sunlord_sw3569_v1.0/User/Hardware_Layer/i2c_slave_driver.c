/****************************************************************************
 * @copyright Copyright(C) 2020-2023 Ismartware Limited. All rights reserved.
 * @file i2c_slave.c
 * @brief i2c slave read demo
 * @author Zeng jianbo
 ****************************************************************************/
#include "i2c_slave_driver.h"
#include "hardware_config.h"
#include "zr_i2c.h"
#include "zr_gpio.h"
#include "zr_rcc.h"
#include "system.h"
#include "zr_nvic.h"
#include "api.h"

static const uint8_t gReg0x1Data[] = {0x1, 0x2, 0x3, 0x4};

static uint8_t gReg0x8Data = 0;

P_I2c_Slave_Int_Dispose   Slave_Int_Dispose;

void I2c_Slave_Driver_Init(P_I2c_Slave_Int_Dispose Int_Dispose_Func, Slave_Device_Addr_e dev_addr)
{
    Rcc_Module_Rst(I2C_ID);
    Rcc_Module_Clk_Enable(GPIO_ID | I2C_ID);
    Rcc_Module_Rst_Release(I2C_ID);
    Gpio_Function_Select(I2C_S_SCK_GPIO_PIN, I2C_S_SCK_AF_MODE);
    Gpio_Function_Select(I2C_S_SDA_GPIO_PIN, I2C_S_SDA_AF_MODE);

    Gpio_Mos_Opendrain_Cfg(I2C_S_SCK_GPIO_PIN, PMOS_OPDRAIN_DIS, NMOS_OPDRAIN_EN);
    Gpio_Mos_Opendrain_Cfg(I2C_S_SDA_GPIO_PIN, PMOS_OPDRAIN_DIS, NMOS_OPDRAIN_EN);
    Gpio_Pulldown_Pullup_Cfg(I2C_S_SCK_GPIO_PIN, PULLDOWN_DIS, PULLUP_EN);
    Gpio_Pulldown_Pullup_Cfg(I2C_S_SDA_GPIO_PIN, PULLDOWN_DIS, PULLUP_EN);

    I2c_Slave_Device_Addr_Cfg(dev_addr);

    I2c_Slave_TX_FIFO_Reset();
    I2c_Slave_Int_Enable(I2C_S_TX_DATA_INT_EN | I2C_S_RX_DATA_INT_EN);
    // set to highest priority - > PRIORITY_0
    Nvic_Irq_Enable(I2CS_IRQn, PRIORITY_0);
    I2c_Slave_Enable();
    I2c_Slave_Clear_Pending(I2C_S_RX_DATA_PENGING | I2C_S_TX_DATA_PENGING | I2C_S_FINISH_PENGING
                            | I2C_S_RX_OVERFLOW_PENGING | I2C_S_TX_UNDERFLOW_PENGING | I2C_S_RX_FIFO_PENGING
                            | I2C_S_TX_FIFO_PENGING | I2C_S_BUS_ERROR_PENGING);
    Slave_Int_Dispose = Int_Dispose_Func;
}

void I2CS_Handler()
{
    Slave_Int_Dispose();
}