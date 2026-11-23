#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/cinema.h"
#include "globals/game.h"
#include "globals/camera.h"
#include "globals/cinema.h"
#include "globals/rumble.h"
#include "protos/scene.h"
#include "cinema_scene.h"


void CPlayer__ResetControlState(CPlayer *player);
void CCameraPool__ReleaseAllTexts(void *game);
void RumblePool_FreeAll(RumblePool *pool);
s32 Cinema_NoWaitCues(void);

/* Runs once (until the state is cleared): resets the current player when
 * asked, remembering where it starts the first time; warps to the pending
 * warp id unless it is the current one; then marks the state started. */
void Cinema_Start(void) {
    CPlayer *player;
    s32 warp;
    void *world;
    s32 out[3];

    warp = gCinemaPlayerP->warpId;
    if (gCinemaPlayerP->started != 0) {
        return;
    }
    if (gCinemaPlayerP->resetPlayer != 0) {
        player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
        if (player != NULL) {
            CPlayer__ResetControlState(player);
            if (gCinemaPlayerP->saveStart != 0 && gCinemaPlayerP->startLevel == -1
                && gCinemaPlayerP->startWarp == gCinemaPlayerP->startLevel) {
                gCinemaPlayerP->startLevel = gScene_m_LevelID[0];
                gCinemaPlayerP->startWarp = gScene_m_nLevel[0];
                gCinemaPlayerP->restored = 0;
                gCinemaPlayerP->startPos = player->m_CharacterActor.header.m_vPos;
                gCinemaPlayerP->startRotY = player->m_CharacterActor.m_RotY;
                gCinemaPlayerP->startRegion = CScene__GetRegionIndex(&gScene, player->m_CharacterActor.header.m_pCurrentRegion);
            }
            player->m_WeaponActor.m_AI.m_VarBuffer[0] = 0;
            player->m_WeaponActor.m_AI.m_VarBuffer[1] = 0;
            player->m_WeaponActor.m_AI.m_VarBuffer[5] = 0;
        }
        CCameraPool__ReleaseAllTexts(&gEngineApp_m_Game_m_CameraPool);
        RumblePool_FreeAll(&gRumblePool);
    }
    if (warp != -1) {
        world = &gScene;
        if (gScene_m_nLevel[0] != warp) {
            out[0] = 0;
            out[1] = 0;
            out[2] = 0;
            gScene_m_LevelID[0] = CScene__FindNearestWarpDest(world, out, warp, &gScene_m_WarpSpawn);
            gScene_m_bWarpSpawn = gScene_m_LevelID[0] != 0;
            CScene__LoadLevel(world, warp, gScene_m_bWarpSpawn);
        }
    }
    gCinemaPlayerP->restoreAtEnd = Cinema_NoWaitCues();
    gCinemaPlayerP->unk50 = 1;
    gCinemaPlayerP->warpId = -1;
    gCinemaPlayerP->started = 1;
}
