#include "common.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"


s32 func_00421250();                            /* extern */

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
} Unk00419618;

s32 SingleMenu_Load(s32 arg0, Unk00419618 *arg1) {
    func_00421250();
    MenuPool_OpenMenu((s32) &gEngineApp_m_Game_m_MenuPool, (s32) &gPakLoadGameMenuDesc, arg1->unk1C, arg1->unk20, 0);
    return 1;
}
