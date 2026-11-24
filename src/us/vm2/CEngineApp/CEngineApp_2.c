/* MATCHES at its real start (checked with the symbol moved to 0x4301C0, size 0x90). */
/* WRONG CUT: the real function starts at 0x004691C0 (size 0x90 = 144); the two
 * words at 0x004691B8 (sw v0,0(t3); move a1,zero) are junk between objects. */
#include "common.h"
#include "structs/gbi.h"

/* Initialise a unit quad (4 vertices in the XZ plane, 32x32 texels, white). */


#include "globals/render.h"

void Render_InitUnitQuad_2(void) {
    Vtx_t *v;
    s32 i;

    i = 0;
    v = gUnitQuadVtx;
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
    for (; i < 4; i++) {
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

void RumblePool_FreeAll(RumblePool *pool);
s32 Controller_SetRumbleOptionAll(s32);
s32 CPlayerPool__StopRumble(CPlayerPool *);                   /* extern */

/* The multiplayer match state at gEngineApp_m_Settings_match: the fields this resets. */

/* The game/menu context this runs on. */


void CEngineApp__QuitToLegal_2(CEngineApp *arg0)
{
  GameMatch *new_var;
  Controller_SetRumbleOptionAll(0);
  RumblePool_FreeAll(&gRumblePool);
  CPlayerPool__StopRumble(&gEngineApp_m_Game.m_PlayerList);
  new_var = &gEngineApp_m_Settings.match;
  new_var->teams = 0;
  gEngineApp_m_Settings.match.tag = 0;
  gEngineApp_m_Settings.match.unk44 = 0;
  arg0->m_Settings.fadeMode = 2;
  arg0->m_Settings.nextMode = 1;
}

#include "globals/scene.h"

/* The play-time clock (see CEngineApp__UpdatePlayTime.c) and a word at 0x44. */

/* The fields this sets, far into this (large) state. */

s32 CScene__FindWarpDests_2(void *, s32, s32, void *, s32);   /* extern */

void CEngineApp__StartCinemaGame_2(CEngineApp *arg0) {
    u8 sp18[0x18];
    s32 var_s1;

    var_s1 = -1;
    if (CScene__FindWarpDests_2(&gScene, -1, -1, sp18, 1) == 0) {
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

/* Start a level: check that the warp target exists, reset the multiplayer
 * options and switch the game state. */


s32 CScene__FindWarpDests_2(void *level, s32 a, s32 id, void *out, s32 flags);

void CEngineApp__StartGame_2(CEngineApp *game, s32 warpId) {
    u8 sp18[0x18];
    s32 id;

    id = warpId;

    if (CScene__FindWarpDests_2(&gScene, -1, id, sp18, 1) == 0) {
        id = 0;
    }
    gEngineApp_m_Settings.match.frames = 0;
    gEngineApp_m_Settings.match.seconds = 0;
    gEngineApp_m_Settings.match.minutes = 0;
    gEngineApp_m_Settings.match.hours = 0;
    gEngineApp_m_Settings.match.unk44 = 0;
    do {
        game->m_Settings.warpFlags = 0;
        game->m_Settings.matchMode = id;
    } while (0);
    do {
        game->m_Settings.fadeMode = 2;
        game->m_Settings.nextMode = 13;
    } while (0);
}
