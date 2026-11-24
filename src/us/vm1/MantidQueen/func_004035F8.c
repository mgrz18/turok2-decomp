#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/boss.h"
/* the view at the host offsets (round 9 x2, the Mantid Queen fight): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
typedef struct {
    union {
        struct { u8 _hp_alive[__builtin_offsetof(CGameObjectInstance, m_AI.m_Mode)]; s8 alive; };
    };
} Member004035F8;
typedef struct {
    union {
        struct { u8 _hp_members[__builtin_offsetof(WaveBossState, members)]; Member004035F8 * members[8]; };
        struct { u8 _hp_count[__builtin_offsetof(WaveBossState, memberCount)]; s32 count; };
    };
} Group004035F8;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x174];
    /* 0x174 */ s8 alive;
} Member004035F8;
typedef struct {
    /* 0x000 */ u8 pad0[0x224];
    /* 0x224 */ Member004035F8 *members[8];
    /* 0x244 */ s32 count;
} Group004035F8;
#endif



s32 MantidQueen_CountActiveMinions(Group004035F8 *group) {
    s32 i;
    s32 n = 0;
    CListedInstance *a;

    for (i = 0; i < group->count; i++) {
        if (group->members[i]->alive == 1) {
            n++;
        }
    }
    for (a = gScene.m_InstancePool.m_ActiveList.m_pHead; a != NULL; a = a->m_pNext) {
        if (a->m_Instance.m_AI.m_Health > 0) {
            n++;
        }
    }
    return n;
}
