#include "common.h"
#include "structs/game_object.h"

/* The block at 0x14 of the object definition (func_0025EFF8 hands it to func_00259E60). */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 id;
} CIntelligenceSub;

/* The object definition at header.m_pIntelligence (0x14), as the
 * func_00245A98 callers (func_0025AC5C...), func_0025EFF8 and func_0025E888 see it. */
struct CIntelligence {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad4[0x14 - 4];
    /* 0x14 */ CIntelligenceSub sub;
    /* 0x20 */ u8 pad20[0x74 - 0x20];
    /* 0x74 */ s16 unk74;
    /* 0x76 */ u8 pad76[2];
    /* 0x78 */ u16 m_AnimMasks[18];
};

extern s32 func_00259E60(void *, s32, void *);

s32 func_0025EFF8(CGameObjectInstance *arg0, s32 arg1) {
    u16 kind;
    CIntelligenceSub *sub;

    kind = arg0->m_nTypeFlag;
    sub = &arg0->header.m_pIntelligence->sub;
    if (kind == 0x1F9 || kind == 0x385) {
        switch (sub->id) {
            case 0x2B16:
            case 0x2B20:
            case 0x2B2A:
            case 0x2B34:
            case 0x2B3E:
                break;
            default:
                return func_00259E60(arg0, arg1, sub);
        }
    }
    return sub->id;
}
