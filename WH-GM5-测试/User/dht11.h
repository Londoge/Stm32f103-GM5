#ifndef __DHT11_H_
#define __DHT11_H_

#include "main.h"

#define uint16_t unsigned short
#define uint8_t unsigned char
#define DHT11PORT GPIOA
#define DHT11PIN GPIO_PIN_0

typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
		                 
}DHT11_Data_TypeDef;

void Delay_us(unsigned int time);
void DHT_GPIO_SET_OUTPUT(void);
void DHT_GPIO_SET_INPUT(void);
uint8_t Read_Byte(void);
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data);
uint16_t HAL_GetTick_us(void);

#endif
