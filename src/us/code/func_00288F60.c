#include "common.h"
#include "protos/scene.h"

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

s32 COnScreen__DrawBossBars(s32);                         /* extern */
s32 func_00281354(s32);                         /* extern */
s32 CEngineApp__DrawFade(s32);                         /* extern */
s32 Automap_Update(s32);                         /* extern */
s32 Automap_IsShown(s32);                             /* extern */

void CEngineApp__DrawWorld(s32 arg0) {
    s32 temp_s0;

    temp_s0 = arg0 + APP_AUTOMAP;
    Automap_Update(temp_s0);
    CScene__Update(arg0 + APP_SCENE);
    func_00281354(arg0 + APP_CAMERAPOOL);
    COnScreen__DrawBossBars(arg0 + APP_ONSCREEN);
    if (Automap_IsShown(temp_s0) == 0) {
        CEngineApp__DrawFade(arg0);
    }
}
