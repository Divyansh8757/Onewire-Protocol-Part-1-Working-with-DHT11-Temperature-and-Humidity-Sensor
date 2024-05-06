#include "main.h"
#include "string.h"
#include <stdio.h>

TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart1;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
static void MX_GPIO_Init(void);
static void MX_ICACHE_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USB_DRD_FS_HCD_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */
void SetPin_Out();						// Function to set GPIO Pin as Output
void SetPin_In();						// Function to set GPIO Pin as Input
int Sensor_Initialize();					// Function to Initialize DHT11 Sensor
void udelay(uint32_t us);					// Function to create delay in microseconds
uint8_t read();							// Function to read Temperature and Humidity values
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
// User Variable Declarations
uint8_t timCnt1=0;						// Variable to count microseconds delay
uint8_t timCnt2=0;
uint8_t intRH;							// Variable to store integer part of Temperature value
uint8_t decRH;							// Variable to store decimal part of Temperature value
uint8_t intT;							// Variable to store integer part of Humidity value
uint8_t decT;							// Variable to store decimal part of Humidity value
uint8_t Checksum;						// Variable to store Checksum value
char temp[32];							// Variable to display Temperature value though UART
char Humi[32];							// Variable to display Humidity value though UART
char error[32];
/* USER CODE END 0 */

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  SystemPower_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ICACHE_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start(&htim6);						// Initialise timer for microsecond delay

while(1){
int Ret = Sensor_Initialize();						// Initialization of sensor returns 1	
if(Ret==1)
{
  intRH = read();							// Read Integer value of Humidity
  decRH = read();							// Read Decimal value of Humidity
  intT = read();							// Read Integer value of Temperature
  decT = read();							// Raed Decimal value of Temperature
  Checksum= read();							// Read Checksum value
  sprintf(temp, "Temperature = %d.%d\n\r ", intT,decT);			
  sprintf(Humi, "Humidity = %d.%d\n\r ", intRH,decRH);			
  HAL_UART_Transmit(&huart1, (uint8_t*)temp, sizeof(temp), 10);		// Display Temperature value
  HAL_UART_Transmit(&huart1, (uint8_t*)Humi, sizeof(Humi), 10);		// Display Humidity Value
  HAL_Delay(3000);							// Delay of 3 seconds before reading the values again
}
else{
	sprintf(error, "Sensor Initialization Failed\n\r ");		
	HAL_UART_Transmit(&huart1, (uint8_t*)error, sizeof(error), 10);		// Print error message
}
}
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_0;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLMBOOST = RCC_PLLMBOOST_DIV4;
  RCC_OscInitStruct.PLL.PLLM = 3;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 1;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLLVCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV8;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

static void SystemPower_Config(void)
{

  /*
   * Switch to SMPS regulator instead of LDO
   */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }
/* USER CODE BEGIN PWR */
/* USER CODE END PWR */
}

static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache in 1-way (direct mapped cache)
  */
  if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 16-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 65535;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DHT11_Pin|LED_GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_BUTTON_Pin */
  GPIO_InitStruct.Pin = USER_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_BUTTON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DHT11_Pin */
  GPIO_InitStruct.Pin = DHT11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_GREEN_Pin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_GREEN_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
//Sensor Functions start here

void udelay(uint32_t us){
__HAL_TIM_SET_COUNTER(&htim6,0);
while((__HAL_TIM_GET_COUNTER(&htim6))<us){}
}

int Sensor_Initialize(){
	int Response=0;
	SetPin_In();							// Set GPIO pin Input
	HAL_Delay(100);
	SetPin_Out();							// Set GPIO Pin Output
	HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin,0);		// Set pin low for 18 milliseconds
	HAL_Delay(18);
	HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin,1);		// Set pin high for 40 microseconds
	udelay(40);
	SetPin_In();							// Set GPIO pin Input
	__HAL_TIM_SET_COUNTER(&htim6,0);
	timCnt1=__HAL_TIM_GET_COUNTER(&htim6);
	while(!(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)))		// Check if the GPIO pin is low for a maximum of 80 microseconds
		timCnt2=__HAL_TIM_GET_COUNTER(&htim6);
	if(timCnt2-timCnt1<=80){
		__HAL_TIM_SET_COUNTER(&htim6,0);
		timCnt1=__HAL_TIM_GET_COUNTER(&htim6);
		while(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin))	// Check if the Gpio pin is high for a maximum of 80 microseconds
			timCnt2=__HAL_TIM_GET_COUNTER(&htim6);
		if(timCnt2-timCnt1<=80)
			Response =1;					// if the conditions are true then sensor initialization was a success
		else
			Response=-1;					// else sensor initialization failed
	}
	else
		Response = -1;
	return Response;						// Return response
}

uint8_t read(){								// Function to read values from sensor
	uint8_t data={0};
	SetPin_In();							// Set pin Input
	for(int i=0;i<8;i++){
		__HAL_TIM_SET_COUNTER(&htim6,0);
		timCnt1=__HAL_TIM_GET_COUNTER(&htim6);
		while(!(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)))	// if the sensor holds pin low for a maximum of 50 microseconds then it means sensor is ready to transmit
			timCnt2=__HAL_TIM_GET_COUNTER(&htim6);
		if(timCnt2-timCnt1<=50)
		{
			__HAL_TIM_SET_COUNTER(&htim6,0);
			timCnt1=__HAL_TIM_GET_COUNTER(&htim6);
			while(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin))	// if the sensor holds pin high for a maximum of 28 microseconds then it means it is transmitting 0
				timCnt2=__HAL_TIM_GET_COUNTER(&htim6);
			if(timCnt2-timCnt1<=28)
				data|=(0<<(7-i));
			else if (timCnt2-timCnt1>28 && timCnt2-timCnt1<=80)	// if the sensor holds pin high for a maximum of 80 microseconds then it means it is transmitting 1
				data|=(1<<(7-i));
			else
				Error_Handler();				// else there is an error in transmitting
		}
	}
	return data;							
}


void SetPin_In(){								// Function to set GPIO pin Input
GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = DHT11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
  }
void SetPin_Out(){								// Function to set GPIO pin Output
GPIO_InitTypeDef GPIO_InitStruct = {0};
 GPIO_InitStruct.Pin = DHT11_Pin;
 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
 GPIO_InitStruct.Pull = GPIO_NOPULL;
 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
 HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);
 }
/* USER CODE END 4 */

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
