#include "mouse.h"

int hookIdMouse = 3;
uint8_t current, bIndex = 0; 
struct packet mPacket;
uint8_t mBytes[3];   

void (mouse_ih)(){
  if (read_scancode(W_CMD, &current, 1) != 0) 
    printf("Error reading a byte from mouse\n");
  else 
    mouse_init();
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
    uint8_t status = NACK;
    while (i && status != ACK){
        if (write_scancode(IN_CMD, M_WRITE_BYTE) != 0) {
            return 1;
        }
        if (write_scancode(W_CMD, command) != 0) {
            return 1;
        }  
        tickdelay(micros_to_ticks(20000)); 
        if (util_sys_inb(W_CMD, &status) != 0) {
            return 1;
        }
        i--; 
    }

    if (status == ACK) {
        return 0;
    } 
    return 1;
}

void (packet_contruction)() {
  int i = 0;
  while (i < 3) {
    mPacket.bytes[i] = mBytes[i];
    ++i;
  }

  mPacket.lb = mBytes[0] & M_LB;
  mPacket.mb = mBytes[0] & M_MB;
  mPacket.rb = mBytes[0] & M_RB;
  if (mBytes[0] & M_X_DELTA)
      mPacket.delta_x = 0xFF00 | mBytes[1];
  else
      mPacket.delta_x = mBytes[1];
  if (mBytes[0] & M_Y_DELTA)
      mPacket.delta_y = 0xFF00 | mBytes[2];
  else
      mPacket.delta_y = mBytes[2];
  mPacket.x_ov = mBytes[0] & M_X_OVFL;
  mPacket.y_ov = mBytes[0] & M_Y_OVFL;
  bIndex = (bIndex == 3) ? 0 : bIndex;
}

void (mouse_init) () {
  if ((bIndex == 0 && (current & BYTE1)) || (bIndex > 0 && bIndex < 3)) {
    mBytes[bIndex] = current;
    bIndex++;
  }
}



