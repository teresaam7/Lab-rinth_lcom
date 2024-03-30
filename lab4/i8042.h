#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define MOUSE_IRQ  12

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
#define MAKE_OR_BREAK      BIT(7) 


#define WRITE_BYTE_MOUSE   0xD4
#define ENABLE_DATA_MODE   0xF4
#define DISABLE_DATA_MODE  0xF5

#define ACK                0xFA
#define NACK               0xFE
#define ERROR              0xFC

#define LB                 BIT(0)
#define RB                 BIT(1)
#define MB                 BIT(2)

#define MSB_X_DELTA        BIT(4)
#define MSB_Y_DELTA        BIT(5)

#define X_OVFL             BIT(6)
#define Y_OVFL             BIT(7)

#endif
