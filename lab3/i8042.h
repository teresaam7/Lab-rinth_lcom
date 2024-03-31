#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

//slide 8 powerpoint 
//status register-> true(1) if set 
#define OBF        BIT(0) // Output buffer full - data available for reading
#define IBF        BIT(1) //Input buffer full don’t write commands or arguments
#define INH        BIT(4) //Inhibit flag: 0 if keyboard is inhibited
#define AUX        BIT(5) //Mouse data
#define TIMEOUT    BIT(6) //Timeout error - invalid data
#define PARITY     BIT(7) //Parity error - invalid data

//slide 7 e 17
#define STATUS_REG     0x64  //Read the KBC state
#define KBC_CMD_REG   0x64 //Commands to the KBC 
#define IN_BUF       0x60 //Arguments of KBC commands
#define OUT_BUF      0x60 //Read scancodes or return values from KBC commands

//slide 10
#define BREAK_ESC    0x81
#define SCAN_ONE_TWO 0xE0 //The first byte of two byte scancodes

#define KBD_IRQ 1
#define DELAY_US  20000 //20ms

#endif

