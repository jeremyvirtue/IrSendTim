#ifndef __IRSEND_H
#define __IRSEND_H

#include "main.h"

#define DATA_TATOL			8//发送红外数据个数
#define SEND_NUM			4//发送红外总次数


/*13us IR时间表*/
#define IR_START_HIGH_TIME	 692	//13us * x = 9ms  x =  
#define IR_START_LOW_TIME	 346	//13us * x = 4.5ms  x =  
#define IR_0_HIGH_TIME		 43		//13us * x = 560us
#define IR_0_LOW_TIME		 43		
#define IR_1_HIGH_TIME		 43
#define IR_1_LOW_TIME		 129	//13us * x = 1.68ms
#define IR_END_TIME			 1384

#define IR_H	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET)
#define IR_L	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET)
#define IR_T	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);

extern uint8_t buf[DATA_TATOL];

void IrSendData(void);
void IrOpen(void);
void IrClose(void);

#endif
