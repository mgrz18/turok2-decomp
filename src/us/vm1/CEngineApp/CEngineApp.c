/* WRONG CUT: the real function is Render_InitUnitQuad, size 0x90 (splat split it at func_004302CC). */
#include "common.h"
#include "structs/gbi.h"

extern Vtx_t gUnitQuadVtx[4]; /* local: no header declares it (globals/render.h has no Vtx_t) */

/* Builds a unit quad in the XZ plane, white, textured 32x32 (S10.5 texture coordinates). */
void Render_InitUnitQuad(void) {
    s32 i;
    Vtx_t *v = gUnitQuadVtx;

    v[0].ob[0] = -1;
    v[0].ob[2] = 1;
    v[0].tc[0] = 0;
    v[0].tc[1] = 0;
    v[1].ob[0] = 1;
    v[1].ob[2] = 1;
    v[1].tc[0] = 0x400;
    v[1].tc[1] = 0;
    v[2].ob[0] = 1;
    v[2].ob[2] = -1;
    v[2].tc[0] = 0x400;
    v[2].tc[1] = 0x400;
    v[3].ob[0] = -1;
    v[3].ob[2] = -1;
    v[3].tc[0] = 0;
    v[3].tc[1] = 0x400;
    for (i = 0; i < 4; i++) {
        v[i].ob[1] = 0;
        v[i].flag = 0;
        v[i].cn[0] = 0xFF;
        v[i].cn[1] = 0xFF;
        v[i].cn[2] = 0xFF;
        v[i].cn[3] = 0xFF;
    }
}

#include "structs/game.h"
#include "globals/game.h"
#include "globals/rumble.h"

/* Turok 2: the mode block sits at 0x23FD0 (see CEngineApp__UpdateCinemaMode). */


void Controller_SetRumbleOptionAll(s32);
void RumblePool_FreeAll(RumblePool *pool);
void CPlayerPool__StopRumble(void *);

/* Resets the match state and asks the session for mode 1. */
void CEngineApp__QuitToLegal(CEngineApp *session) {
    GameMatch *m;

    Controller_SetRumbleOptionAll(0);
    RumblePool_FreeAll(&gRumblePool);
    CPlayerPool__StopRumble(&gEngineApp_m_Game.m_PlayerList);
    m = &gEngineApp_m_Settings.match;
    m->teams = 0;
    m->tag = 0;
    m->unk44 = 0;
    session->m_Settings.fadeMode = 2;
    session->m_Settings.nextMode = 1;
}

#include "globals/scene.h"
#include "protos/scene.h"

void CEngineApp__StartCinemaGame(CEngineApp *arg0) {
    u8 sp18[0x18];
    s32 var_s1;

    var_s1 = -1;
    if (CScene__FindWarpDests(&gScene, -1, -1, sp18, 1) == 0) {
        var_s1 = 0;
    }
    gEngineApp_m_Settings.match.frames = 0;
    gEngineApp_m_Settings.match.seconds = 0;
    gEngineApp_m_Settings.match.minutes = 0;
    gEngineApp_m_Settings.match.hours = 0;
    arg0->m_Settings.warpFlags = 0;
    gEngineApp_m_Settings.match.unk44 = 0;
    arg0->m_Settings.fadeMode = 2;
    arg0->m_Settings.matchMode = var_s1;
    arg0->m_Settings.nextMode = 0xB;
}

#include "structs/vector.h"
#include "structs/world.h"

/* Turok 2: the mode block sits at 0x23FD0 (see CEngineApp__UpdateCinemaMode). */



/* Resets the match state and asks the session for mode 13 at warp `warp` (0 if the
 * world has no such warp point). */
/* The two do/while(0) blocks (macro-like groups in the original?) give the ROM's register choice
 * and store order. */
void CEngineApp__StartGame(CEngineApp *session, s32 warp) {
    CWarpPoint point;

    if (CScene__FindWarpDests(&gScene, -1, warp, &point, 1) == 0) {
        warp = 0;
    }
    gEngineApp_m_Settings.match.frames = 0;
    gEngineApp_m_Settings.match.seconds = 0;
    gEngineApp_m_Settings.match.minutes = 0;
    gEngineApp_m_Settings.match.hours = 0;
    gEngineApp_m_Settings.match.unk44 = 0;
    do {
        session->m_Settings.warpFlags = 0;
        session->m_Settings.matchMode = warp;
    } while (0);
    do {
        session->m_Settings.fadeMode = 2;
        session->m_Settings.nextMode = 13;
    } while (0);
}
