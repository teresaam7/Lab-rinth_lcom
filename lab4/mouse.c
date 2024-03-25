#include "mouse.h"

int hookIdMouse = 3;
uint8_t current, bIndex = 0; 
struct packet mPacket;
uint8_t mBytes[3];   

void (mouse_ih)(){
  if (read_scancode(W_CMD, &current, 1) != 0) 
        printf("Read scancode error\n");
}

int (mouse_subscribe_int)(uint8_t *bit_no){
    if (!bit_no) {
    return 1;
  } 
  *bit_no = BIT(hookIdMouse);
  if (sys_irqsetpolicy(MOUSE,IRQ_REENABLE | IRQ_EXCLUSIVE, &hookIdMouse) != 0) {
    return 1;
  }

  return 0;
}

int (mouse_unsubscribe_int)(){
  return sys_irqrmpolicy(&hookIdMouse);
}

int (mouse_write)(uint8_t command) {
    int i = 10;
    uint8_t status;
    do {
        i--;
        if (write_scancode(W_CMD, command) != 0) {
            return 1;
        } 
        if (write_scancode(IN_CMD, M_WRITE_BYTE) != 0) {
            return 1;
        } 
        tickdelay(micros_to_ticks(20000)); 
        if (util_sys_inb(W_CMD, &status) != 0) {
            return 1;
        }
        if (status == ACK) {
            return 0;
        }  
    } while (i && status != ACK);

    return 1;
}

void (packet_contruction)() {
  int i = 0;
  while (i < 3) {
    mPacket.bytes[i] = mBytes[i];
    ++i;
  }

  mPacket.lb = mBytes[0] & M_LB;                                                         // Byte 1 - lb
  mPacket.mb = mBytes[0] & M_MB;                                                         // Byte 1 - mb
  mPacket.rb = mBytes[0] & M_RB;                                                         // Byte 1 - rb
  mPacket.delta_x = (mBytes[0] & M_X_DELTA) ? (0xFF00 | mBytes[1]) : mBytes[1];          // Byte 1 - MSB X Delta
  mPacket.delta_y = (mBytes[0] & M_Y_DELTA) ? (0xFF00 | mBytes[2]) : mBytes[2];          // Byte 1 - MSB Y Delta
  mPacket.x_ov = mBytes[0] & M_X_OVFL;                                                   // Byte 1 - X ovfl
  mPacket.y_ov = mBytes[0] & M_Y_OVFL;                                                   // Byte 1 - Y ovfl
}

void (mouse_init) () {
  if ((bIndex == 0 && (current & BYTE1))) {
    mBytes[bIndex] = current;
    bIndex++;
  }
  else if (bIndex > 0) {
    mBytes[bIndex] = current;
    bIndex++;
  }
}



