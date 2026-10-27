#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ u16 frags[4];        /* per opponent slot */
} PlayerOptions;

typedef struct Player {
    /* 0x0000 */ u8 pad0[0x518];
    /* 0x0518 */ PlayerOptions *options;
    /* 0x051C */ u8 pad51C[0xBF4 - 0x51C];
    /* 0x0BF4 */ s32 team;
    /* 0x0BF8 */ u8 padBF8[0x1320 - 0xBF8];
    /* 0x1320 */ struct Player *next;
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Player *head;
} PlayerList;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 limit;
    /* 0x1C */ u8 pad1C[0x24 - 0x1C];
    /* 0x24 */ s32 teams;
} GameState;

extern GameState D_8011ACB0;
extern s32 func_0025340C(PlayerList *list, Player *player);

/* Whether the frag limit is reached: by either team's total in team games,
 * otherwise by one player's frags (the slot func_0025340C names left out). */
s32 func_0025295C(PlayerList *list) {
    GameState *state = &D_8011ACB0;
    Player *p;
    s32 i;

    if (state->teams) {
        s32 team1 = 0;
        s32 team0 = 0;
        s32 skip;

        for (p = list->head; p != NULL; p = p->next) {
            skip = func_0025340C(list, p);
            for (i = 0; i < 4; i++) {
                if (i != skip) {
                    switch (p->team) {
                        case 0:
                            team0 += p->options->frags[i];
                            break;
                        case 1:
                            team1 += p->options->frags[i];
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

        for (p = list->head; p != NULL; p = p->next) {
            s32 skip;
            s32 sum;

            skip = func_0025340C(list, p);
            sum = 0;
            for (i = 0; i < 4; i++) {
                if (i != skip) {
                    sum += p->options->frags[i];
                }
            }
            if (sum >= gs->limit) {
                return 1;
            }
        }
    }
    return 0;
}
