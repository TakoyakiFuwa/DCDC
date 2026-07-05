#include "Button.h"
/*  ST库  */
#include "stm32f10x.h"
/*  OS库  */
#include "FreeRTOS.h"
#include "task.h"
/*  外设库  */
#include "U_USART1.h"

void Init_Button(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
}
extern uint16_t output_pause;
void B_Add(void)
{
	if(output_pause<100)
	{
		output_pause++;
	}
	TIM_SetCompare3(TIM4,200+output_pause);
	U_Printf("增加到: %d\r\n",output_pause);
}
void B_Mul(void)
{
	if(output_pause>0)
	{
		output_pause--;
	}
	TIM_SetCompare3(TIM4,200+output_pause);
	U_Printf("降低到: %d\r\n",output_pause);
}
void Task_Button(void* pvParameters)
{
	uint16_t count = 0;
	while(1)
	{
		vTaskDelay(20);
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==Bit_RESET)
		{
			vTaskDelay(10);
			B_Add();
			while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==Bit_RESET)
			{
				vTaskDelay(100);
				count++;
				if(count>=10)
				{
					B_Add();
				}
			}
			vTaskDelay(10);
			count = 0;
		}
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==Bit_RESET)
		{
			vTaskDelay(10);
			B_Mul();
			while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==Bit_RESET)
			{
				vTaskDelay(100);
				count++;
				if(count>=10)
				{
					B_Mul();
				}
			}
			vTaskDelay(10);
			count = 0;
		}
	}
	
}


