#include "myuart.h"

unsigned char text[128] = {0};
unsigned char wp = 0;
unsigned char tempw = 0;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    HAL_UART_Transmit(huart,&tempw,1,0xff);
    HAL_UART_Receive_IT(huart,&tempw,1);
}





