#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "dht11.h"
#define usdelay(X) while(X--);

void Delay_us(unsigned int time)
{
	unsigned int temptime;
	temptime = time * 10;
	usdelay(temptime);
}

/*------------------------------*/
void DHT_GPIO_SET_OUTPUT(void)     //设置GPIOx为输出模式（MCU的IO口向DHT11发激活信号）
{
	GPIO_InitTypeDef GPIO_InitStructure;    //在GPIO_InitTypeDef结构体中修改IO口参数（结构体成员）
	GPIO_InitStructure.Pin=DHT11PIN;      //设置的格式必须严格遵循注释，比如GPIO_PIN_define
	GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull=GPIO_NOPULL;
	GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DHT11PORT,&GPIO_InitStructure);
}

void DHT_GPIO_SET_INPUT(void)     //设置GPIOx为输入模式（DHT11向MUC的IO发电平信号，信号里包含了温湿度信息）
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin=DHT11PIN;
	GPIO_InitStructure.Mode=GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull=GPIO_PULLUP;
	GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DHT11PORT,&GPIO_InitStructure);
}

/*------------------------------*/

/*
     uint8_t DHT_Read_Byte(void)用于转译采集DHT11发送给IO口的电平信号（8位）。
*/
unsigned short time_wait = 25565;
uint8_t Read_Byte(void)  //从DHT11读取一位（8字节）信号
{
	uint8_t i, temp=0;

	for (i=0; i<8; i++)    
	{	 
		time_wait = 25565;

		while (HAL_GPIO_ReadPin(DHT11PORT,DHT11PIN) == 0 && time_wait--);

		Delay_us(40);    	  

		if (HAL_GPIO_ReadPin(DHT11PORT,DHT11PIN) == 1)
		{
			time_wait = 25565;

			while(HAL_GPIO_ReadPin(DHT11PORT,DHT11PIN) == 1 && time_wait--);

			temp |= (uint8_t)(0x01 << (7 - i));
		}
		else
		{			   
			temp &= (uint8_t) ~ (0x01<<(7-i)); 
		}
	}
	return temp;
}

/*------------------------------*/
//0 means Success
//1 means Error
/*
     uint8_t DHT_Read(void)表达完整时序
*/
uint8_t Read_DHT11(DHT11_Data_TypeDef *DHT11_Data)
{
	unsigned char dht11temp[5] = {0};

	DHT_GPIO_SET_OUTPUT();
	HAL_GPIO_WritePin(DHT11PORT,DHT11PIN,GPIO_PIN_RESET);
	HAL_Delay(18);

	HAL_GPIO_WritePin(DHT11PORT,DHT11PIN,GPIO_PIN_SET);
	Delay_us(30);  
	DHT_GPIO_SET_INPUT();

	if(HAL_GPIO_ReadPin(DHT11PORT,DHT11PIN) == 0)     
	{
		time_wait = 25565;

		while(HAL_GPIO_ReadPin(DHT11PORT,DHT11PIN) == 0 && time_wait--);

		time_wait = 25565;

		while(HAL_GPIO_ReadPin(DHT11PORT,DHT11PIN) == 1 && time_wait--);

		dht11temp[0] = Read_Byte();

		dht11temp[1] = Read_Byte();

		dht11temp[2] = Read_Byte();

		dht11temp[3] = Read_Byte();

		dht11temp[4] = Read_Byte();

		DHT_GPIO_SET_OUTPUT();
		HAL_GPIO_WritePin(DHT11PORT,DHT11PIN,GPIO_PIN_SET);

		if (dht11temp[4] == dht11temp[3] + dht11temp[2] + dht11temp[1] + dht11temp[0] && (time_wait != 0) && dht11temp[0] != 0x00)
		{
			DHT11_Data -> humi_int = dht11temp[0];

			DHT11_Data -> humi_deci = dht11temp[1];

			DHT11_Data -> temp_int = dht11temp[2];

			DHT11_Data -> temp_deci = dht11temp[3];

			DHT11_Data -> check_sum = dht11temp[4];

			return SUCCESS;
		}
			
		else 
			return ERROR;
	}
	else
	{
		return ERROR;
	}   
	

	
}

