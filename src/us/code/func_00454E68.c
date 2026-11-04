#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ CPlayer *player;
} MenuItem;

s32 func_0041648C(s32, s32, s32, s32, s32, s32);
extern CPlayerOptions D_8011AB18;

s32 func_00454E68(s32 arg0, MenuItem *arg1) {
    CPlayerOptions *temp_v0_2;
    CPlayerOptions *var_s0;
    CPlayer *temp_v0;

    temp_v0 = arg1->player;
    var_s0 = &D_8011AB18;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pOptions;
        if (temp_v0_2 != NULL) {
            var_s0 = temp_v0_2;
        }
    }
    var_s0->unk_0x2B = func_0041648C((s32) arg1, (s32) var_s0->unk_0x2B, 1, 0, 1, 1);
    return 0;
}
