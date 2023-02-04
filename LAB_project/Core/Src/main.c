/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

struct PortPin
{
	GPIO_TypeDef* PORT;
	uint16_t PIN;
};
struct PortPin R[4] =
{
		{GPIOA,GPIO_PIN_10},
		{GPIOB,GPIO_PIN_3},
		{GPIOB,GPIO_PIN_5},
		{GPIOB,GPIO_PIN_4},
};
struct PortPin L[4] =
{
		{GPIOA,GPIO_PIN_9},
		{GPIOC,GPIO_PIN_7},
		{GPIOB,GPIO_PIN_6},
		{GPIOA,GPIO_PIN_7},
};

uint16_t ButtonMatrix=0;ButtonMatrix_L = 0;
int Input_st = 0, state_st = 0; //input state & Real time state
int RF_E = 0, mistake = 0;
int clear = 4096,bs = 8192,ok = 32768;

//0000000000000001 = 7 or first position '1'
//0000000000000010 = 4 or second position '2'
//0000000000000100 = 1 or '4'
//0000000000001000 = 0 or '8'


//struct GPIOState
//{
//	GPIO_PinState Current;
//	GPIO_PinState Last;
//};
//
////declare variable
//struct GPIOState Button1;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  static uint32_t timestamp = 0;
	  if(HAL_GetTick()>=timestamp)
	  {
		  timestamp = HAL_GetTick()+50;
		  ReadMatrixButton_1Row();
	  }

	  if (ButtonMatrix > ButtonMatrix_L) {
	  			RF_E = 1;
	  		} else {
	  			RF_E = 0;
	  		}
	  		ButtonMatrix_L = ButtonMatrix;

	  		switch (Input_st) {
	  		case (0)://initial
	  			state_st = 0;
	  			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	  			if (RF_E == 1) {
	  				if (ButtonMatrix == 512) { //6=512
	  					Input_st = 1;
	  				} else if (ButtonMatrix == clear) {
	  					Input_st = 0;
	  				} else if (ButtonMatrix == bs) {
	  					Input_st = 0;
	  				} else if (ButtonMatrix == ok) {
	  					Input_st = 0;
	  				} else if (ButtonMatrix == 0) {
	  					Input_st = 0;
	  				} else {
	  					Input_st = 12;
	  				}
	  			}
	  			break;
	  		case (1): //6
	  			state_st = 1;
	  			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	  			if (RF_E == 1) {
	  				if (ButtonMatrix == 2) { // 4 = 2
	  					Input_st = 2;
	  				} else if (ButtonMatrix == clear) {
	  					Input_st = 0;
	  				} else if (ButtonMatrix == bs) {
	  					Input_st = 0;
	  				} else if (ButtonMatrix == ok) {
	  					Input_st = 1;
	  				} else if (ButtonMatrix == 0) {
	  					Input_st = 1;
	  				} else {
	  					Input_st = 12;
	  				}
	  			}
	  			break;
	  		case (2)://64
	  			state_st = 2;
	  			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	  			if (RF_E == 1) {
	  				if (ButtonMatrix == 1024) { //3 = 1024
	  					Input_st = 3;
	  				} else if (ButtonMatrix == clear) {
	  					Input_st = 0;
	  				} else if (ButtonMatrix == bs) {
	  					Input_st = 1;
	  				} else if (ButtonMatrix == ok) {
	  					Input_st = 2;
	  				} else if (ButtonMatrix == 0) {
	  					Input_st = 2;
	  				} else {
	  					Input_st = 12;
	  				}
	  			}
	  			break;
	  		case (3)://643
	  			state_st = 3;
	  			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	  			if (RF_E == 1) {
	  				if (ButtonMatrix == 2) { //4 = 2
	  					Input_st = 4;
	  				} else if (ButtonMatrix == clear) {
	  					Input_st = 0;
	  				} else if (ButtonMatrix == bs) {
	  					Input_st = 2;
	  				} else if (ButtonMatrix == ok) {
	  					Input_st = 3;
	  				} else if (ButtonMatrix == 0) {
	  					Input_st = 3;
	  				} else {
	  					Input_st = 12;
	  				}
	  			}
	  			break;
	  		case (4)://6434
	  			state_st = 4;
	  			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	  			if (RF_E == 1) {
	  				if (ButtonMatrix == 8) { //0 = 8
	  					Input_st = 5;
	  				} else if (ButtonMatrix == clear) {
	  					Input_st = 0;
	  				} else if (ButtonMatrix == bs) {
	  					Input_st = 3;
	  				} else if (ButtonMatrix == ok) {
	  					Input_st = 4;
	  				} else if (ButtonMatrix == 0) {
	  					Input_st = 4;
	  				} else {
	  					Input_st = 12;
	  				}
	  			}
	  			break;
	  		case (5)://64340
	  			state_st = 5;
	  			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	  			if (RF_E == 1) {
	  				if (ButtonMatrix == 32) { //5 = 32
	  					Input_st = 6;
	  				} else if (ButtonMatrix == clear) {
	  					Input_st = 0;
	  				} else if (ButtonMatrix == bs) {
	  					Input_st = 4;
	  				} else if (ButtonMatrix == ok) {
	  					Input_st = 5;
	  				} else if (ButtonMatrix == 0) {
	  					Input_st = 5;
	  				} else {
	  					Input_st = 12;
	  				}
	  			}
	  			break;
	  		case (6)://643405
	  			state_st = 6;
	  			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	  			if (RF_E == 1) {
	  				if (ButtonMatrix == 8) { //0 = 8
	  					Input_st = 7;
	  				} else if (ButtonMatrix == clear) {
	  					Input_st = 0;
	  				} else if (ButtonMatrix == bs) {
	  					Input_st = 5;
	  				} else if (ButtonMatrix == ok) {
	  					Input_st = 6;
	  				} else if (ButtonMatrix == 0) {
	  					Input_st = 6;
	  				} else {
	  					Input_st = 12;
	  				}
	  			}
	  			break;
	  		case (7)://6434050
	  			state_st = 7;
	  			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	  			if (RF_E == 1) {
	  				if (ButtonMatrix == 8) { //0 = 8
	  					Input_st = 8;
	  				} else if (ButtonMatrix == clear) {
	  					Input_st = 0;
	  				} else if (ButtonMatrix == bs) {
	  					Input_st = 6;
	  				} else if (ButtonMatrix == ok) {
	  					Input_st = 7;
	  				} else if (ButtonMatrix == 0) {
	  					Input_st = 7;
	  				} else {
	  					Input_st = 12;
	  				}
	  			}
	  			break;
	  		case (8):
	  			state_st = 8;//64340500
	  			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	  			if (RF_E == 1) {
	  				if (ButtonMatrix == 0x8) { //0 = 8
	  					Input_st = 9;
	  				} else if (ButtonMatrix == clear) {
	  					Input_st = 0;
	  				} else if (ButtonMatrix == bs) {
	  					Input_st = 7;
	  				} else if (ButtonMatrix == ok) {
	  					Input_st = 8;
	  				} else if (ButtonMatrix == 0) {
	  					Input_st = 8;
	  				} else {
	  					Input_st = 12;
	  				}
	  			}
	  			break;
	  		case (9)://643405000
	  			state_st = 9;
	  			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	  			if (RF_E == 1) {
	  				if (ButtonMatrix == 64) {// 2 = 64
	  					Input_st = 10;
	  				} else if (ButtonMatrix == clear) {
	  					Input_st = 0;
	  				} else if (ButtonMatrix == bs) {
	  					Input_st = 8;
	  				} else if (ButtonMatrix == ok) {
	  					Input_st = 9;
	  				} else if (ButtonMatrix == 0) {
	  					Input_st = 9;
	  				} else {
	  					Input_st = 12;
	  				}
	  			}
	  			break;
	  		case (10)://6434050002
	  			state_st = 10;
	  			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	  			if (RF_E == 1) {
	  				if (ButtonMatrix == 8) { //0 = 8
	  					Input_st = 11;
	  				} else if (ButtonMatrix == clear) {
	  					Input_st = 0;
	  				} else if (ButtonMatrix == bs) {
	  					Input_st = 9;
	  				} else if (ButtonMatrix == ok) {
	  					Input_st = 10;
	  				} else if (ButtonMatrix == 0) {
	  					Input_st = 10;
	  				} else {
	  					Input_st = 12;
	  				}
	  			}
	  			break;
	  		case (11)://64340500020
	  			state_st = 11;
	  			if (RF_E == 1) {
	  				if (ButtonMatrix == 0x8000) { //if enter ld2 high
	  					HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
	  				} else if (ButtonMatrix == 0) {
	  					Input_st = 11;
	  				} else if (ButtonMatrix == bs) {
	  					Input_st = 10;
	  				} else if (ButtonMatrix == clear) {
	  					Input_st = 0;
	  				} else {
	  					Input_st = 0;
	  				}
	  			}
	  			break;
	  		case (12):
	  			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
	  			if (RF_E == 1) {
	  				if (ButtonMatrix == clear) {
	  					Input_st = 0;
	  					mistake = 0;
	  				} else if (ButtonMatrix == ok) {
	  					Input_st = 12;
	  				} else if (mistake == 0 && ButtonMatrix == bs) {
	  					Input_st = state_st;
	  				} else if (ButtonMatrix == bs) {
	  					mistake -= 1;
	  					Input_st = 12;
	  				} else {
	  					mistake++;
	  					Input_st = 12;
	  				}
	  			}
	  			break;
	  		}


	  //PIN 1,5 Curse
	  //read B1 and save it to Button1
//	  GPIO_PinState Button1;
//	  Button1 = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10);
//
//	  //send logic from buttn1 to LD2
//	  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, Button1);

	  //Week 1 Blink LD2
//	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5 , GPIO_PIN_SET);
//	  HAL_Delay(500);
//	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5 , GPIO_PIN_RESET);
//	  HAL_Delay(500);


//	  //Example1 change HZ
//	  static uint32_t delayTime = 500;
//	  Button1.Current = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
//	  if(Button1.Last == 1 && Button1.Current == 0)
//	  {
//		  //toggle pin
//		  if(delayTime = 500)
//		  {
//			  delayTime = 1000;
//		  }
//
//		  else
//		  {
//			  delayTime = 500;
//		  }
//	  }
//
//	  Button1.Last = Button1.Current;
//
//	  //Example1 P7 W2
//	  static uint32_t timestamp = 0;
//	  //check current time more than alarm time
//	  if(HAL_GetTick()>= timestamp)
//	  {
//		  //set next alarm
//		  timestamp = HAL_GetTick()+ delayTime;
//
//		  //toggle led
//		  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//	  }

//	  GPIO_PinState Button;
//	  Buttin = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10);
//	  HAL_GPIO_W

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, R2_Pin|R4_Pin|R3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : L4_Pin L1_Pin */
  GPIO_InitStruct.Pin = L4_Pin|L1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : L2_Pin */
  GPIO_InitStruct.Pin = L2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(L2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : R1_Pin */
  GPIO_InitStruct.Pin = R1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(R1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PC10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : R2_Pin R4_Pin R3_Pin */
  GPIO_InitStruct.Pin = R2_Pin|R4_Pin|R3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : L3_Pin */
  GPIO_InitStruct.Pin = L3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(L3_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void ReadMatrixButton_1Row()
{
	static uint8_t x = 0;
	register int i;
	for(i=0;i<4;i++)
	{
		if(HAL_GPIO_ReadPin(L[i].PORT, L[i].PIN) == 1)
		{
			ButtonMatrix &= ~(1<<x*4+i);
		}
		else
		{
			ButtonMatrix |= 1<<(x*4+i);
		}
	}
	//Set RX
	HAL_GPIO_WritePin(R[x].PORT, R[x].PIN, 1);
	//Reset RX+1%4
	HAL_GPIO_WritePin(R[(x+1)%4].PORT, R[(x+1)%4].PIN, 0);
	x++;
	x%=4;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
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
