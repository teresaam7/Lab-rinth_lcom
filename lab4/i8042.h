#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KEYBOARD       1
#define MOUSE          12

#define OBF            BIT(0) // Output buffer
#define IBF            BIT(1) // Input buffer
#define INH            BIT(4) // Inhibit flag
#define AUX            BIT(5) // Mouse data
#define TIMEOUT        BIT(6) // Timeout error - invalid data
#define  PARITY        BIT(7) // Parity error - invalid data

#define BC_ESC         0x81   // Break code of ESC
#define SCAN_1OF2      0xE0   // Code for scan
#define R_CMD          0x20   // Read back command
#define W_CMD          0x60   // Write command

#define STAT_REG       0x64
#define IN_CMD         0x64
#define IN_BUF         0x60
#define OUT_BUF        0x60
#define MAKEORBREAK    0x80

#define M_LB            BIT(0)  // Mouse left button
#define M_RB            BIT(1)  // Mouse right button
#define M_MB            BIT(2)  // Mouse middle button
#define BYTE1           BIT(3)  // Byte 
#define M_X_DELTA       BIT(4)  // Relative displacement MSBit X axis
#define M_Y_DELTA       BIT(5)  // Relative displacement MSBit Y axis
#define M_X_OVFL        BIT(6)  // Flag that the mouse displacement is to large
#define M_Y_OVFL        BIT(7)  // Flag that the mouse displacement is to large


#define RESET           0xFF
#define RESEND          0xFE
#define DISABLE         0xF5
#define ENABLE_STRMODE  0xF4
#define SET_STRMODE     0xEA
#define M_READ          0xEB
#define M_WRITE_BYTE    0xD4
#define ACK             0xFA
#define NACK            0xFE
#define ERROR           0xFC




#endif /* _LCOM_I8042_H_ */

