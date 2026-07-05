#include "Shadow.h"
#include "BaseFunc.h"
/*  ST库  */
#include "stm32f10x.h"
/*  OS库  */
#include "FreeRTOS.h"
#include "task.h"
/*  接口库  */
#include "Func.h"
/*  外设库  */
#include "U_USART1.h"
#include "A_ADC.h"
#include "TFT_ST7735.h"
#include "UI_DEF.h"
#include "TFT_font.h"
#include "P_PWM.h"
#include "R_RTC.h"

/**@brief  初始化线程
  */
void Start_MainTask(void* pvParameters)
{
	//启动内容
	Start_Func();
		//初始化函数-格式建议用Init_Xxx
	Init_Func();
	Init_ADC();
	Init_TFT();
	Init_UI();
	Init_PWM();
	
	UI_Write_String(0,0,"DC-DC",FONT_NI7SEG_2412,COLOR_WHITE,COLOR_BLACK,7);
	UI_Draw_Rect(0,24,80,4,COLOR_MOON);
	UI_Write_String(0,28+0,"V:",FONT_PIXEL_2412,COLOR_F,COLOR_B1,7);
	UI_Write_String(0,28+24,"I1:",FONT_PIXEL_2412,COLOR_F,COLOR_B2,7);
	UI_Write_String(0,28+48,"I2:",FONT_PIXEL_2412,COLOR_F,COLOR_B1,7);
	UI_Write_String(0,28+72,"I3:",FONT_PIXEL_2412,COLOR_F,COLOR_B2,7);
	UI_Write_String(0,28+96,"Ix:",FONT_PIXEL_2412,COLOR_F,COLOR_B1,7);
	UI_Write_String(0,52+96," ",FONT_PIXEL_2412,COLOR_F,COLOR_B2,7);
	
	//进入临界区
	taskENTER_CRITICAL();
		//线程函数-格式建议用Task_Xxx
	xTaskCreate(Task_Func,"Func",64,NULL,1,NULL);
//	xTaskCreate(Task_PWM,"PWM",64,NULL,1,NULL);
	xTaskCreate(Task_ADC,"ADC",128,NULL,1,NULL);
//	xTaskCreate(Task_RTC,"RTC",32,NULL,1,NULL);
	
	//退出临界区
	taskEXIT_CRITICAL();
	//删除自身函数
	vTaskDelete(NULL);
}

/**@brief  指令监听
  */
uint16_t output_pause = 50;
uint8_t Start_CommandFunc(void)
{
	if(Command("Start_CommandFunc"))
	{
		U_Printf("Command(\"COMMAND\")||Command(\"HELP\")\r\n");
	}
	//添加区
	else if(Command("COMMAND")||Command("HELP"))
	{
		U_Printf("这里是stm32f103c6t6的测试程序 \r\n");
		U_Printf("现在在整理库文件2026/6/23-12:54 \r\n");
	}
	else if(Command("A"))
	{
		if(output_pause<100)
		{
			output_pause++;
		}
		TIM_SetCompare3(TIM4,200+output_pause);
		U_Printf("增加到: %d\r\n",output_pause);
	}
	else if(Command("D"))
	{
		if(output_pause>0)
		{
			output_pause--;
		}
		TIM_SetCompare3(TIM4,200+output_pause);
		U_Printf("降低到: %d\r\n",output_pause);
	}
	else if(Command("C"))
	{
		for(int i=0;i<80;i++)
		{
			U_Printf("\r\n");
		}
		U_Printf("清理屏幕 \r\n");
	}
	
	//结束
	else
	{
		return 0;
	}
	return 1;
}

