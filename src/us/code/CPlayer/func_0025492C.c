#include "common.h"
#include "globals/menu.h"
#include "globals/engine.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ f32 timeLeft;
    /* 0x18 */ s32 fragLimit;
    /* 0x1C */ s32 over;
    /* 0x20 */ u8 pad20[0x34 - 0x20];
    /* 0x34 */ s32 tagMode;
} MatchState_25492C;

/* The globals from gEngineApp_m_Game_m_MenuPool: a menu pool first. */
typedef struct {
    /* 0x000 */ void *menus;            /* CMenuPool */
    /* 0x004 */ u8 pad4[0xCC5 - 4];
    /* 0xCC5 */ u8 timed;               /* gEngineApp_m_Settings_multiplayer */
    /* 0xCC6 */ u8 padCC6[0xE78 - 0xCC6];
    /* 0xE78 */ s32 paused;             /* gEngineApp_m_Settings_paused */
    /* 0xE7C */ u8 padE7C[0xE84 - 0xE7C];
    /* 0xE84 */ MatchState_25492C match;       /* gEngineApp_m_Settings_match */
} Globals_25492C;

typedef struct PlayerList_25492C PlayerList_25492C;

extern Globals_25492C gMenuPoolGlobalsView_25492C __asm__("gEngineApp_m_Game_m_MenuPool"); /* gEngineApp_m_Game_m_MenuPool (declared by globals/menu.h) seen as this file's type: what keeps the ROM's code */

s32 MenuPool_CountOpen(void *);
extern s32 CPlayerPool__CountIdleMenus(PlayerList_25492C *list);
extern s32 CPlayerPool__IsFragLimitReached(PlayerList_25492C *list);
extern void CPlayerPool__EndMatch(PlayerList_25492C *list);

/* Run the match clock and end the match when the time is up or, outside tag
 * mode, when the frag limit is reached. Not while a menu is open. */
#ifdef TARGET_PORT
#include "structs/game.h"
#include "globals/game.h"

/* The port: the settings by member of gEngineApp_m_Settings (the N64 code reaches multiplayer,
 * paused and the match through a view of the menu pool at 0xCC5/0xE78/0xE84, N64 offsets past
 * the host menu pool); the menu pool itself is the first member of that view. */
void CPlayerPool__UpdateMatchClock(void *list_) {
    PlayerList_25492C *list = list_;
    GameMatch *match = &gEngineApp_m_Settings.match;

    if (match->over || !gEngineApp_m_Settings.multiplayer) {
        return;
    }
    if (!MenuPool_CountOpen(&gMenuPoolGlobalsView_25492C) && match->timeLeft > 0.0f && !gEngineApp_m_Settings.paused) {
        if (CPlayerPool__CountIdleMenus(list)) {
            match->timeLeft -= gFrameIncrement;
        }
        if (match->timeLeft <= 0.0f && !match->over) {
            match->timeLeft = 0.0f;
            CPlayerPool__EndMatch(list);
        }
    }
    if (!MenuPool_CountOpen(&gMenuPoolGlobalsView_25492C) && CPlayerPool__IsFragLimitReached(list)) {
        if (!match->over && !gEngineApp_m_Settings.paused && match->scoreLimit > 0 && !match->tag) {
            CPlayerPool__EndMatch(list);
        }
    }
}
#else
void CPlayerPool__UpdateMatchClock(void *list_) {
    PlayerList_25492C *list = list_;
    MatchState_25492C *match = &gMenuPoolGlobalsView_25492C.match;
    Globals_25492C *g;

    if (match->over || !gMenuPoolGlobalsView_25492C.timed) {
        return;
    }
    if (!MenuPool_CountOpen(&gMenuPoolGlobalsView_25492C) && match->timeLeft > 0.0f && !gMenuPoolGlobalsView_25492C.paused) {
        if (CPlayerPool__CountIdleMenus(list)) {
            match->timeLeft -= gFrameIncrement;
        }
        if (match->timeLeft <= 0.0f && !match->over) {
            match->timeLeft = 0.0f;
            CPlayerPool__EndMatch(list);
        }
    }
    g = &gMenuPoolGlobalsView_25492C;
    if (!MenuPool_CountOpen(g) && CPlayerPool__IsFragLimitReached(list)) {
        MatchState_25492C *m = &g->match;

        if (!m->over && !g->paused && m->fragLimit > 0 && !m->tagMode) {
            CPlayerPool__EndMatch(list);
        }
    }
}
#endif
