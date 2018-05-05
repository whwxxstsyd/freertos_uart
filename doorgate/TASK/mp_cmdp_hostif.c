#include "mp_cmdp_hostif.h"
#include "list_base.h"	


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


