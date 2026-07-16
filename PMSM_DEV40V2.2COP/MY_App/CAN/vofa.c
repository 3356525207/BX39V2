#include "wk_usart.h"
#include  "vofa.h"
#include "arm_math.h"
float tempFloat[8];                
uint8_t tempData[28];

//发送数组
void send_array(uint8_t *array, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        usart_data_transmit(USART1, array[i]);
        while (RESET==	usart_flag_get(USART1 ,USART_TDC_FLAG));
    }
}

//vofa发送函数
void vofa_send(void)
{

//    tempFloat[0] = (float)0;
//    tempFloat[1] = (float)0;
//    tempFloat[2] = (float)0;
//    tempFloat[3] = (float)0;
//    tempFloat[4] = (float)0;
//    tempFloat[5] = (float)0;

    //将浮点数转换为字节数组
    memcpy(tempData, tempFloat, sizeof(tempFloat));

    tempData[24] = 0x00; //帧头
    tempData[25] = 0x00; //帧头
    tempData[26] = 0x80; //帧头
    tempData[27] = 0x7f; //帧头

    //发送数据
    send_array(tempData, 28);
}