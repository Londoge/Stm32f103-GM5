/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "myuart.h"
#include "oled.h"
#include "stdio.h"
#include "dht11.h"
//#include "stdbool.h"
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

/* USER CODE BEGIN PV */
extern unsigned char text[128];
extern unsigned char tempw;
extern unsigned char wp;
unsigned char rp;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

int16_t factory_crc16 ( uint8_t *bufData, uint16_t buflen);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
  unsigned char senddata[20];
int main(void)
{
  /* USER CODE BEGIN 1 */
  unsigned char rtext[20] = {0};
  unsigned char tpoint = 0;
  unsigned char temp[2];
  unsigned short int tempi;
  unsigned char otext[16];
  DHT11_Data_TypeDef dht11data = {0,0,0,0,0};
  
  unsigned char read_flag = 0;
  unsigned char spoint = 0;
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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(50);
  OLED_Init();
  OLED_Clear();
  sprintf((char*)otext,"OLED_Init Succes!");
  OLED_ShowString(1,1,(char*)otext);
  HAL_Delay(250);
  OLED_Clear();
  
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  HAL_UART_Receive_IT(&huart1,&tempw,1);
  while (1)
  {
//	  // static unsigned char tempx = 0x55;
//	  // HAL_Delay(1000);
//		// HAL_UART_Transmit(&huart1,(const uint8_t *)&tempx,1,0xff);
//    if(!Read_DHT11(&dht11data))
//    {
//      sprintf((char*)otext,"temp = %d.%d",dht11data.temp_int,dht11data.temp_deci);
//      OLED_ShowString(1,1,(char*)otext);
//      sprintf((char*)otext,"humi = %d.%d",dht11data.humi_int,dht11data.humi_deci);
//      OLED_ShowString(2,1,(char*)otext);
//    }
//    if(rp < wp || ((rp != 0)&&(wp == 0)))
//    {
//      // HAL_UART_Transmit(&huart1,&text[rp],1,0xff);
//      rtext[tpoint++] = text[rp++];
//		  if(rp >= 128)
//        rp = 0;
//    }
//    if(tpoint == 8)
//    {
//      tpoint = 0;
//      if(rtext[0] == 0x01)
//      {
//        switch (rtext[1])
//        {
//          case 0x03:
//              read_flag = 1;
//            break;
//          
//          default:
//            break;
//        }
//        if (read_flag == 1)
//        {
//          read_flag = 0;
//          senddata[0] = 0x01;
//          senddata[1] = 0x03;
//          senddata[2] = 0x02;
//          senddata[3] = 0x00;
//          switch (rtext[2])
//          {
//            case 0x00:
//                senddata[4] = dht11data.temp_int;
//              break;
//            case 0x01:
//                senddata[4] = dht11data.humi_int;
//              break;
//            default:
//              break;
//          }
//          
//          tempi = factory_crc16((uint8_t *)senddata,5);
//          senddata[5] = tempi & 0xff;
//          senddata[6] = (tempi >> 8) & 0xff;
//          HAL_UART_Transmit(&huart1,(const uint8_t *)senddata,7,0xff);
//          // HAL_UART_Transmit(&huart1,rtext,6,0xff);
//          // tempi = factory_crc16((uint8_t *)&rtext,tpoint);
//          // temp[1] = (tempi >> 8) & 0xff;
//          // temp[0] = tempi & 0xff;
//          // HAL_UART_Transmit(&huart1,(const uint8_t *)temp,2,0xff);
//        }
//        
//      }
//      
//      
//    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

/* USER CODE BEGIN 4 */
//CRC 校验
int16_t factory_crc16 ( uint8_t *bufData, uint16_t buflen)
{
    uint16_t TCPCRC = 0xffff;
    uint16_t POLYNOMIAL = 0xa001;
    uint8_t i, j;
   
    for (i = 0; i < buflen; i++)
    {
        TCPCRC ^= bufData[i];
        for (j = 0; j < 8; j++)
        {
            if ((TCPCRC & 0x0001) != 0)
            {
                TCPCRC >>= 1;
                TCPCRC ^= POLYNOMIAL;
            }
            else
            {
                TCPCRC >>= 1;
            }
        }
    }
    return TCPCRC;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
