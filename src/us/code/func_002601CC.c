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

f32 func_002119FC(f32, f32);                        /* extern */
s32 func_00243414(void *, void *, s32);     /* extern */
extern f32 D_800A797C;
extern f32 D_800A7980;
extern f32 D_800B6D28;

/* m_VarBuffer[6] holds a float timer here. */
s32 func_002601CC(CGameObjectInstance *arg0, CAI *arg1) {
    f32 temp_f0;
    s8 temp_v1;

    if (arg0->header.m_pIntelligence->sub.flags & 0x40) {
        temp_f0 = *(f32 *) &arg1->m_VarBuffer[6] - D_800B6D28;
        *(f32 *) &arg1->m_VarBuffer[6] = temp_f0;
        if (temp_f0 < 0.0f) {
            *(f32 *) &arg1->m_VarBuffer[6] = 0.0f;
        }
        if (arg1->m_Health != 0) {
            if (*(f32 *) &arg1->m_VarBuffer[6] <= 0.0f) {
                temp_v1 = arg1->m_Mode;
                if ((temp_v1 == 4) || (temp_v1 == 0xE) || (temp_v1 == 0xF) || (temp_v1 == 0x10) || (temp_v1 == 0x11) || (temp_v1 == 0x12) || (temp_v1 == 0xA)) {
                    *(f32 *) &arg1->m_VarBuffer[6] = func_002119FC(D_800A797C, D_800A7980);
                    func_00243414(arg0, arg1, 0x17);
                    return 1;
                }
                /* Duplicate return node #14. Try simplifying control flow for better match */
                return 0;
            }
            goto block_13;
        }
        /* Duplicate return node #14. Try simplifying control flow for better match */
        return 0;
    }
block_13:
    return 0;
}
