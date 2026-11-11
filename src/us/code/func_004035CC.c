#include "common.h"
#include "structs/game_object.h"

#ifdef TARGET_PORT
/* qsort comparator over actor pointers, by the float the AI keeps in its first variable
 * (m_AI.m_VarF32[0], N64 0x24C; round 9 x2: the host member) */
s32 MantidQueen_SortCompare(CGameObjectInstance **arg0, CGameObjectInstance **arg1) {
    s32 var_v0;

    var_v0 = 1;
    if ((*arg0)->m_AI.m_VarF32[0] < (*arg1)->m_AI.m_VarF32[0]) {
        var_v0 = -1;
    }
    return var_v0;
}
#else
/* qsort comparator over actor pointers, by the float the AI keeps in its first variable (0x24C). */
typedef struct {
    /* 0x000 */ u8 pad0[0x24C];
    /* 0x24C */ f32 sortKey;
} SortedActor;

s32 MantidQueen_SortCompare(SortedActor **arg0, SortedActor **arg1) {
    s32 var_v0;

    var_v0 = 1;
    if ((*arg0)->sortKey < (*arg1)->sortKey) {
        var_v0 = -1;
    }
    return var_v0;
}
#endif
