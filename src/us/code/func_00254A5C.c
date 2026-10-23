#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ u16 field10[4];
} PlayerOptions;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ CPlayer *head;
} PlayerList;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 limit;
} GameState;

extern GameState D_8011ACB0;
extern s32 func_0025340C(PlayerList *list, CPlayer *player);

/* Whether some player sums, over the slots other than func_0025340C's, at
 * least D_8011ACB0.limit in options field 0x10. */
s32 func_00254A5C(PlayerList *list) {
    GameState *state;
    CPlayer *p;
    s32 skip;
    s32 sum;
    s32 i;

    for (p = list->head; p != NULL; p = p->m_pNext) {
        state = &D_8011ACB0;
        skip = func_0025340C(list, p);
        sum = 0;
        for (i = 0; i < 4; i++) {
            if (i != skip) {
                sum += ((PlayerOptions *)p->m_pOptions)->field10[i];
            }
        }
        if (sum >= state->limit) {
            return 1;
        }
    }
    return 0;
}
