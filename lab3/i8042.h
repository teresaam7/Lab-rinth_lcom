#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define KEYBOARD       1

#define OBF            BIT(0) // Output buffer
#define IBF            BIT(1) // Input buffer
#define INH            BIT(4) // Inhibit flag
#define AUX            BIT(6) // Mouse data
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



#endif /* _LCOM_I8042_H_ */

