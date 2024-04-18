// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include "graphic.h"
#include "keyboard.h"

extern uint8_t scancode;
extern int counter;
extern vbe_mode_info_t info;

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


int (receive_ESC)() {
  int ipc_status, r;
  message msg;
  uint8_t irq_set_kbc;
  
  if (keyboard_subscribe_int(&irq_set_kbc) != 0) {
    return 1;
  }

  while (scancode != BC_ESC) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: // hardware interrupt notification
          if (msg.m_notify.interrupts & irq_set_kbc) { // subscribed interrupt
            kbc_ih();
          }
          break;
        default:
          break; // no other notifications expected: do nothing
      }
    }
  }

  if (keyboard_unsubscribe_int() != 0) {
    return 1;
  }

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (videocard_graphic_mode(mode) != 0) {
    return 1;
  }
  
  sleep(delay);
  if (vg_exit() != 0) {
    printf("Could not close video memory\n");
    return 1;
  }

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

  if (frame(mode) != 0) {
    return 1;
  }

  if (videocard_graphic_mode(mode) != 0) {
    return 1;
  }

  uint32_t newColor;
  if (color_normalization(color, &newColor) != 0) {
    return 1;
  }

  if (vg_draw_rectangle(x, y, width, height, color) != 0) {
    return 1;
  }

  if (receive_ESC() != 0) {
    return 1;
  }

  if (vg_exit() != 0) {
    printf("Could not close video memory\n");
    return 1;
  }

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if (frame(mode) != 0) {
    return 1;
  }

  if (videocard_graphic_mode(mode) != 0) {
    return 1;
  }

  int height = info.YResolution / no_rectangles;
  int width = info.XResolution / no_rectangles;

  for  (int i = 0; i < no_rectangles; ++i) {
    for (int j = 0; j < no_rectangles; i++) {
      uint32_t color;
      if (info.MaxPixelClock == DIRECT_COLOR) {
        uint32_t red = Red(j, step, first);
        uint32_t green = Green(i, step, first);
        uint32_t blue = Blue(i, j, step, first);
        color = (red << info.RedFieldPosition) | (green << info.GreenFieldPosition) | (blue << info.BlueFieldPosition);
      }
      else {
        color = color_mode_indexed(j, i, step, first, no_rectangles);
      }
      if (vg_draw_rectangle(j*width, i*height, width, height, color) != 0) {
        return 1;
      }
    }
  }

  if (receive_ESC() != 0) {
    return 1;
  }

  if (vg_exit() != 0) {
    printf("Could not close video memory\n");
    return 1;
  }
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  uint16_t mode = VBE_1024x768;  // Default to a common resolution
  if (frame(mode) != 0) {
    return 1;
  }

  if (videocard_graphic_mode(mode) != 0) {
    return 1;
  }
  
  if (make_xpm(xpm, x, y) != 0) {
    return 1;
  }
  
  if (receive_ESC() != 0) {
    return 1;
  }

  if (vg_exit() != 0) {
    printf("Could not close video memory\n");
    return 1;
  }
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}

