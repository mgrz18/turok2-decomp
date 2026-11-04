#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"

s32 func_00455924(s32 arg0, Menu *arg1) {
    ((CPlayer *) arg1->player)->m_pOptions->unk_0x3D = 1;
    return 1;
}
