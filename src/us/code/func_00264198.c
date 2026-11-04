#include "common.h"
#include "structs/game_object.h"

void func_0026426C(s32, s32);
s32 func_00264378(s32, s32);

extern s32 D_800B20B0;

/* The actor's intelligence block (header.m_pIntelligence): a type word,
 * then a sub-block at 0x14. */
typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 pad4[0x8 - 0x4];
    /* 0x08 */ s32 unk8;
    /* 0x0C */ u8 padC[0x18 - 0xC];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x34 - 0x1C];
    /* 0x34 */ s32 unk34;
    /* 0x38 */ s32 unk38;
    /* 0x3C */ s32 unk3C;
    /* 0x40 */ f32 unk40;
    /* 0x44 */ u8 pad44[0x58 - 0x44];
    /* 0x58 */ s16 unk58;
} IntelSub;

typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ IntelSub sub;
} Intel;

void func_00264198(CGameObjectInstance *arg0, CAI *arg1) {
    IntelSub *temp_v0;
    Intel *temp_v1;

    temp_v1 = (Intel *) arg0->header.m_pIntelligence;
    arg1->m_pModeTable = (struct CAIModeInfo *) &D_800B20B0;
    arg1->m_pfStep = func_00264378;
    arg1->m_pfCalculateOrientationMatrix = func_0026426C;
    arg1->m_VarBuffer[4] = 0;
    arg1->m_VarBuffer[5] = 0;
    temp_v0 = &temp_v1->sub;
    arg1->m_ConditionTimer = temp_v0->unk40;
    arg1->m_VarBuffer[0] = 0;
    arg1->m_VarBuffer[1] = 0;
    arg1->m_VarBuffer[2] = 0;
    arg1->m_VarBuffer[3] = 0;
    *(f32 *)&arg1->m_VarBuffer[6] = temp_v0->unk40 * 6.283186f;
    if (temp_v1->sub.flags & 4) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x2000);
    }
}
