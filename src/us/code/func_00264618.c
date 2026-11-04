#include "common.h"
#include "structs/game_object.h"

/* The object's intelligence (header.m_pIntelligence): only 0x38 here. */
struct CIntelligence {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s32 flags38;
};

void func_00264618(CGameObjectInstance *arg0, s32 *arg1) {
    if (!(arg0->header.m_pIntelligence->flags38 & 0x80)) {
        *arg1 &= 0xEFFFFFFF;
    }
}
