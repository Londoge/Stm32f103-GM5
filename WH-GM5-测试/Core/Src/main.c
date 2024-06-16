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
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "myuart.h"
#include "oled.h"
#include "stdio.h"
#include "string.h"
//#include "stdbool.h"
#include "dht11.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

//设备ID
#define Device_ID 0x01

//MODBUS-功能ID-读取
#define MODBUS_READ 0x03


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern unsigned char text[rx_len];
uint8_t rx_flag;
extern unsigned int indx;

unsigned char temp_send[32];  //定义发送数组
unsigned char send_num = 0;   //定义发送数组指针

struct MODBUS_DATA{
  unsigned char data1_0;
  unsigned char data1_1;
  unsigned char data2_0;
  unsigned char data2_1;
  unsigned char data3_0;
  unsigned char data3_1;
  unsigned char data4_0;
  unsigned char data4_1;

  //add here
};

struct MODBUS_DATA modbus_data;
DHT11_Data_TypeDef DHT11_Data;

unsigned short adc1_data;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Uart_Process(void);
int16_t factory_crc16 ( uint8_t *bufData, uint16_t buflen);
unsigned short Get_ADCValue_1(void);
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
  unsigned char otext[16];
  
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
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(50);
  OLED_Init();
  OLED_Clear();
  sprintf((char*)otext,"OLED Act");
  OLED_ShowString(1,1,(char*)otext);
  HAL_Delay(50);
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);

  HAL_UARTEx_ReceiveToIdle_IT(&WHGM5_huart, text, 16);
  HAL_ADC_Start_IT(&hadc1);


  while (1)
  {
    //如果接收到 一帧数据 进行数据处理
    if(rx_flag == 1)
    {
      rx_flag = 0;
      Uart_Process();
	    HAL_Delay(500);
      // HAL_UART_Transmit(&WHGM5_huart,text,indx,0xff);
      // memset(text,0,rx_len);
      // sprintf((char *)otext," %02d,%02d",DHT11_Data.temp_int,DHT11_Data.humi_int);
      // HAL_UART_Transmit(&WHGM5_huart,otext,6,0xff);
    }
    adc1_data = Get_ADCValue_1();
    memset(otext,0,5);
    sprintf((char *)otext,"%04d",adc1_data);
    OLED_ShowString(3,1,(char *)otext);
    HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
    HAL_Delay(250);
    if(Read_DHT11(&DHT11_Data) == SUCCESS)
    {
      memset(otext,0,5);
      sprintf((char *)otext,"%02d,%02d",DHT11_Data.temp_int,DHT11_Data.humi_int);
      OLED_ShowString(2,1,(char *)otext);
      OLED_ShowString(3,1,(char *)"OK");
    }
    else
    {
      OLED_ShowString(3,1,(char *)"NO");
    }

    modbus_data.data1_0 = 0;
    modbus_data.data1_1 = DHT11_Data.temp_int;

    modbus_data.data2_0 = 0;
    modbus_data.data2_1 = DHT11_Data.humi_int;

    modbus_data.data3_0 = 0x27;
    modbus_data.data3_1 = 0x10;   // 10k for test

    modbus_data.data4_0 = adc1_data >> 8;
    modbus_data.data4_1 = adc1_data;
    

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
/*
  主机对从机写操作
  01             06             00 01           00 17          98 04 
  从机地址        功能号          数据地址          数据         CRC校验



  主机对从机读操作
  01             03            00 01           00 01          D5 CA 
  从机地址        功能号          数据地址      读取数据个数       CRC校验
  返回内容：  
  01         03            02             00 17          F8 4A
  从机地址   功能号     数据字节个数    两个字节数据    CRC校验
  


  --有人云测试： 默认单从机模式 设备地址01 读取功能号03 数据地址为40001起 校验方式为CRC16/MODBUS
  CRC16|MODBUS 多项式公式 'x16 + x15 + x2 + 1'
*/

unsigned short Get_ADCValue_1(void)
{
  int ADC_Num;
	HAL_ADC_Start_IT(&hadc1);
  ADC_Num = HAL_ADC_GetValue(&hadc1);
  return (ADC_Num);
}

void Uart_Process(void)
{
  //接收一帧数据后  对数据进行处理的函数
  unsigned char *place = NULL; //数据寄存器地址起始位置

  uint16_t byte_num = 0; //需要填入的数据字节量

  int16_t Check_num = 0; //校验位
  Check_num = factory_crc16(text,indx-2);

  
  memset(temp_send,0,32);

  if(((Check_num & 0x00ff)!= text[indx - 2]) || (((Check_num >> 8) & 0x00ff) != text[indx - 1]))
  {
    //校验位不符合 舍弃数据
    return;
  }
  else
  {
    //CRC通过 进行数据判定
    if(text[0] != Device_ID) //设备号不符合 抛弃
      return;      
    temp_send[0] = Device_ID;
    
    //根据 MODBUS-功能ID 来执行操作 
    switch (text[1])
    {
      //读取变量
      case MODBUS_READ:
          byte_num = ((text[4] << 8 | text[5]))*2;
          temp_send[1] = MODBUS_READ;
          temp_send[2] = byte_num;
          send_num = 3;
          place = (&(modbus_data.data1_0) + ((text[2] << 8) | text[3])*2);
          while(byte_num)
          {
            temp_send[send_num++] = *(place++);
            temp_send[send_num++] = *(place++);
            byte_num -= 2;
          }
          Check_num = factory_crc16((uint8_t*)temp_send,send_num);
          temp_send[send_num++] = Check_num;
          temp_send[send_num++] = Check_num >> 8;
          HAL_UART_Transmit(&WHGM5_huart,temp_send,send_num,0xffffffff);
        break;
      
      default:

        break;
    }
  }
}




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
