#include "myuart.h"

extern uint8_t rx_flag;
unsigned char text[rx_len] = {0};
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    
}

//事件中断
unsigned int indx;
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    indx = Size;
    HAL_UARTEx_ReceiveToIdle_IT(huart, text, 16);
    rx_flag = 1;
}





