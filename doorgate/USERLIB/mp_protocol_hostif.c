#include "mp_protocol_hostif.h"
#include "mp_uart_task.h"
#include "mp_mem.h"
#include "list_base.h"
#include "debug.h"	
#include "param.h"
#include "protocol.h"	
	
	
/* event, cmdrsp msg */
struct tls_hostif_tx_msg       g_hostif_tx_event_msg[HOSTIF_TX_EVENT_MSG_NUM];

struct tls_hostif g_hostif;		


static void tls_hostif_tx_timeout(void *parg);


struct tls_hostif *tls_get_hostif(void)
{
	return &g_hostif;
}		

		
int tls_hostif_init(void)
{
	struct tls_hostif *hif;		
	struct tls_hostif_tx_msg *tx_msg;
	int i;
	int err;

    hif= &g_hostif;
    memset(hif, 0, sizeof(struct tls_hostif));	
	
	dl_list_init(&hif->tx_event_msg_list);
	
    for (i = 0; i < HOSTIF_TX_EVENT_MSG_NUM; i++) 
	{	
        tx_msg = &g_hostif_tx_event_msg[i];	
        dl_list_add_tail(&hif->tx_event_msg_list, &tx_msg->list);
    }
		
	
#if 0	
	err = tls_os_timer_create(&hif->tx_timer,  
						60*HZ , 	
						pdTRUE, 		
						NULL, 		
						tls_hostif_tx_timeout); 	


    if (!err)			
        tls_os_timer_start(hif->tx_timer); 	

#endif 

	return err;	
}


#if 0
	
static void tls_hostif_tx_timeout(tls_os_timer_t timer)		
{	
    struct tls_hostif *hif = tls_get_hostif();		

	if (hif->hostif_mode == HOSTIF_MODE_UART0) 
	{
        if(hif->uart_send_tx_msg_callback != NULL)
            hif->uart_send_tx_msg_callback(HOSTIF_MODE_UART0, NULL);
    } 
	else if (hif->hostif_mode == HOSTIF_MODE_UART1) 	
    {
        if(hif->uart_send_tx_msg_callback != NULL)	
            hif->uart_send_tx_msg_callback(HOSTIF_MODE_UART1, NULL);	
    }
}

#endif


struct tls_hostif_tx_msg *tls_hostif_get_tx_event_msg(struct tls_hostif *hif)
{	
    u32 cpu_sr;
    struct tls_hostif_tx_msg *tx_msg;

    cpu_sr = tls_os_set_critical();
    if (dl_list_empty(&hif->tx_event_msg_list)) {
        tx_msg = NULL;
    } else {
        tx_msg = dl_list_first(&hif->tx_event_msg_list,
                struct tls_hostif_tx_msg, list);
        dl_list_del(&tx_msg->list);
    }
    tls_os_release_critical(cpu_sr); 

    return tx_msg; 
}
	

int tls_hostif_process_cmdrsp(u8 hostif_type, char *cmdrsp, u32 cmdrsp_size)
{
    struct tls_hostif_tx_msg *tx_msg;
    struct tls_hostif *hif = tls_get_hostif();
    		
    if (cmdrsp == NULL || cmdrsp_size == 0)
        return -1;	

    switch (hostif_type) 
	{
        case HOSTIF_MODE_UART0:
			//在链表中拿下一个来节点用作数据发送
            tx_msg = tls_hostif_get_tx_event_msg(hif);
            if (tx_msg == NULL) 
			{	
                return -1;
            }
            tx_msg->offset = 0;
            tx_msg->u.msg_cmdrsp.buf = cmdrsp;
            tx_msg->type = HOSTIF_TX_MSG_TYPE_CMDRSP;			
            tx_msg->u.msg_cmdrsp.buflen = cmdrsp_size;	

			if(hif->uart_send_tx_msg_callback != NULL)
			{		
				hif->uart_send_tx_msg_callback(hostif_type, tx_msg);	
			}	
			
            break;	
			
        case HOSTIF_MODE_UART1:		
            tx_msg = tls_hostif_get_tx_event_msg(hif);
            if (tx_msg == NULL)
                return -1;

            tx_msg->offset = 0;
            tx_msg->u.msg_cmdrsp.buf = cmdrsp;
            tx_msg->type = HOSTIF_TX_MSG_TYPE_CMDRSP;
            tx_msg->u.msg_cmdrsp.buflen = cmdrsp_size;

            if(hif->uart_send_tx_msg_callback != NULL)
                hif->uart_send_tx_msg_callback(hostif_type, tx_msg);	
            break;
			
        default:
            break;
    }
    return 0;
}



#define CMD_RSP_BUF_SIZE	64	

int tls_hostif_cmd_handler(u8 hostif_cmd_type, char *buf, u32 *len)
{			
    char *cmdrsp_buf;	
    u32 cmdrsp_size;
    struct tls_protocmd_token_t protocmd_tok;
    int err;	
    int i;		
    u8 hostif_type;		
	
    cmdrsp_size = CMD_RSP_BUF_SIZE;

    switch (hostif_cmd_type) 
	{	
        case HOSTIF_UART1_CMD:
        case HOSTIF_UART0_CMD:	
            if (hostif_cmd_type == HOSTIF_UART1_CMD)
                hostif_type = HOSTIF_MODE_UART1;	
            else
                hostif_type = HOSTIF_MODE_UART0;
			
            cmdrsp_buf = tls_mem_alloc(CMD_RSP_BUF_SIZE);
				
            if (!cmdrsp_buf)	
            {	
            	LOG_ERROR("cmdrsp_buf alloc failed\n");			
                return -1;	
			}
				
            memset(&protocmd_tok, 0, sizeof(struct tls_protocmd_token_t));
				
            err = tls_protocmd_parse(&protocmd_tok, buf, len);	

            if (err == RTN_CMDCHK_ERR) //校验错误
			{	
				LOG_ERROR("cmd check sum error\n");		
				//cmdrsp_buf = ;	
                //cmdrsp_size = ;
            } 	
			else if(err == RTN_VER_ERR)
			{	
				LOG_ERROR("return version err\n");		
				//cmdrsp_buf = ;
				//cmdrsp_size = ;	
			}
			else 
			{			
                cmdrsp_size = CMD_RSP_BUF_SIZE;
				
				//执行命令,发送回复
                err = tls_protocmd_exec(&protocmd_tok, cmdrsp_buf, &cmdrsp_size);
            }
            break;
				
        default:
            
			return -1;
            //break;
    }

	//将需要发送的信息重新组装	
	//tls_protocol_rebuild(&protocmd_tok,cmdrsp_buf,&cmdrsp_size);		
	
	//发送回复信息
    err = tls_hostif_process_cmdrsp(hostif_type, cmdrsp_buf, cmdrsp_size);
	
    if (err)
    {	
        tls_mem_free(cmdrsp_buf);
    }

    return err;

}



