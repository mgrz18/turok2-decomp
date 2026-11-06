#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/cinema.h"


extern Cinema *gCinemaPlayerP;
extern CPlayer *gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;          /* current player */
extern u8 gEngineApp_m_Game_m_CameraPool[];
extern u8 gRumblePool[];
/* The world and fields of it (see CScene__UpdateWarp's World). gScene_m_nLevel and
 * gScene_m_LevelID are arrays because the ROM accesses them as struct members: a
 * store through a pointer may alias them, so their loads stay after it. */
extern u8 gScene[];             /* the world */
extern s32 gScene_m_nLevel[];            /* world +0x18FA8: the current warp id */
extern s32 gScene_m_bWarpSpawn;              /* world +0x18FB8: selectionFlags */
extern u8 gScene_m_WarpSpawn[];             /* world +0x18FBC: selectionSpawn */
extern s32 gScene_m_LevelID[];            /* world +0x18FD0: selectionValues */

void CPlayer__ResetControlState(CPlayer *player);
s32 CScene__GetRegionIndex(void *world, s32 region);
void CCameraPool__ReleaseAllTexts(void *game);
void RumblePool_FreeAll(void *arg0);
s32 CScene__FindNearestWarpDest(void *world, s32 *out, s32 id, void *spawn);
void CScene__LoadLevel(void *world, s32 id, s32 flags);
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
        player = gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;
        if (player != NULL) {
            CPlayer__ResetControlState(player);
            if (gCinemaPlayerP->saveStart != 0 && gCinemaPlayerP->startLevel == -1
                && gCinemaPlayerP->startWarp == gCinemaPlayerP->startLevel) {
                gCinemaPlayerP->startLevel = gScene_m_LevelID[0];
                gCinemaPlayerP->startWarp = gScene_m_nLevel[0];
                gCinemaPlayerP->restored = 0;
                gCinemaPlayerP->startPos = player->m_CharacterActor.header.m_vPos;
                gCinemaPlayerP->startRotY = player->m_CharacterActor.m_RotY;
                gCinemaPlayerP->startRegion = CScene__GetRegionIndex(gScene, player->m_CharacterActor.header.m_pCurrentRegion);
            }
            player->m_WeaponActor.m_AI.m_VarBuffer[0] = 0;
            player->m_WeaponActor.m_AI.m_VarBuffer[1] = 0;
            player->m_WeaponActor.m_AI.m_VarBuffer[5] = 0;
        }
        CCameraPool__ReleaseAllTexts(gEngineApp_m_Game_m_CameraPool);
        RumblePool_FreeAll(gRumblePool);
    }
    if (warp != -1) {
        world = gScene;
        if (gScene_m_nLevel[0] != warp) {
            out[0] = 0;
            out[1] = 0;
            out[2] = 0;
            gScene_m_LevelID[0] = CScene__FindNearestWarpDest(world, out, warp, gScene_m_WarpSpawn);
            gScene_m_bWarpSpawn = gScene_m_LevelID[0] != 0;
            CScene__LoadLevel(world, warp, gScene_m_bWarpSpawn);
        }
    }
    gCinemaPlayerP->restoreAtEnd = Cinema_NoWaitCues();
    gCinemaPlayerP->unk50 = 1;
    gCinemaPlayerP->warpId = -1;
    gCinemaPlayerP->started = 1;
}
