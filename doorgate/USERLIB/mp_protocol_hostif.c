#include "mp_protocol_hostif.h"
#include "list_base.h"
#include "debug.h"	


/* data msg */
struct tls_hostif_tx_msg       g_hostif_tx_msg[HOSTIF_TX_MSG_NUM];
/* event, cmdrsp msg */
struct tls_hostif_tx_msg       g_hostif_tx_event_msg[HOSTIF_TX_EVENT_MSG_NUM];

struct tls_hostif g_hostif;

struct tls_hostif *tls_get_hostif(void)
{
	return &g_hostif;
}	

		
int tls_hostif_init(void)
{
	
}



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


struct tls_hostif_tx_msg *tls_hostif_get_tx_msg(void)
{
    u32 cpu_sr;
    struct tls_hostif_tx_msg *tx_msg;

    cpu_sr = tls_os_set_critical();
    if (dl_list_empty(&g_hostif.tx_msg_list)) {
        tx_msg = NULL;
    } else {
        tx_msg = dl_list_first(&g_hostif.tx_msg_list,
                struct tls_hostif_tx_msg, list);
        dl_list_del(&tx_msg->list);
    }
    tls_os_release_critical(cpu_sr); 

    return tx_msg; 
}


int tls_hostif_recv_data(struct tls_hostif_tx_msg *tx_msg) 
{
	
}


int tls_hostif_cmd_handler(u8 hostif_cmd_type, char *buf, u32 length)
{

	#define CMD_RSP_BUF_SIZE    256

    char *cmdrsp_buf;
    u32 cmdrsp_size;
    struct tls_atcmd_token_t atcmd_tok;
    int err;
    int i, name_len;
    struct tls_hostif_hdr *hdr = (struct tls_hostif_hdr *)buf;
    u8 hostif_type;
	struct tls_hostif *hif = tls_get_hostif();

    cmdrsp_size = CMD_RSP_BUF_SIZE;

    switch (hostif_cmd_type) {
        case HOSTIF_UART1_AT_CMD:
        case HOSTIF_UART0_AT_CMD:
            if (hostif_cmd_type == HOSTIF_UART1_AT_CMD)
                hostif_type = HOSTIF_MODE_UART1_LS;
            else
                hostif_type = HOSTIF_MODE_UART0;

            cmdrsp_buf = tls_mem_alloc(CMD_RSP_BUF_SIZE);
            if (!cmdrsp_buf)		
                return -1;
            memset(&atcmd_tok, 0, sizeof(struct tls_atcmd_token_t));
            if (hostif_cmd_type == HOSTIF_UART0_AT_CMD)
                atcmd_tok.cmd_mode = CMD_MODE_UART0_ATCMD;
            else
                atcmd_tok.cmd_mode = CMD_MODE_UART1_ATCMD;

                err = tls_atcmd_parse(&atcmd_tok, buf+3, length - 3);

            if (err) 
			{
                cmdrsp_size = sprintf(cmdrsp_buf, "+ERR=%d\r\n", err);
            } 
			else 
			{
                name_len = strlen(atcmd_tok.name);
                for (i = 0; i < name_len; i++)
                    atcmd_tok.name[i] = toupper(atcmd_tok.name[i]);
                cmdrsp_size = CMD_RSP_BUF_SIZE;

                err = tls_atcmd_exec(&atcmd_tok, cmdrsp_buf, &cmdrsp_size);

            }
            break;
        default:
            
			return -1;
            //break;
    }

	//发送回复信息
    err = tls_hostif_process_cmdrsp(hostif_type, cmdrsp_buf, cmdrsp_size);
    if (err)
    {
        tls_mem_free(cmdrsp_buf);
    }

    return err;

}



