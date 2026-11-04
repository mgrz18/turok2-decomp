#include "common.h"
#include "structs/world.h"

typedef struct {
    /* 0x0 */ s32 type;
} ActorDef;

typedef struct LevelActor {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ u8 pad18[0xB8 - 0x18];
    /* 0x0B8 */ u16 typeFlag;
    /* 0x0BA */ u8 padBA[0xD4 - 0xBA];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x20A - 0xD8];
    /* 0x20A */ s8 enemyIndex;
} LevelActor;

s32 func_00267090(s32);
s32 func_002681BC(s32);

s32 func_0021EBC0(s32);                         /* extern */

/* Files a new actor into every actor list of the world it belongs to. */
void func_002255CC(CWorld *arg0, LevelActor *arg1) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 temp_v1_5;

    func_0021EBC0((s32) arg1);
    if (func_00267090((s32) arg1) != 0) {
        temp_v1 = arg0->m_nDrawActors;
        if (temp_v1 != 0x200) {
            arg0->m_pDrawActors[temp_v1] = (CGameObjectInstance *)arg1;
            arg0->m_nDrawActors = (s32) (temp_v1 + 1);
        }
    }
    if (func_002681BC((s32) arg1) != 0) {
        temp_v1_2 = arg0->m_nMovers;
        if (temp_v1_2 != 0x80) {
            arg0->m_pMovers[temp_v1_2] = (CGameObjectInstance *)arg1;
            arg0->m_nMovers = (s32) (temp_v1_2 + 1);
        }
    }
    temp_v1_3 = arg0->m_nRegistered;
    if (temp_v1_3 < 0x80) {
        arg0->m_pRegistered[temp_v1_3] = (CGameObjectInstance *)arg1;
        arg0->m_nRegistered = (s32) (temp_v1_3 + 1);
    }
    temp_a0 = arg0->m_nKind1;
    if ((temp_a0 < 0x40) && (arg1->def->type == 1)) {
        arg0->m_pKind1[temp_a0] = (CGameObjectInstance *)arg1;
        temp_v0 = arg0->m_nKind1;
        arg1->enemyIndex = (s8) temp_v0;
        arg0->m_nKind1 = (s32) (temp_v0 + 1);
    }
    temp_v1_4 = arg0->m_nFlagged;
    if ((temp_v1_4 < 0x20) && (arg1->flags & 2)) {
        arg0->m_pFlagged[temp_v1_4] = (CGameObjectInstance *)arg1;
        arg0->m_nFlagged = (s32) (temp_v1_4 + 1);
    }
    temp_a0_2 = arg0->m_nKind5;
    if ((temp_a0_2 < 0x10) && (arg1->def->type == 5)) {
        arg0->m_pKind5[temp_a0_2] = (CGameObjectInstance *)arg1;
        arg0->m_nKind5 = (s32) (temp_a0_2 + 1);
    }
    temp_a0_3 = arg0->m_nList6;
    if (temp_a0_3 < 0x20) {
        if (arg1->typeFlag == 0x64F) {
            arg0->m_pList6[temp_a0_3] = (CGameObjectInstance *)arg1;
            arg0->m_nList6 = (s32) (temp_a0_3 + 1);
        }
    }
    if (arg1->typeFlag == 0x64D) {
        temp_v1_5 = arg0->m_nList7;
        if (temp_v1_5 < 4) {
            arg0->m_pList7[temp_v1_5] = (CGameObjectInstance *)arg1;
            arg0->m_nList7 = (s32) (temp_v1_5 + 1);
        }
    }
}
