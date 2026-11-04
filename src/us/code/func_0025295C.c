#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ CPlayer *head;
} PlayerList;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 limit;
    /* 0x1C */ u8 pad1C[0x24 - 0x1C];
    /* 0x24 */ s32 teams;
} GameState;

extern GameState D_8011ACB0;
extern s32 func_0025340C(PlayerList *list, CPlayer *player);

/* Whether the frag limit is reached: by either team's total in team games,
 * otherwise by one player's frags (the slot func_0025340C names left out). */
s32 func_0025295C(PlayerList *list) {
    GameState *state = &D_8011ACB0;
    CPlayer *p;
    s32 i;

    if (state->teams) {
        s32 team1 = 0;
        s32 team0 = 0;
        s32 skip;

        for (p = list->head; p != NULL; p = p->m_pNext) {
            skip = func_0025340C(list, p);
            for (i = 0; i < 4; i++) {
                if (i != skip) {
                    switch (p->m_nDeathmatchTeamSelected) {
                        case 0:
                            team0 += p->m_pOptions->m_Killed[i];
                            break;
                        case 1:
                            team1 += p->m_pOptions->m_Killed[i];
                            break;
                    }
                }
            }
            if (team0 >= D_8011ACB0.limit) {
                return 1;
            }
            if (team1 >= D_8011ACB0.limit) {
                return 1;
            }
        }
    } else {
        GameState *gs = state;

        for (p = list->head; p != NULL; p = p->m_pNext) {
            s32 skip;
            s32 sum;

            skip = func_0025340C(list, p);
            sum = 0;
            for (i = 0; i < 4; i++) {
                if (i != skip) {
                    sum += p->m_pOptions->m_Killed[i];
                }
            }
            if (sum >= gs->limit) {
                return 1;
            }
        }
    }
    return 0;
}
