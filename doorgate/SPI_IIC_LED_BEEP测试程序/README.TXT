实验器材:
	MiniSTM32F103开发板V3版本
	
实验目的:
	学习STM32 SPI接口的使用,驱动W25Q64实现SPI FLASH数据读写.
	
硬件资源:
	1,DS0(连接在PA8) 
	2,串口1(波特率:9600,PA9/PA10连接在板载USB转串口芯片CH340上面)
	3,ALIENTEK 2.8/3.5/4.3/7寸TFTLCD模块(通过GPIO驱动,连接关系见lcd.h)
	4,按键KEY0(PC5)/KEY_UP(PA0,也称之为WK_UP) 
	5,W25Q64(SPI FLASH芯片,连接在SPI1上)
		
实验现象:
	本实验通过KEY_UP按键来控制W25Q64的写入，通过另外一个按键KEY0来控制W25Q64的读取。并在LCD模块
	上面显示相关信息。DS0提示程序正在运行。同时，我们可以通过USMART控制读取W25QXX的ID或者整片擦除。
	
注意事项:  
	1,4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源供电(5V电源，接VOUT2即可).
	2,本例程在LCD_Init函数里面(在ILI93xx.c),用到了printf,如果不初始化串口1,将导致液晶无法显示!! 
	3,LCD模块在开发板的LCD接口上面，必须靠右插
	4,不要把1820和PA0的跳线帽跳上,否则可能导致按键"不灵".  
	5,W25Q64和NRF24L01/SD卡接口共用SPI1,所以他们必须分时复用,使用一个的时候,必须禁止另外两个的片选.
	  

					正点原子@ALIENTEK
					2014-3-09
					广州市星翼电子科技有限公司
					电话：020-38271790
					传真：020-36773971
					购买：http://shop62103354.taobao.com
					http://shop62057469.taobao.com
					公司网站：www.alientek.com
					技术论坛：www.openedv.com