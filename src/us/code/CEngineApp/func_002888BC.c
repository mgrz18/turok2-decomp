#include "common.h"

#include "structs/game.h"
#define APP_SCENE T2_OFFSETOF(CEngineApp, m_Scene)
#define APP_ONSCREEN T2_OFFSETOF(CEngineApp, unk19440)
#define APP_GAME T2_OFFSETOF(CEngineApp, m_Game)
#define APP_CAMERAPOOL T2_OFFSETOF(CEngineApp, m_Game.m_CameraPool)
#define APP_AUTOMAP T2_OFFSETOF(CEngineApp, m_Game.m_Automap)

extern void CPlayerPool__NewGame(void *list, s32 count, s32 arg2);
extern void CCameraPool__NewGame(void *owner, s32 count);

void CEngineApp__NewGame(sptr arg0, s32 arg1, s32 arg2) {
#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): a1 passes through to CCameraPool__NewGame on MIPS */
    CCameraPool__NewGame((void *) (arg0 + APP_CAMERAPOOL), arg1);

#else
    ((void (*)(void *)) CCameraPool__NewGame)((void *) (arg0 + APP_CAMERAPOOL)); /* a1 passes through */

#endif
    CPlayerPool__NewGame((void *) (arg0 + APP_GAME), arg1, arg2);
}
