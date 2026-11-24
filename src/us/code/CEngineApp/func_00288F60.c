#include "common.h"
#include "protos/scene.h"

#include "structs/game.h"
#define APP_SCENE T2_OFFSETOF(CEngineApp, m_Scene)
#define APP_ONSCREEN T2_OFFSETOF(CEngineApp, unk19440)
#define APP_GAME T2_OFFSETOF(CEngineApp, m_Game)
#define APP_CAMERAPOOL T2_OFFSETOF(CEngineApp, m_Game.m_CameraPool)
#define APP_AUTOMAP T2_OFFSETOF(CEngineApp, m_Game.m_Automap)

s32 COnScreen__DrawBossBars(sptr);                         /* extern */
s32 func_00281354(sptr);                         /* extern */
s32 CEngineApp__DrawFade(sptr);                         /* extern */
s32 Automap_Update(sptr);                         /* extern */
extern s32 Automap_IsShown(void *);

void CEngineApp__DrawWorld(sptr arg0) {
    sptr temp_s0;

    temp_s0 = arg0 + APP_AUTOMAP;
    Automap_Update(temp_s0);
    CScene__Update(arg0 + APP_SCENE);
    func_00281354(arg0 + APP_CAMERAPOOL);
    COnScreen__DrawBossBars(arg0 + APP_ONSCREEN);
    if (Automap_IsShown((void *) temp_s0) == 0) {
        CEngineApp__DrawFade(arg0);
    }
}
