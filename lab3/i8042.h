#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

//teorica the pcs keyboard 

#define KEYBOARD    1

//-----------------slide 8-----------------
#define OBF BIT(0)          //output buffer
#define IBF BIT(1)          //input buffer
#define INH BIT(4)          //inhibit flag
#define AUX BIT(5)          //mouse data
#define TIMEOUT BIT(6)      //timeout error - invalid data
#define PARITY BIT(7)       //parity error - invalid data

//-----------------slide 7-----------------
#define IN_CMD      0x64
#define STAT_REG    0x64
#define IN_BUFF     0x60
#define OUT_BUFF    0x60
#define MAKEORBREAK 0x80


//-----------------slide 10----------------
#define BC_ESC       0x81     //breakcode esc
#define SCAN_1OF2    0xE0     // 1st byte of scancode two bytes

//-----------------slide 14----------------
#define R_CMD       0x20      //read command byte
#define W_CMD       0x60      //write command byte

#endif


