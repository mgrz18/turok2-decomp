#include "common.h"
#include "structs/player.h"

/* A menu item (MenuItem in func_00450F4C): the player whose options it shows. */
typedef struct MenuItem {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ CPlayer *player;
} MenuItem;

extern s32 D_8011AB18;

s32 *func_0041BD3C(MenuItem *arg0) {
    s32 *temp_v1_2;
    s32 *var_v0;
    CPlayer *temp_v1;

    temp_v1 = arg0->player;
    var_v0 = &D_8011AB18;
    if (temp_v1 != NULL) {
        temp_v1_2 = (s32 *) temp_v1->m_pOptions;
        if (temp_v1_2 != NULL) {
            var_v0 = temp_v1_2;
        }
    }
    return var_v0;
}
