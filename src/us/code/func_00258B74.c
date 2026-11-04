#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);
s32 func_0024E700(s32, s32);
s32 func_00257BA0(s32);
s32 func_00257EA0(s32, s32);

extern u8 D_800ADF68[];

/* The AI parent of the actor (m_AI.m_pParent), seen as a larger object. */
typedef struct {
    /* 0x000 */ u8 pad0[0x996];
    /* 0x996 */ s16 unk996;
    /* 0x998 */ u8 pad998[0xA08 - 0x998];
    /* 0xA08 */ s16 unkA08;   /* row in the 0x18-byte table D_800ADF68 */
    /* 0xA0A */ u8 padA0A[0xB14 - 0xA0A];
    /* 0xB14 */ s16 unkB14;
} Unk00258B74;

void func_00258B74(CGameObjectInstance *arg0, s32 arg1) {
    s32 temp_s2;
    Unk00258B74 *temp_s0;

    temp_s0 = (Unk00258B74 *)arg0->m_AI.m_pParent;
    temp_s2 = *(s16 *)(D_800ADF68 + (temp_s0->unkA08 * 0x18));
    if (func_0024E700((s32) temp_s0, (s32) temp_s0->unk996) == 0) {
        temp_s0->unkB14 = func_00257BA0((s32) temp_s0);
    }
    if ((func_00257EA0((s32) arg0, arg1) == 0) && !(arg0->m_dwFlags & 0x400)) {
        func_00243414((s32) arg0, arg1, (s32) temp_s2);
    }
}
