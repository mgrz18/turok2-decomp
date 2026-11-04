#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"

extern s32 D_800B3F00;
extern s32 D_800B3F18;

extern CPlayerOptions D_8011AB18;

s32 func_0041C034(MenuItem *arg0, Menu *arg1) {
    CPlayerOptions *temp_v0_2;
    CPlayerOptions *var_v1;
    u8 temp_v1;
    CPlayer *temp_v0;

    temp_v0 = arg1->player;
    var_v1 = &D_8011AB18;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pOptions;
        if (temp_v0_2 != NULL) {
            var_v1 = temp_v0_2;
        }
    }
    temp_v1 = var_v1->m_ControlSide;
    if (temp_v1 != 0) {
        if (temp_v1 == 1) {
            arg0->table = &D_800B3F00;
        }
    } else {
        arg0->table = &D_800B3F18;
    }
    return 0;
}
