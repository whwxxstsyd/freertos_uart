#ifndef MP_PROTOCOL_HOSTIF_H
#define MP_PROTOCOL_HOSTIF_H	
#include "sys.h"	



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



struct tls_hostif_hdr {
    u8      sync;
    u8      type;
    u16     length;
    u8      seq_num;
    u8      flag;
    u8      dest_addr;
    u8      chk;
};


struct tls_protocmd_token_t {
    char   name;	
    u32   op;
    char  *arg[PROTOCMD_MAX_ARG];
    u32   arg_found;		
};	


typedef void  (*hostif_send_tx_msg_callback)(u8 hostif_mode, struct tls_hostif_tx_msg *tx_msg, bool is_event);

struct tls_hostif {
    tls_os_timer_t          *tx_timer;
    struct dl_list          tx_msg_list;
    struct dl_list          tx_event_msg_list;

    hostif_send_tx_msg_callback uart_send_tx_msg_callback;
    tls_os_sem_t            *uart_atcmd_sem;

    u8 uart0_port_set;
    u8 uart1_port_set;

    u8   uart1_atcmd_snd_skt;
    u8   uart1_atcmd_rec_skt;

    u8 last_scan;
    enum tls_cmd_mode last_scan_cmd_mode;
    u8 last_join;
    enum tls_cmd_mode last_join_cmd_mode;

    /*  indicate use which port: SYS_HOSTIF_XXX */
    u8 hostif_mode;

    u32 uart_atlt;
    u32 uart_atpt;
    /* uart at cmd loopback control */
    u8 uart_insdisp;
    u8 reserved[3];
	
	/*uart escape*/
    u8  escape_char;
    u8  escape_reserved;
    u16 escape_pt;

    u8 hspi_tx_seq;
	u8 rptmode; /*0:host inquire, 1:auto report*/
    u8 reserved1[2];
};




#endif


