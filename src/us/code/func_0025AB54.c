#include "common.h"
#include "structs/game_object.h"

/* The object's intelligence (header.m_pIntelligence): its flags and its
 * table of animation ids, one per action (0x76-0x9A). */
struct CIntelligence {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 m_dwFlags;
    /* 0x18 */ u8 pad18[0x54 - 0x18];
    /* 0x54 */ s32 unk54;
    /* 0x58 */ u8 pad58[0x76 - 0x58];
    /* 0x76 */ u16 m_AnimIds[19];
};

s32 func_00245A98(CGameObjectInstance *obj, CAI *ai, u8 *table, s32 anim);

extern u8 D_800B0480[];

void func_0025AB54(CGameObjectInstance *obj, CAI *ai) {
    func_00245A98(obj, ai, D_800B0480, obj->header.m_pIntelligence->m_AnimIds[3]);
}
