#include "IrSend.h" 

uint8_t buf[DATA_TATOL] = {0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00};//发送红外的数据

uint8_t ir_state = 0;	//状态标志位
uint8_t ir_sw;			//IR开关
uint16_t ir_time; 		//Ir计数器
uint8_t send_num = 0;   //发送数据次数计数器

uint8_t ir_bit = 0;		//位计数器
uint8_t ir_data = 0;;	//字节计数器

/*定时器处理*/
void IrSendData(void)
{ 
	switch(ir_state)
	{
		case 0:
		{
			if(ir_sw)
			{
				send_num++;
				IR_H;
				ir_time = 0;
				ir_state = 1;//跳转状态
			}
		}break;
		case 1:
		{
			IR_T;//38K载波信号
			ir_time++;
			if(ir_time >= IR_START_HIGH_TIME)//发送9ms载波
			{
				ir_time = 0;
				ir_state = 2;//跳转状态
			}
		}break;
		case 2:
		{
			IR_H;//无载波，此时接收为低电平
			ir_time++;
			if(ir_time >= IR_START_LOW_TIME)//
			{
				ir_time = 0;
				ir_state = 3;//跳转状态
			}
		}break;
		case 3:
		{
			if(ir_bit>7)//成功接收一个字节后
			{ 
				ir_bit = 0;
				ir_data++;//
				if(ir_data>=DATA_TATOL)//
				{
					ir_data = 0;
					ir_state = 4;//跳转状态
					return;
				}
			}
			ir_time++;
			if(ir_time<IR_0_HIGH_TIME)//发送560us载波
			{
				IR_T;
			}
			else//if(ir_time>IR_0_HIGH_TIME)
			{ 
				IR_H;//置高（无载波）
				if((buf[ir_data]>>ir_bit)&0x01)//
				{
					 if(ir_time > (IR_0_HIGH_TIME+IR_1_LOW_TIME))
					 {
						ir_time = 0;
						 ir_bit++;//移位
					 }
				}
				else
				{
					 if(ir_time > (IR_0_HIGH_TIME+IR_0_LOW_TIME))
					 {
						ir_time = 0;
						 ir_bit++;
					 }	
				}				
			}

		}break;
		case 4:
		{
			IR_H;
			ir_time++;
			if(ir_time>IR_END_TIME)//每成功发送一次间隔时间
			{
				ir_state = 0;//跳转状态
				ir_time = 0;
				if(send_num>=SEND_NUM)
				{
					send_num = 0;
					IrClose();
				}
			}
		}break;
	}
}

void IrOpen(void)
{
	ir_sw = 1;
}

void IrClose(void)
{
	ir_sw = 0;
}



