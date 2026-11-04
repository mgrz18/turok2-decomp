#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x174];
    /* 0x174 */ s8 alive;
} Member004035F8;

typedef struct {
    /* 0x000 */ u8 pad0[0x224];
    /* 0x224 */ Member004035F8 *members[8];
    /* 0x244 */ s32 count;
} Group004035F8;

extern CListedInstance *D_8010FEB4;

s32 func_004035F8(Group004035F8 *group) {
    s32 i;
    s32 n = 0;
    CListedInstance *a;

    for (i = 0; i < group->count; i++) {
        if (group->members[i]->alive == 1) {
            n++;
        }
    }
    for (a = D_8010FEB4; a != NULL; a = a->m_pNext) {
        if (a->m_Instance.m_AI.m_Health > 0) {
            n++;
        }
    }
    return n;
}
