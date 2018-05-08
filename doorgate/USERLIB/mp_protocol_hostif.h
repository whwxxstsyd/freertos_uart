#ifndef MP_PROTOCOL_HOSTIF_H
#define MP_PROTOCOL_HOSTIF_H	
#include "sys.h"	



struct tls_atcmd_token_t {
    char   name[ATCMD_NAME_MAX_LEN];
    u32   op;
    char  *arg[ATCMD_MAX_ARG];
    u32   arg_found;
    enum  tls_cmd_mode cmd_mode;
};





#endif


