#include "common.h"
#include "structs/game_object.h"

/* The object's intelligence (header.m_pIntelligence): only 0x72 here. */
struct CIntelligence {
    /* 0x00 */ u8 pad0[0x72];
    /* 0x72 */ s16 unk72;
};

s16 func_0025FDF8(CGameObjectInstance *arg0) {
    return arg0->header.m_pIntelligence->unk72;
}
