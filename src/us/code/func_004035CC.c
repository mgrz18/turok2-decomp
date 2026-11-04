#include "common.h"
#include "structs/game_object.h"

/* qsort comparator over actor pointers, by the float the AI keeps in its first variable (0x24C). */
typedef struct {
    /* 0x000 */ u8 pad0[0x24C];
    /* 0x24C */ f32 sortKey;
} SortedActor;

s32 func_004035CC(SortedActor **arg0, SortedActor **arg1) {
    s32 var_v0;

    var_v0 = 1;
    if ((*arg0)->sortKey < (*arg1)->sortKey) {
        var_v0 = -1;
    }
    return var_v0;
}
