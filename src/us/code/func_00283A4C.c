#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/cinema.h"
#include "globals/scene.h"


extern Cinema *gCinemaPlayerP;
extern CPlayer *gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;          /* current player */
extern s32 gEngineApp_m_Settings_mode;
extern s32 gScene_m_nLevel;              /* world +0x18FA8: the current warp id */
extern s32 gScene_m_WarpFade;
extern s32 gScene_m_WarpState;
extern s32 gScene_m_WarpTime;
extern s32 gScene_m_LevelID;              /* world +0x18FD0: selectionValues */
extern s32 gScene_m_WarpID;

void CScene__LoadLevel(void *world, s32 id, s32 flags);
s32 CScene__GetRegion(void *world, s32 region);
void CPlayer__UpdateOrbitCamera(CPlayer *player, CPlayer *other);

/* Puts the current player back where Cinema_Start saw it start (once,
 * when gEngineApp_m_Settings_mode is 13 and a start was saved), warping there first if
 * needed; any other gEngineApp_m_Settings_mode drops the saved start. */
void Cinema_RestorePlayer(void) {
    void *world;
    CPlayer *player;
    CVector3 pos;
    f32 rotY;
    s32 region;
    s32 level;
    s32 warp;

    world = &gScene;
    level = gCinemaPlayerP->startLevel;
    warp = gCinemaPlayerP->startWarp;
    if (gEngineApp_m_Settings_mode != 13) {
        gCinemaPlayerP->restored = 1;
        gCinemaPlayerP->saveStart = 0;
        gCinemaPlayerP->startLevel = -1;
        gCinemaPlayerP->startWarp = -1;
        return;
    }
    if (gCinemaPlayerP->restored == 0 && level != -1 && warp != -1 && gCinemaPlayerP->saveStart != 0) {
        gCinemaPlayerP->restored = 1;
        pos = gCinemaPlayerP->startPos;
        rotY = gCinemaPlayerP->startRotY;
        region = gCinemaPlayerP->startRegion;
        if (gScene_m_nLevel != warp) {
            CScene__LoadLevel(world, warp, 1);
        }
        player = gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;
        gScene_m_WarpFade = 0;
        gScene_m_WarpState = 0;
        gScene_m_WarpTime = 0;
        gScene_m_LevelID = level;
        gScene_m_WarpID = level;
        if (player != NULL) {
            player->m_CharacterActor.header.m_vPos = pos;
            player->m_CharacterActor.m_RotY = rotY;
            player->m_CharacterActor.header.m_pCurrentRegion = CScene__GetRegion(world, region);
            CPlayer__UpdateOrbitCamera(player, player);
        }
        gCinemaPlayerP->saveStart = 0;
        gCinemaPlayerP->startLevel = -1;
        gCinemaPlayerP->startWarp = -1;
    }
}
