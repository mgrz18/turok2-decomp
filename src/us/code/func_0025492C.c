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
extern f32 gFrameIncrement;          /* frame time scale */

extern s32 MenuPool_CountOpen(void *menuPool);
extern s32 CPlayerPool__CountIdleMenus(PlayerList *list);
extern s32 CPlayerPool__IsFragLimitReached(PlayerList *list);
extern void CPlayerPool__EndMatch(PlayerList *list);

/* Run the match clock and end the match when the time is up or, outside tag
 * mode, when the frag limit is reached. Not while a menu is open. */
void CPlayerPool__UpdateMatchClock(PlayerList *list) {
    MatchState *match = &D_80119E2C.match;
    Globals *g;

    if (match->over || !D_80119E2C.timed) {
        return;
    }
    if (!MenuPool_CountOpen(&D_80119E2C) && match->timeLeft > 0.0f && !D_80119E2C.paused) {
        if (CPlayerPool__CountIdleMenus(list)) {
            match->timeLeft -= gFrameIncrement;
        }
        if (match->timeLeft <= 0.0f && !match->over) {
            match->timeLeft = 0.0f;
            CPlayerPool__EndMatch(list);
        }
    }
    g = &D_80119E2C;
    if (!MenuPool_CountOpen(g) && CPlayerPool__IsFragLimitReached(list)) {
        MatchState *m = &g->match;

        if (!m->over && !g->paused && m->fragLimit > 0 && !m->tagMode) {
            CPlayerPool__EndMatch(list);
        }
    }
}
