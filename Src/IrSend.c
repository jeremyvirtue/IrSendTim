#include "IrSend.h" 

uint8_t buf[DATA_TATOL] = {0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00};//���ͺ��������

uint8_t ir_state = 0;	//״̬��־λ
uint8_t ir_sw;			//IR����
uint16_t ir_time; 		//Ir������
uint8_t send_num = 0;   //�������ݴ���������

uint8_t ir_bit = 0;		//λ������
uint8_t ir_data = 0;;	//�ֽڼ�����

/*��ʱ������*/
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
				ir_state = 1;//��ת״̬
			}
		}break;
		case 1:
		{
			IR_T;//38K�ز��ź�
			ir_time++;
			if(ir_time >= IR_START_HIGH_TIME)//����9ms�ز�
			{
				ir_time = 0;
				ir_state = 2;//��ת״̬
			}
		}break;
		case 2:
		{
			IR_H;//���ز�����ʱ����Ϊ�͵�ƽ
			ir_time++;
			if(ir_time >= IR_START_LOW_TIME)//
			{
				ir_time = 0;
				ir_state = 3;//��ת״̬
			}
		}break;
		case 3:
		{
			if(ir_bit>7)//�ɹ�����һ���ֽں�
			{ 
				ir_bit = 0;
				ir_data++;//
				if(ir_data>=DATA_TATOL)//
				{
					ir_data = 0;
					ir_state = 4;//��ת״̬
					return;
				}
			}
			ir_time++;
			if(ir_time<IR_0_HIGH_TIME)//����560us�ز�
			{
				IR_T;
			}
			else//if(ir_time>IR_0_HIGH_TIME)
			{ 
				IR_H;//�øߣ����ز���
				if((buf[ir_data]>>ir_bit)&0x01)//
				{
					 if(ir_time > (IR_0_HIGH_TIME+IR_1_LOW_TIME))
					 {
						ir_time = 0;
						 ir_bit++;//��λ
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
			if(ir_time>IR_END_TIME)//ÿ�ɹ�����һ�μ��ʱ��
			{
				ir_state = 0;//��ת״̬
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



