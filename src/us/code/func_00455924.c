#include "common.h"
#include "structs/player.h"

/* A menu item (MenuItem in func_00450F4C): the player whose options it shows. */
typedef struct MenuItem {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ CPlayer *player;
} MenuItem;

s32 func_00455924(s32 arg0, MenuItem *arg1) {
    arg1->player->m_pOptions->unk_0x3D = 1;
    return 1;
}
