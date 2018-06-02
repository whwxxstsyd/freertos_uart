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





程序和协议对不上的地方
ReadCard_CardNoReport()










usart.c		硬件部分的初始化和串口的底层收发










uart_rx();//串口接收任务
	parse_protocol_line();
		tls_hostif_cmd_handler();
			tls_protocmd_parse();//协议数据解析
			tls_protocmd_exec();//协议命令执行
			tls_hostif_process_cmdrsp();//发送回复命令




tls_uart_init
	hif->uart_send_tx_msg_callback = uart_send_tx_msg;	
		//在这里将数据填充进列表，其实就相当于，将tx_event_msg_list内的数据复制给tx_msg_pending_list，再进行发送
		dl_list_add_tail(&uart_st[0].tx_msg_pending_list, &tx_msg->list);
		tls_os_mailbox_send(uart_st[0].tx_mailbox, (void *)MBOX_MSG_UART_TX);//发送信号量到tls_uart_x_tx_task()=>uart_tx()=>dl_list_first()将数据取出并发送
	

tx_msg_pending_list表示等待发送的数据列表由&tx_msg->list数据传入

uart_send_tx_msg_callback在以下函数中被调用	
1.tls_hostif_atcmd_loopback(将输入的数据再次显示在串口上)		NO
2.tls_hostif_process_cmdrsp(将任务执行完毕的回复信息输出)		YES
	tx_msg = tls_hostif_get_tx_event_msg(hif);//tx_msg由tls_hostif_get_tx_event_msg()获得，在传入下方的函数中
	hif->uart_send_tx_msg_callback(hostif_type, tx_msg);	
3.tls_hostif_send_event(用于特殊事件发生的时候传输)				NO
4.tls_hostif_tx_timeout(定时器中定时返回)						NO	
5.tls_hostif_recv_data(在原版的SDK中用于网络传输，不适用)		NO












//这两个函数内部有大小端置换的嫌疑
Ariber_GetArmyParam()
Ariber_GetAlarmParam()




//flash存储，hash关系
https://blog.csdn.net/u011855647/article/details/62215255

