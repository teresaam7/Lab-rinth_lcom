#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if(!lsb) return 1;
  *lsb = val & 0xFF;  
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if(!msb) return 1;
  val = val >> 8;
  *msb = val & 0xFF;
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  if(!value) return 1;
  uint32_t temp = 0;
  temp |= *value;
  if(sys_inb(port,&temp)!= 0) return 1;
  *value = temp & 0xFF;
  return 0;
}
