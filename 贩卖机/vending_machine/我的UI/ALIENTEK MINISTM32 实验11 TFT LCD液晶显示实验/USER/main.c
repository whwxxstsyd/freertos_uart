#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "ugui.h"
#include "my_ui.h"	


UG_GUI GUI_Struct;


void gui_draw_line(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2,UG_COLOR c)
{
	POINT_COLOR = c;
	LCD_DrawLine(x1,y1,x2,y2);
}

void gui_fill_rectangle(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2,UG_COLOR c) 
{	
	POINT_COLOR = c;
	LCD_DrawRectangle(x1,y1,x2,y2);
}


void MiddleLayer_Init(void)		
{		
	//ugui 初始化
	////////////*******************************************************/
	UG_Init(&GUI_Struct, LCD_Fast_DrawPoint, 240, 240);			
	UG_SelectGUI(&GUI_Struct);		
	
	UG_DriverRegister(DRIVER_DRAW_LINE, (void *) gui_draw_line);
	UG_DriverRegister(DRIVER_FILL_FRAME, (void *) gui_fill_rectangle);
	UG_DriverEnable(DRIVER_DRAW_LINE);
	UG_DriverEnable(DRIVER_FILL_FRAME);	
		
	UG_FillScreen ( C_BLACK) ;											
}










		
int main(void)
{ 		
	delay_init();	    	 //延时函数初始化	  
	uart_init(9600);	 	//串口初始化为9600
	//usmart_dev.init(72); 	//初始化USMART		
	LED_Init();		  		//初始化与LED连接的硬件接口
	LCD_Init();				
	LCD_Display_Dir(1);		
	POINT_COLOR = WHITE;
	
	MiddleLayer_Init();			
		
	windows_create();		

	UG_Update();							

	while(1) 
	{		 	
		delay_ms(500);	
	} 
}



