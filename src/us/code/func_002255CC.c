#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"


/* The per-kind actor lists of the world (layout as in func_00221210). */
typedef struct {
    /* 0x0000 */ u8 pad0[0xEC];
    /* 0x00EC */ CGameObjectInstance *l0[0x200];
    /* 0x08EC */ s32 n0;
    /* 0x08F0 */ CGameObjectInstance *l1[0x80];
    /* 0x0AF0 */ s32 n1;
    /* 0x0AF4 */ u8 padAF4[0x104];
    /* 0x0BF8 */ CGameObjectInstance *l2[0x80];
    /* 0x0DF8 */ s32 n2;
    /* 0x0DFC */ CGameObjectInstance *l3[0x40];
    /* 0x0EFC */ s32 n3;
    /* 0x0F00 */ s32 cur3;
    /* 0x0F04 */ CGameObjectInstance *l4[0x20];
    /* 0x0F84 */ s32 n4;
    /* 0x0F88 */ CGameObjectInstance *l5[0x10];
    /* 0x0FC8 */ s32 n5;
    /* 0x0FCC */ CGameObjectInstance *l6[0x20];
    /* 0x104C */ s32 n6;
    /* 0x1050 */ CGameObjectInstance *l7[4];
    /* 0x1060 */ s32 n7;
} World;

s32 func_00267090(s32);
s32 func_002681BC(s32);

s32 func_0021EBC0(s32);                         /* extern */

/* Files a new actor into every actor list of the world it belongs to. */
void func_002255CC(World *arg0, CGameObjectInstance *arg1) {
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
        temp_v1 = arg0->n0;
        if (temp_v1 != 0x200) {
            arg0->l0[temp_v1] = arg1;
            arg0->n0 = (s32) (temp_v1 + 1);
        }
    }
    if (func_002681BC((s32) arg1) != 0) {
        temp_v1_2 = arg0->n1;
        if (temp_v1_2 != 0x80) {
            arg0->l1[temp_v1_2] = arg1;
            arg0->n1 = (s32) (temp_v1_2 + 1);
        }
    }
    temp_v1_3 = arg0->n2;
    if (temp_v1_3 < 0x80) {
        arg0->l2[temp_v1_3] = arg1;
        arg0->n2 = (s32) (temp_v1_3 + 1);
    }
    temp_a0 = arg0->n3;
    if ((temp_a0 < 0x40) && (arg1->header.m_pIntelligence->m_Type == 1)) {
        arg0->l3[temp_a0] = arg1;
        temp_v0 = arg0->n3;
        arg1->m_AI.m_PathFindCount = (s8) temp_v0;
        arg0->n3 = (s32) (temp_v0 + 1);
    }
    temp_v1_4 = arg0->n4;
    if ((temp_v1_4 < 0x20) && (arg1->m_dwFlags & 2)) {
        arg0->l4[temp_v1_4] = arg1;
        arg0->n4 = (s32) (temp_v1_4 + 1);
    }
    temp_a0_2 = arg0->n5;
    if ((temp_a0_2 < 0x10) && (arg1->header.m_pIntelligence->m_Type == 5)) {
        arg0->l5[temp_a0_2] = arg1;
        arg0->n5 = (s32) (temp_a0_2 + 1);
    }
    temp_a0_3 = arg0->n6;
    if (temp_a0_3 < 0x20) {
        if (arg1->m_nTypeFlag == 0x64F) {
            arg0->l6[temp_a0_3] = arg1;
            arg0->n6 = (s32) (temp_a0_3 + 1);
        }
    }
    if (arg1->m_nTypeFlag == 0x64D) {
        temp_v1_5 = arg0->n7;
        if (temp_v1_5 < 4) {
            arg0->l7[temp_v1_5] = arg1;
            arg0->n7 = (s32) (temp_v1_5 + 1);
        }
    }
}
