基于FreeRTOS的串口收发框架


框架:将串口分为三层
	
	
mp_protocol_hostif.c和protocol.c联系在一起，实现上层协议的解析，对相应功能的执行，通过信号量和mp_uart_task.c实现交互

	
	
	
	
mp_uart_task.c	负责上层协议通信
	
	tls_uart_port_init()硬件初始化
		tls_uart_open
			tls_uart_rx_register()注册串口接收中断回调(发送信号量)
			tls_uart_tx_register()注册串口发送中断回调(发送信号量)
			
			tls_uart_1_rx_task()串口总任务(接收信号量)
				uart_rx()
					parse_atcmd_line()解析发来的数据
						tls_hostif_cmd_handler()
							tls_cmd_exec()
								match->proc_func()执行命令对应的功能
			tls_uart_1_tx_task
				
	
	
mp_uart.c	收发缓冲区的中间部分









usart.c		硬件部分的初始化和串口的底层收发










uart_rx();//串口接收任务
	parse_protocol_line();
		tls_hostif_cmd_handler();
			tls_protocmd_parse();//协议数据解析
			tls_protocmd_exec();//协议命令执行
			tls_hostif_process_cmdrsp();//发送回复命令








