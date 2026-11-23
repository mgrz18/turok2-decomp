#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"

#include "structs/game.h"
#define APP_SCENE T2_OFFSETOF(CEngineApp, m_Scene)
#define APP_ONSCREEN T2_OFFSETOF(CEngineApp, unk19440)
#define APP_GAME T2_OFFSETOF(CEngineApp, m_Game)
#define APP_CAMERAPOOL T2_OFFSETOF(CEngineApp, m_Game.m_CameraPool)
#define APP_AUTOMAP T2_OFFSETOF(CEngineApp, m_Game.m_Automap)

s32 Cinema_RequestById(s32);                         /* extern */
s32 CPlayerPool__NewGame(sptr, s32, s32);       /* extern */
s32 CCameraPool__NewGame(sptr, s32);                /* extern */

void CEngineApp__StartWarp(sptr arg0, s32 arg1) {
    CCameraPool__NewGame(arg0 + APP_CAMERAPOOL, 1);
    CPlayerPool__NewGame(arg0 + APP_GAME, 1, 0);
    CScene__RequestWarp(&gScene, ~Cinema_RequestById(arg1), 0);
    CScene__LoadBitSections(&gScene);
    CScene__LoadBitSections2(&gScene);
}
