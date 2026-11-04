#include "common.h"
#include "structs/player.h"

/* The menu this handler belongs to: only its player here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ CPlayer *player;
} PlayerMenu;

s32 func_0041648C(s32, s32, s32, s32, s32, s32);
extern struct CPlayerOptions D_8011AB18;

s32 func_00455420(s32 arg0, PlayerMenu *arg1) {
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
    var_s0->m_SettingX = func_0041648C((s32) arg1, (s32) var_s0->m_SettingX, 8, 8, 0xF8, 0);
    return 0;
}
