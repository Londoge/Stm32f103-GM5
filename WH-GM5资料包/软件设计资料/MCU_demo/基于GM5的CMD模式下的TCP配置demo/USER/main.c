/********************************************************************************
* @File name: main.c
* @Author: zhaipengju
* @Version: 1.0
* @Date: 2021-5-18
* @Description: GM5 CMD_mode TCP test demo
********************************************************************************/                      


#include <string.h> 
#include <stdio.h> 
#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "sys.h" 
#include "key.h" 


/*-----------------------------��������------------------------------*/
char received_house[256]; //���ݽ�������
int house_num = 0;        //���ݽ����������
uint8_t SEND_ARR[207];    //���ݷ�������

   

/*-----------------------------����ʵ��------------------------------*/
/*******************************************************
*
* Function name      : UART1 initialization Function
* Description        : UART1 initialization
* Parameter          : NONE
* Return             : NONE
**********************************************************/
void my_usart1_init()
{
	GPIO_InitTypeDef GPIO_Initstrue1; //����GPIO�ṹ��
	USART_InitTypeDef USART_Initstrue1; //���崮��1�ṹ��
	NVIC_InitTypeDef NVIC_Initstrue1; //�����жϲ���
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��A��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //ʹ�ܴ���1ʱ��
	
	GPIO_Initstrue1.GPIO_Pin = GPIO_Pin_9; //���ô���1��tx��A��9�ţ�
	GPIO_Initstrue1.GPIO_Mode = GPIO_Mode_AF_PP; //Ϊ��������	
	GPIO_Initstrue1.GPIO_Speed = GPIO_Speed_50MHz; //ʱ��50mhz
	GPIO_Init(GPIOA,&GPIO_Initstrue1);  //д��
	  
	GPIO_Initstrue1.GPIO_Pin = GPIO_Pin_10; //���ô���1��rx��A��10�ţ�
	GPIO_Initstrue1.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//Ϊ��������
	GPIO_Initstrue1.GPIO_Speed = GPIO_Speed_50MHz; //ʱ��50mhz
	GPIO_Init(GPIOA,&GPIO_Initstrue1); //д��
	
	USART_Initstrue1.USART_BaudRate = 115200; //������115200
	USART_Initstrue1.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������
	USART_Initstrue1.USART_Mode = USART_Mode_Rx|USART_Mode_Tx; //���ͽ���ʹ��
	USART_Initstrue1.USART_Parity = USART_Parity_No; //����żУ��
	USART_Initstrue1.USART_StopBits = USART_StopBits_1; //ֹͣλ1
	USART_Initstrue1.USART_WordLength = USART_WordLength_8b; //��Ч���ݰ�λ 
	USART_Init(USART1,&USART_Initstrue1); //���ڳ�ʼ��
	
	USART_Cmd(USART1,ENABLE);  //����1ʹ��
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE); //ʹ�ܽ��շǿ��ж�
	
	NVIC_Initstrue1.NVIC_IRQChannel = USART1_IRQn; //�ж�ͨ������Ϊ����1
	NVIC_Initstrue1.NVIC_IRQChannelCmd = ENABLE; //�����ж�ͨ��
	NVIC_Initstrue1.NVIC_IRQChannelPreemptionPriority = 1; //�����ж����ȼ�Ϊ1
	NVIC_Initstrue1.NVIC_IRQChannelSubPriority = 1; //���������ȼ�Ϊ1
	NVIC_Init(&NVIC_Initstrue1);
}

/*******************************************************
*
* Function name      : UART1 interrupt handle Function
* Description        : UART1 interrupt handle
* Parameter          : NONE
* Return             : NONE
**********************************************************/
void USART1_IRQHandler()
{
	u8 res;
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		 res = USART_ReceiveData(USART1);
		 USART_SendData(USART1,res);
	}
}

/*******************************************************
*
* Function name      : UART2 initialization
* Description        : UART2 initialization
* Parameter          : NONE
* Return             : NONE
**********************************************************/
void my_usart2_init()
{
	GPIO_InitTypeDef GPIO_Initstrue2; //����GPIO�ṹ��
	USART_InitTypeDef USART_Initstrue2; //���崮��2�ṹ��                                                                                                                                    
	NVIC_InitTypeDef NVIC_Initstrue2; //�����жϲ���
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //ʹ��A��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  //ʹ�ܴ���2ʱ��
  
	GPIO_Initstrue2.GPIO_Pin = GPIO_Pin_2; //���ô���2��tx��A��2�ţ�
	GPIO_Initstrue2.GPIO_Mode = GPIO_Mode_AF_PP; //Ϊ��������	
	GPIO_Initstrue2.GPIO_Speed = GPIO_Speed_50MHz; //ʱ��50mhz
	GPIO_Init(GPIOA,&GPIO_Initstrue2); //д��
	
	GPIO_Initstrue2.GPIO_Pin = GPIO_Pin_3; //���ô���2��rx��A��3�ţ�
	GPIO_Initstrue2.GPIO_Mode = GPIO_Mode_IN_FLOATING; //Ϊ��������
	GPIO_Initstrue2.GPIO_Speed = GPIO_Speed_50MHz; //ʱ��50mhz
	GPIO_Init(GPIOA,&GPIO_Initstrue2); //д��
	
	USART_Initstrue2.USART_BaudRate = 115200; //������115200
	USART_Initstrue2.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������
	USART_Initstrue2.USART_Mode = USART_Mode_Rx|USART_Mode_Tx; //���ͽ���ʹ��
	USART_Initstrue2.USART_Parity = USART_Parity_No; //����żУ��
	USART_Initstrue2.USART_StopBits = USART_StopBits_1; //ֹͣλ1
	USART_Initstrue2.USART_WordLength = USART_WordLength_8b; //��Ч���ݰ�λ
	USART_Init(USART2,&USART_Initstrue2); //���ڳ�ʼ��
	USART_Cmd(USART2,ENABLE); //����2ʹ��
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE); //ʹ�ܽ��շǿ��ж�
	
	NVIC_Initstrue2.NVIC_IRQChannel = USART2_IRQn; //�ж�ͨ������Ϊ����2
	NVIC_Initstrue2.NVIC_IRQChannelCmd =  ENABLE; //�����ж�ͨ��
	NVIC_Initstrue2.NVIC_IRQChannelPreemptionPriority = 1;//�����ж����ȼ�Ϊ1
	NVIC_Initstrue2.NVIC_IRQChannelSubPriority = 1;  //���������ȼ�Ϊ1
	NVIC_Init(&NVIC_Initstrue2);
}

/*******************************************************
*
* Function name      : UART2 interrupt handle Function
* Description        : UART2 interrupt handle
* Parameter          : NONE
* Return             : NONE
**********************************************************/
void USART2_IRQHandler()
{
	if(USART_GetITStatus(USART2,USART_IT_RXNE))
	{
		received_house[house_num] = USART_ReceiveData(USART2);
		house_num++;
		if(house_num==255)
			house_num = 0;
	}
}

/*******************************************************
*
* Function name      : UART Byte Send Function
* Description        : UART Byte Send
* Parameter          : 
*                      @pUSARTx     Type of USART(USART1 or USART2)
*                      @Send_byte   8 bit data will send by USART  
* Return             : NONE
**********************************************************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch )
{
	USART_SendData(pUSARTx,ch); //����һ���ֽ����ݵ�USART1 
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET); //�ȴ�������� 
}

/*******************************************************
*
* Function name      : UART string Send Function
* Description        : UART string Send
* Parameter          : 
*                      @pUSARTx     Type of USART(USART1 or USART2)
*                      @str         addr of data that will send by USART  
* Return             : NONE
**********************************************************/
void Usart_SendString( USART_TypeDef * pUSARTx, uint8_t *str)
{
	unsigned int k=0;
    do 
    {
        Usart_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');
}

/*******************************************************
*
* Function name      : Led twinkle Function
* Description        : Led twinkle
* Parameter          : NONE
* Return             : NONE
**********************************************************/
void led_circle()
{
	LED1=0;
	delay_ms(100);
	LED1=1;
	delay_ms(100);
}

/*******************************************************
*
* Function name      : Received Arr clear Function
* Description        : Received Arr clear
* Parameter          : NONE
* Return             : NONE
**********************************************************/
void clear_received_house()
{
	int i;
	for(i=0;i<=255;i++)
	{
		received_house[i]=0;
	}
	house_num = 0;
}

/*******************************************************
*
* Function name      : Modul Power on information handle  Function
* Description        : Modul Power on information handle
* Parameter          : NONE
* Return             : NONE
**********************************************************/
void start_handle()
{
	while(strstr(received_house,"GM5")==NULL);
	clear_received_house();
	led_circle();
	printf("Modul Power on success\r\n");
}

/*******************************************************
*
* Function name      : Change Modul mode is CMD Function
* Description        : Change Modul mode is CMD
* Parameter          : NONE
* Return             : NONE
**********************************************************/
void CMDmod_change() 
{
		Usart_SendString(USART2,"+++");
		delay_ms(200);
		Usart_SendString(USART2,"a");
		delay_ms(200);
		if(strstr(received_house,"ok")!=NULL) //NETģʽ�½������óɹ�
		{
			printf("Current mode is net mode\r\n");
			printf("Enter AT config success(net mode)\r\n");
			clear_received_house();
			Usart_SendString(USART2,"AT+WKMOD=CMD\r\n");
			while(strstr(received_house,"OK")==NULL);
			clear_received_house();
			Usart_SendString(USART2,"AT+S\r\n");
			while(strstr(received_house,"OK")==NULL);
			clear_received_house();
			led_circle();
			start_handle();
		}
		else //��CMDģʽ�·�+++a��֮���ٷ�ATָ���ʱ���״η��Ϳ��ܻᱨ��
				 //������﷢����ATָ���ָ֤�������
		{
			printf("Current mode is cmd mode\r\n");
			clear_received_house();
			Usart_SendString(USART2,"AT\r\n");
			delay_ms(200);
			clear_received_house();
			Usart_SendString(USART2,"AT+S\r\n");
			while(strstr(received_house,"OK")==NULL);
			clear_received_house();
			led_circle();
			start_handle();
		}
}

/*******************************************************
*
* Function name      : NET And TCP config Function
* Description        : NET And TCP config
* Parameter          : NONE
* Return             : NONE
**********************************************************/
void TCP_config() //TCP����
{
	Usart_SendString(USART2,"AT+CEREG?\r\n");
	while(strstr(received_house,"+CEREG: 1, 1")==NULL); //��ѯ4G����ע��
	clear_received_house();
	printf("CMD_Mode network registry success\r\n");
	
	Usart_SendString(USART2,"AT+CGATT?\r\n");
	while(strstr(received_house,"+CGATT:1")==NULL); //��ѯ���總��
	clear_received_house();
	printf("CMD_Mode network attachment success\r\n");
	
	Usart_SendString(USART2,"AT+CGACT=1,1\r\n");
	while(strstr(received_house,"+CGACT: 1")==NULL); //������缤��
	clear_received_house();
	printf("CMD_Mode network active success\r\n");
	
	Usart_SendString(USART2,"AT+CSTT\r\n"); //ʹ��APN����
	while(strstr(received_house,"OK")==NULL);
	clear_received_house();
	printf("CMD_Mode enable APN parameter\r\n");
	
	Usart_SendString(USART2,"AT+CIICR\r\n"); //�����ƶ�����
	while(strstr(received_house,"OK")==NULL);
	clear_received_house();
	printf("CMD_Mode activate mobile scene success\r\n");
	
	Usart_SendString(USART2,"AT+CIPSTART=\"TCP\",\"47.93.32.32\",9999\r\n"); //TCP����д��
	while(strstr(received_house,"CONNECT OK")==NULL);
	clear_received_house();
	printf("CMD_Mode TCP parameter config\r\n");
	
	led_circle();
	
}

/*******************************************************
*
* Function name      : TCP send Function
* Description        : TCP send
* Parameter          : NONE
* Return             : NONE
**********************************************************/
void TCP_send()
{
		Usart_SendString(USART2,"AT+CIPSTATUS\r\n");
		while(strstr(received_house,"STATE:CONNECT OK")==NULL);
		printf("CMD_Mode TCP connect status is ok\r\n");
	
		Usart_SendString(USART2,"AT+CIPSEND\r\n"); //
		while(strstr(received_house,">")==NULL);
		clear_received_house();
		delay_ms(200);
		Usart_SendString(USART2,SEND_ARR); //TCP����д��
		delay_ms(200);
		clear_received_house();
		Usart_SendByte(USART2,0x1a);
		while(strstr(received_house,"SEND OK")==NULL);
		printf("CMD_Mode TCP send ok\r\n");
		clear_received_house();
}

/*******************************************************
*
* Function name      : MAIN Function
* Description        : MAIN
* Parameter          : NONE
* Return             : NONE
**********************************************************/
int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //��ռ����Ӧ���ȼ�Ϊ2
	my_usart1_init(); //���õ�����1��ע��ȥ������������1�жϷų�����
	KEY_Init(); //������ʼ��
	my_usart2_init(); //����2��ʼ��
	LED_Init(); //led��ʼ��
	delay_init(72);	 //��ʱ��ʼ��
	delay_ms(200);
	printf("System init success\r\n");
	memset(SEND_ARR, 0XAA, sizeof(SEND_ARR));
	printf("Data load success\r\n");
	
	start_handle();
	CMDmod_change();
	while(strstr(received_house,"+CIEV:")==NULL);
	clear_received_house();
	TCP_config();
	
	while(1)
	{
		TCP_send();
	}					 
}


