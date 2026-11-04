#include "common.h"
#include "structs/menu.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x29];
    /* 0x29 */ u8 unk29;
} Settings;

typedef struct {
    /* 0x000 */ u8 pad0[0x518];
    /* 0x518 */ Settings *settings;
} Player;

s32 func_0041648C(s32, s32, s32, s32, s32, s32);
extern Settings D_8011AB18;     /* the default settings */

s32 func_0041C4C0(s32 arg0, Menu *arg1) {
    Settings *temp_v0_2;
    Settings *var_s0;
    Player *temp_v0;

    temp_v0 = arg1->player;
    var_s0 = &D_8011AB18;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->settings;
        if (temp_v0_2 != NULL) {
            var_s0 = temp_v0_2;
        }
    }
    var_s0->unk29 = func_0041648C((s32) arg1, (s32) var_s0->unk29, 8, 8, 0xF8, 0);
    return 0;
}
