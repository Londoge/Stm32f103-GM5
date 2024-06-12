#ifndef __MYIART_H_
#define __MYIART_H_

#include "usart.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);


#define rx_len 128
#define WHGM5_huart huart1

#endif
