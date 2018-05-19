#ifndef    __PARAM_H__
#define     __PARAM_H__
#include "sys.h"

#define WM_SUCCESS		0
#define WM_FAILED		-1

#define SDK_OK  1
#define SDK_ERR	 -1			

#define CFG_ON  1
#define CFG_OFF 0

#define TRUE				1
#define FALSE				0

#define true				1
#define false				0


/* error code */
#define CMD_ETRUERR_OK          0
#define CMD_ERR_INV_FMT         1
#define CMD_ERR_UNSUPP          2
#define CMD_ERR_OPS             3
#define CMD_ERR_INV_PARAMS      4
#define CMD_ERR_NOT_ALLOW       5
#define CMD_ERR_MEM             6
#define CMD_ERR_FLASH           7
#define CMD_ERR_BUSY            8
#define CMD_ERR_SLEEP           9
#define CMD_ERR_JOIN            10
#define CMD_ERR_NO_SKT          11
#define CMD_ERR_INV_SKT         12
#define CMD_ERR_SKT_CONN        13
#define CMD_ERR_LEN_CMP         14
#define CMD_ERR_UNDEFINE        64



#define ONENET_AT_CMD_NOT_RESP    -0xfffff


#endif

