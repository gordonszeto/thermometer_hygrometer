/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f3xx.h"
#include "stm32f3_discovery.h"

#include "htu21d.h"

#include <stddef.h>
			
#define SENSOR_I2C_ADDRESS 0x40

extern void initialise_monitor_handles(void);

void delay(uint32_t cycles)
{
	for (int i = 0; i < cycles; ++i);
}

void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

void init(void)
{
	HAL_StatusTypeDef status;
	status = HAL_Init();
	if (status != HAL_OK)
		while(1);

	SystemClock_Config();
}

int main(void)
{
	HAL_StatusTypeDef status;
	I2C_HandleTypeDef i2c;

	init();

	initialise_monitor_handles();

	i2c.Instance = I2C1;
	i2c.Init.Timing = 0x905064FE;
	i2c.Init.OwnAddress1 = 0x32;
	i2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	i2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

	status = HAL_I2C_Init(&i2c);
	if (status != HAL_OK)
	{
		while(1);
	}

	//delay(72000 * 40);

	while (1)
	{
		uint32_t sensor_data = 0;
		printf("Hello World\n");

		uint8_t command1 = HTU21D_I2C_TRIG_TEMP_MEAS_CMD_NO_HLD;
		status = HAL_I2C_Master_Transmit(&i2c, SENSOR_I2C_ADDRESS << 1, &command1, sizeof(command1), 10000);
		if (status != HAL_OK)
		{
			while (1);
		}

		delay(72 * 50 * 1000);

		status = HAL_I2C_Master_Receive(&i2c, SENSOR_I2C_ADDRESS << 1, (uint8_t *)&sensor_data, 3, 10000);
		if (status != HAL_OK)
		{
			while (1);
		}
		float temperature = htu21d_convert_temperature(sensor_data);
		printf("Temperature: %f\n", temperature);
//
//		uint8_t command2 = HTU21D_I2C_TRIG_HUM_MEAS_CMD_NO_HLD;
//		status = HAL_I2C_Master_Transmit(&i2c, SENSOR_I2C_ADDRESS << 1, &command2, sizeof(command2), 10000);
//		if (status != HAL_OK)
//		{
//			while (1);
//		}
//
//		delay(72 * 50 * 1000);
//
//		sensor_data = 0;
//		status = HAL_I2C_Master_Receive(&i2c, SENSOR_I2C_ADDRESS << 1, (uint8_t *)&sensor_data, 3, 10000);
//		if (status != HAL_OK)
//		{
//			while (1);
//		}
//		float humidity = htu21d_convert_humidity(sensor_data);
//		printf("Humidity: %.2f%%\n", humidity);

		delay(72 * 1000 * 1000);
	}


	for(;;);
}
