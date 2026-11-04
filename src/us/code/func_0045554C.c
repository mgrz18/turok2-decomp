#include "common.h"
#include "structs/player.h"

/* A menu item (MenuItem in func_00450F4C): the player whose options it shows. */
typedef struct MenuItem {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ CPlayer *player;
} MenuItem;

s32 func_0041648C(s32, s32, s32, s32, s32, s32);

extern struct CPlayerOptions D_8011AB18;

s32 func_0045554C(s32 arg0, MenuItem *arg1) {
    struct CPlayerOptions *temp_v0_2;
    struct CPlayerOptions *var_s0;
    CPlayer *temp_v0;

    temp_v0 = arg1->player;
    var_s0 = &D_8011AB18;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pOptions;
        if (temp_v0_2 != NULL) {
            var_s0 = temp_v0_2;
        }
    }
    var_s0->m_SettingY = func_0041648C((s32) arg1, var_s0->m_SettingY, 8, 8, 0xF8, 0);
    return 0;
}
