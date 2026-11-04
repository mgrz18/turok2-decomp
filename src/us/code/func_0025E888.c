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

s32 func_00220260(s32, s32, s32);

s32 func_0026E110(void *, s32, void *);     /* extern */
extern s32 D_800F7078;

void func_0025E888(CGameObjectInstance *arg0, CAI *arg1) {
    s32 temp_a0;
    CIntelligenceSub *sub;

    arg1->m_Health = 0;
    arg1->m_ModeLastFrame = -1;
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x20000);
    temp_a0 = arg0->header.m_pIntelligence->type;
    if (((temp_a0 == 1) || (temp_a0 == 4)) && ((u32) ((u8) arg1->m_Mode - 0x35) < 7U)) {
        arg1->m_dwFlags |= 0x20;
    }
    sub = &arg0->header.m_pIntelligence->sub;
    if (!(arg0->m_dwFlags & 0x80000)) {
        if (!(sub->flags & 0x20)) {
            func_00220260((s32) &D_800F7078, (s32) arg0, 1);
        }
        func_0026E110(arg0, 1, arg0);
    }
}
