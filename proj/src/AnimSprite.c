#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "AnimSprite.h"


/*
AnimSprite *create_animSprite(uint8_t no_pic, char *pic1[]) {
    AnimSprite *asp = malloc(sizeof(AnimSprite));
    if (asp == NULL)
        return NULL;

    asp->sp = create_sprite(pic1[0], 0, 0, 0, 0);
    if (asp->sp == NULL) {
        free(asp);
        return NULL;
    }

    asp->map = malloc(no_pic * sizeof(char *));
    if (asp->map == NULL) {
        destroy_sprite(asp->sp);
        free(asp);
        return NULL;
    }

    asp->map[0] = asp->sp->map;

    for (int i = 1; i < no_pic; i++) {
        xpm_image_t img;
        asp->map[i] = xpm_load(pic1[i], XPM_8_8_8_8, &img);
        if (asp->map[i] == NULL || img.width != asp->sp->width || img.height != asp->sp->height) {
            for (int j = 1; j < i; j++)
                free(asp->map[j]);
            free(asp->map);
            destroy_sprite(asp->sp);
            free(asp);
            return NULL;
        }
    }

    asp->aspeed = 1; 
    asp->cur_aspeed = asp->aspeed;
    asp->num_fig = no_pic;
    asp->cur_fig = 0;

    return asp;
}
*/


