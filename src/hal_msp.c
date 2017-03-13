/*
 * hal_msp.c
 *
 *  Created on: Feb 18, 2017
 *      Author: Gordon
 */

#include "stm32f3xx.h"

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	UNUSED(hi2c);
	HAL_StatusTypeDef status;
	RCC_PeriphCLKInitTypeDef peripheral_clk_init;

	peripheral_clk_init.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
	peripheral_clk_init.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;

	status = HAL_RCCEx_PeriphCLKConfig(&peripheral_clk_init);
	if (status != HAL_OK)
	{
		while(1);
	}

	GPIO_InitTypeDef gpio_init;

	gpio_init.Pin = GPIO_PIN_6 | GPIO_PIN_7;
	gpio_init.Mode = GPIO_MODE_AF_OD;
	gpio_init.Pull = GPIO_NOPULL;
	gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;
	gpio_init.Alternate = GPIO_AF4_I2C1;

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();

	HAL_GPIO_Init(GPIOB, &gpio_init);

}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
	__HAL_RCC_I2C1_FORCE_RESET();
	__HAL_RCC_I2C1_RELEASE_RESET();

	HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6 | GPIO_PIN_7);
}
