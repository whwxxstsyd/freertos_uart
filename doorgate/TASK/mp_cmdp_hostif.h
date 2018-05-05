#ifndef MP_CMDP_HOSTIF_H
#define MP_CMDP_HOSTIF_H	
#include <stdbool.h>	
#include "sys.h"	
#include "list_base.h"	
#include "mp_osal_rtos.h"	



enum tls_cmd_mode {
    CMD_MODE_HSPI_ATCMD,
    CMD_MODE_HSPI_RICMD,
    CMD_MODE_UART0_ATCMD,
    CMD_MODE_UART1_ATCMD,
    CMD_MODE_UART1_RICMD,
    CMD_MODE_INTERNAL,
};

#define	DEVICE_TYPE_1 0x80
#define	DEVICE_TYPE_2 0x90
#define	DEVICE_TYPE_3 0xD0



#define HOSTIF_TX_MSG_NUM       40
#define HOSTIF_TX_EVENT_MSG_NUM  10

#define HOSTIF_UART0_CMD   0	
#define HOSTIF_UART1_CMD   1




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




typedef void  (*hostif_send_tx_msg_callback)(u8 hostif_mode, struct tls_hostif_tx_msg *tx_msg, bool is_event);

struct tls_hostif {		
    struct dl_list          tx_msg_list;
    struct dl_list          tx_event_msg_list;
    //struct tls_hspi         *hspi;
    //struct tls_uart    *uart0;
    //struct tls_uart    *uart1;
    hostif_send_tx_msg_callback hspi_send_tx_msg_callback;
    hostif_send_tx_msg_callback uart_send_tx_msg_callback;
    tls_os_sem_t            uart_atcmd_sem;
    u8 hspi_port_set;	
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





int tls_hostif_init(void);
struct tls_hostif *tls_get_hostif(void);



#endif

