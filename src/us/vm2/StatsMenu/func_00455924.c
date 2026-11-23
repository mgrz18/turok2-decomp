#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"

s32 StatsMenu_Done_2(s32 arg0, Menu *arg1) {
    ((CPlayer *) arg1->player)->m_pOptions->unk_0x3D = 1;
    return 1;
}
