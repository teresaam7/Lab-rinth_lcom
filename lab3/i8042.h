#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

//teorica the pcs keyboard 

#define KEYBOARD 1 

#define STAT_REG 0x64
#define BC_ESC   0x81
#define OUT_BUF  0X60
#define SIZE2_SCAN 0xE0
#define IN_CMD      0x64
#define R_CMD       0x20      //read command byte
#define W_CMD       0x60      //write command byte
#define BREAK_CODE BIT(7)
#define OBF       BIT(0)
#define IBF       BIT(1)
#define TIMEOUT   BIT(6)
#define PARITY    BIT(7)
#define AUX       BIT(5)  


#endif


