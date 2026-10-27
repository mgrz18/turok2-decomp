#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ f32 timeLeft;
    /* 0x18 */ s32 fragLimit;
    /* 0x1C */ s32 over;
    /* 0x20 */ u8 pad20[0x34 - 0x20];
    /* 0x34 */ s32 tagMode;
} MatchState;

/* The globals from D_80119E2C: a menu pool first. */
typedef struct {
    /* 0x000 */ void *menus;            /* CMenuPool */
    /* 0x004 */ u8 pad4[0xCC5 - 4];
    /* 0xCC5 */ u8 timed;               /* D_8011AAF1 */
    /* 0xCC6 */ u8 padCC6[0xE78 - 0xCC6];
    /* 0xE78 */ s32 paused;             /* D_8011ACA4 */
    /* 0xE7C */ u8 padE7C[0xE84 - 0xE7C];
    /* 0xE84 */ MatchState match;       /* D_8011ACB0 */
} Globals;

typedef struct PlayerList PlayerList;

extern Globals D_80119E2C;
extern f32 D_800B6D28;          /* frame time scale */

extern s32 func_00416AF8(void *menuPool);
extern s32 func_00254BE4(PlayerList *list);
extern s32 func_0025295C(PlayerList *list);
extern void func_002534E0(PlayerList *list);

/* Run the match clock and end the match when the time is up or, outside tag
 * mode, when the frag limit is reached. Not while a menu is open. */
void func_0025492C(PlayerList *list) {
    MatchState *match = &D_80119E2C.match;
    Globals *g;

    if (match->over || !D_80119E2C.timed) {
        return;
    }
    if (!func_00416AF8(&D_80119E2C) && match->timeLeft > 0.0f && !D_80119E2C.paused) {
        if (func_00254BE4(list)) {
            match->timeLeft -= D_800B6D28;
        }
        if (match->timeLeft <= 0.0f && !match->over) {
            match->timeLeft = 0.0f;
            func_002534E0(list);
        }
    }
    g = &D_80119E2C;
    if (!func_00416AF8(g) && func_0025295C(list)) {
        MatchState *m = &g->match;

        if (!m->over && !g->paused && m->fragLimit > 0 && !m->tagMode) {
            func_002534E0(list);
        }
    }
}
