/* NON-MATCHING: 14 of 146 words differ: the final ABS(x) > ABS(y) test; the ROM reloads x/y in delay slots (bnel) and branches bgtz for 2/6 */
#include "common.h"

typedef struct WeaponMenu WeaponMenu;

typedef struct {
    /* 0x00 */ u8 pad0[0xC6];
    /* 0xC6 */ s8 stickX;
    /* 0xC7 */ s8 stickY;
} Controller;

typedef struct Player {
    /* 0x000 */ u8 pad0[0xA50];
    /* 0xA50 */ Controller *controller;
} Player;

extern u8 D_8011AAF1;           /* multiplayer */

#define ABS(x) ((f32) (x) < 0.0f ? -(x) : (x))

/* The weapon wheel direction the stick points at: 0 up, then clockwise in
 * eighths (diagonals only in single player); -1 with the stick near the
 * middle. */
s32 func_0024B88C(WeaponMenu *menu, Player *player) {
    Controller *c = player->controller;
    s8 x = c->stickX;
    s8 y = c->stickY;
    s32 dir;

    if (x * x + y * y < 3600) {
        return -1;
    }
    if (ABS(ABS(x) - ABS(y)) < 46 && x != 0 && y != 0 && !D_8011AAF1) {
        if (x * y > 0) {
            if (x > 0) {
                return 1;
            }
            return 5;
        }
        return y > 0 ? 7 : 3;
    }
    if (ABS(x) > ABS(y)) {
        dir = x > 0 ? 2 : 6;
        return dir;
    }
    return y > 0 ? 0 : 4;
}
