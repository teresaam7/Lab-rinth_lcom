#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define OBF        BIT(0)   // Output buffer full - data available for reading
#define IBF        BIT(1)   // Input buffer full - no write
#define INH        BIT(4)   // Inhibit flag - 0 if keyboard is inhibited
#define AUX        BIT(5)   // Mouse data
#define TIMEOUT    BIT(6)   // Timeout error - invalid data
#define PARITY     BIT(7)   // Parity error - invalid data

#define STATUS_REG         0x64
#define KBC_CMD            0x64 
#define IN_BUF             0x60
#define OUT_BUF            0x60

#define SCAN_BREAK_ESC     0x81
#define SCAN_TWO_B         0xE0    // first byte of two byte scancodes 
#define MAKE_OR_BREAK      BIT(3)//0x08

// #define scancode(code)(code&BIT(7))
#endif
