#ifndef MP_PROTOCOL_HOSTIF_H
#define MP_PROTOCOL_HOSTIF_H	
#include "sys.h"	
#include "list_base.h"
#include "mp_osal_rtos.h"		



/* host interface hardware mode, indicate which port used */
#define HOSTIF_MODE_UART0   	0
#define HOSTIF_MODE_UART1   	1	

#define HOSTIF_UART0_CMD   	1
#define HOSTIF_UART1_CMD   	0	


/* ri data format type definition */
#define PACKET_TYPE_DATA      0
#define PACKET_TYPE_AT_CMD    2

#define HOSTCMD_SYN      0xAA

#define HOSTIF_MSG_TYPE_EVENT  0
#define HOSTIF_MSG_TYPE_CMD    1
#define HOSTIF_MSG_TYPE_RSP    2

#define HOSTIF_TX_MSG_TYPE_EVENT       0	
#define HOSTIF_TX_MSG_TYPE_CMDRSP      1


struct tls_hostif_tx_msg {
    struct dl_list list;
    /* message type: HOSTIF_TX_MSG_XXX */
    u32 type;
    u32 time;
    u16 offset;
    union {
        struct msg_event_info {
            char  *buf;
            u32  buflen;
        } msg_event;	
        struct msg_cmdrsp_info {
            char *buf;
            u32 buflen;
        } msg_cmdrsp;
    } u;
};



#define HOSTIF_TX_MSG_NUM       40
#define HOSTIF_TX_EVENT_MSG_NUM  10

#define PROTOCMD_MAX_ARG     64	

//协议解析出来的接口
struct tls_protocmd_token_t {
	u8   VER;
	u8 	 ADDR;
	u8 	 CID1;
	u8 	 CID2;	
	u16  LENGTH;		
	u32  arg_found;	
    u8   arg[PROTOCMD_MAX_ARG];
	u16  CHKSUM;	
};	


typedef void (*hostif_send_tx_msg_callback)(u8 hostif_mode, struct tls_hostif_tx_msg *tx_msg, bool is_event);

struct tls_hostif {
	struct dl_list          tx_msg_list;	
	struct dl_list          tx_event_msg_list;

	hostif_send_tx_msg_callback uart_send_tx_msg_callback;
	tls_os_sem_t            *uart_atcmd_sem;

	/*  indicate use which port: SYS_HOSTIF_XXX */
	u8 hostif_mode;		
};


struct tls_hostif *tls_get_hostif(void);

int tls_hostif_init(void);	

int tls_hostif_cmd_handler(u8 hostif_cmd_type, char *buf, u32 length);


#endif


