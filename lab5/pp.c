/*
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "graphic.h"
#include "timer.c"
#include "keyboard.h"

#define BIT(n) (1 << (n))
#define TEXT_MODE     0
#define INDEXED_MODE  1
#define M_1152        2
#define M_1280        3
#define M_800         4


typedef struct {
    uint16_t x_res; // horizontal resolution
    uint16_t y_res; // vertical resolution
    uint8_t bpp; // bits per pixel
    uint8_t r_sz; // red component size
    uint8_t r_pos; // red component LSB position
    uint8_t g_sz; // green component size
    uint8_t g_pos; // green component LSB position
    uint8_t b_sz; // blue component size
    uint8_t b_pos; // blue component LSB position
    phys_addr_t phys_addr; // video ram base phys address
} lpv_mode_info_t;

enum lpv_dir_t {
    lpv_hor, // horizontal line
    lpv_vert // vertical line
};

lpv_mode_info_t info;
uint8_t timer_hook_id;
char* video_mem;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (set_frame)(uint16_t mode) {
  //int r;
  //struct minix_mem_range mr;
  memset(&info, 0, sizeof(info));

  if (lpv_get_mode_info(mode, &info) != 0) {
    return 1;
  }

  unsigned numBytes = (info.bpp + 7)/ 8;
  uint8_t vram_size = info.x_res * info.y_res * numBytes;

  //mr.mr_base = info.phys_addr;
  //mr.mr_limit = mr.mr_base  + vram_size;

  video_mem = video_map_phys(info.phys_addr, vram_size);
  /*  

  if ( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))) {
      panic("sys_privctl (ADD_MEM) failed: %d\n", r);
      return 1;
  }

  if (video_mem == MAP_FAILED) {
      panic("couldn’t map video memory");
      return 1;
  }
  

  return 0;
}

int (draw_pixel) (uint16_t x, uint16_t y, uint32_t color) {
  if (x < 0 || y < 0 || x > info.x_res || y > info.y_res) {
    return 1;
  }

  unsigned numBytes = (info.bpp + 7)/8;
  uint32_t index = (y*info.x_res + x) * numBytes;

  if (memcpy(&video_mem[index], &color, numBytes) == NULL) {
    return 1;
  }

  return 0;
}

int (draw_hline) (uint16_t x, uint16_t y, uint16_t len, uint32_t color, enum lpv_dir_t dir) {
  if (dir == lpv_hor) {
    for (unsigned i = 0; i < len; i++) {
      if (draw_pixel(x+i, y, color) != 0) {
        return 1;
      }
    }
  }
  else if (dir == lpv_vert) {
    for (unsigned j = 0; j < len; j++) {
      if (draw_pixel(x, y+j, color) != 0) {
        return 1;
      }
    }
  }
  return 0;
}

int (draw_square) (uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  enum lpv_dir_t dir = lpv_hor;
  for (unsigned j = 0; j < height; j++) {
    if (draw_hline(x, y+j, width, color, lpv_hor) != 0) {
      vg_exit();
      return 1;
    }
  }
  return 0;
}


int pp_test_line(uint8_t mode, enum lpv_dir_t dir, uint16_t x,
uint16_t y, uint16_t len, uint32_t color, uint32_t delay) {

  if (mode < 1 || mode > 4) {
    return 1;
  }

  if (set_frame(mode) != 0) {
    return 1;
  }

  if (lpv_set_mode(mode) != 0) {
    return 1;
  }

  uint32_t color_, red, green, blue;

  if (mode == TEXT_MODE) {
    return 0;
  }
  
  else if (mode == M_1152 || mode == M_800 ) {
    red = color >> 16;
    green = (color >> 8) & 0xff;
    blue = color & 0xff;
    color_ = (red << info.r_pos) | (green << info.g_pos) | (blue << info.b_pos);
  }
  else if (mode == M_1280 ) {
    red = (color >> 11) & 0x1f;
    green = (color >> 5) & 0x3f;
    blue = color & 0x1f;
    color_ = (red << info.r_pos) | (green << info.g_pos) | (blue << info.b_pos);
  }

  
  else if (mode != 1) {
    red = ((1 << info.r_sz)-1) & (color >> info.r_pos);
    green = ((1 << info.g_sz)-1) & (color >> info.g_pos);
    blue = ((1 << info.b_sz)-1) & (color >> info.b_pos);
    color_ = (color << info.r_pos) | (color << info.g_pos) | (color << info.b_pos);
  }
  else {
    color_ = color;
  }

  if (draw_hline(x, y, len, color_, dir) != 0) {
    return 1;
  }

  sleep(delay);

  if (lpv_set_mode(TEXT_MODE) != 0) {
    return 1;
  }

  return 0;
}




uint16_t hres, vres;
static uint8_t *frame_buffer;
uint8_t bytesPerPixel;

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
    if (x < 0 || y < 0 || x >= hres || y >= vres) return 0;
    unsigned int index = (hres * y + x) * bytesPerPixel;
    if (memcpy(frame_buffer + index,&color, bytesPerPixel) != 0) return 1;
    return 0;
}
int pp_test_line(uint8_t mode, enum lpv_dir_t dir, uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint32_t delay) {
    if (mode < 1 || mode > 5) return 1;
    lpv_mode_info_t mode_info;
    if (lpv_get_mode_info((uint16_t) mode, &mode_info) != 0) return 1;
    if (lpv_set_mode((uint16_t) mode) != 0) return 1;
    uint32_t normalizedColor = 0;
    if (mode == 3){
        uint32_t red = ((1 << mode_info.r_sz) - 1) & (color >> mode_info.r_pos);
        uint32_t green = ((1 << mode_info.g_sz) - 1) & (color >> mode_info.g_pos);
        uint32_t blue = ((1 << mode_info.b_sz) - 1) & (color >> mode_info.b_pos);
        normalizedColor = (red << mode_info.r_pos) | (green << mode_info.g_pos) | (blue << mode_info.b_pos);
    }
    else{
        normalizedColor = color;
    }
    hres = mode_info.x_res;
    vres = mode_info.y_res;
    bytesPerPixel = (mode_info.bpp + 7)/8;
    frame_buffer = video_map_phys((uint32_t) mode_info.phys_addr, (size_t) (hres * vres * bytesPerPixel));
    if (frame_buffer == NULL) return 1;
    switch (dir)
    {
    case lpv_hor:
        for (uint16_t i = 0; i < len; i++){
            if (draw_pixel(x + i, y, normalizedColor) != 0) return 1;
        }
        break;
    case lpv_vert:
        for (uint16_t i = 0; i < len; i++){
            if (draw_pixel(x, y + i, normalizedColor) != 0) return 1;
        }
        break;
    default:
        return 1;
    }
    sleep(delay);
    if (lpv_set_mode(0) != 0) return 1;
    return 0;
}
*/




