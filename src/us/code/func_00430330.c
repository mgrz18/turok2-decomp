/* WRONG CUT: the real function is func_00430330, size 0x7C (splat split it at func_00430374). */
#include "common.h"
#include "structs/game.h"
#include "globals/game.h"

/* Turok 2: the mode block sits at 0x23FD0 (see CEngineApp__UpdateCinemaMode). */

extern char gRumblePool[];

void Controller_SetRumbleOptionAll(s32);
void RumblePool_FreeAll(void *);
void func_0025359C(void *);

/* Resets the match state and asks the session for mode 1. */
void func_00430330(CEngineApp *session) {
    GameMatch *m;

    Controller_SetRumbleOptionAll(0);
    RumblePool_FreeAll(gRumblePool);
    func_0025359C(&gEngineApp_m_Game.m_PlayerList);
    m = &gEngineApp_m_Settings.match;
    m->teams = 0;
    m->tag = 0;
    m->unk44 = 0;
    session->m_Settings.fadeMode = 2;
    session->m_Settings.nextMode = 1;
}
