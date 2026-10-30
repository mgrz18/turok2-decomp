#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ void *script;
} Cinema;

typedef struct {
    /* 0x000 */ u8 pad0[0x53C];
    /* 0x53C */ Cinema *cinema;
} Camera;

typedef struct {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ Camera *camera;
} Player;

extern s32 D_8011ACA4;
extern char D_00439640; /* death screen */

/* Whether a cinema holds the player's view (the death screen does not count). */

/* Whether a cinema holds the player's view (the death screen does not count). */
s32 func_00253FFC(Player *p) {
    Cinema *c;

    if (D_8011ACA4) {
        return 1;
    }
    if (p->camera != NULL && (c = p->camera->cinema) != NULL) {
        if (c->script != &D_00439640) {
            return 1;
        }
    }
    return 0;
}
