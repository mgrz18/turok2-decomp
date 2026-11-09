#include "common.h"

#ifdef TARGET_PORT /* host layout of CEngineApp (structs/game.h) */
#include "structs/game.h"
#define APP_SCENE T2_OFFSETOF(CEngineApp, m_Scene)
#define APP_ONSCREEN T2_OFFSETOF(CEngineApp, unk19440)
#define APP_GAME T2_OFFSETOF(CEngineApp, m_Game)
#define APP_CAMERAPOOL T2_OFFSETOF(CEngineApp, m_Game.m_CameraPool)
#define APP_AUTOMAP T2_OFFSETOF(CEngineApp, m_Game.m_Automap)
#else
#define APP_SCENE 0x3C8
#define APP_ONSCREEN 0x19440
#define APP_GAME 0x22BC0
#define APP_CAMERAPOOL 0x22C00
#define APP_AUTOMAP 0x23DF0
#endif

s32 CPlayerPool__NewGame(sptr, s32, s32);       /* extern */
s32 CCameraPool__NewGame(sptr);                         /* extern */

void func_002888BC(sptr arg0, s32 arg1, s32 arg2) {
    CCameraPool__NewGame(arg0 + APP_CAMERAPOOL);
    CPlayerPool__NewGame(arg0 + APP_GAME, arg1, arg2);
}
