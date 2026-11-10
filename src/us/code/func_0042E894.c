/* CPlayer code: CPlayerPool__LevelStart, 0x0042E894-0x0042E91C (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/player.h"
#include "globals/game.h"


extern s32 SavedPlayers_IsActive(void);
extern void SavedPlayers_Deactivate(void);
extern void SavedPlayers_Restore(void);
extern void CPlayer__LevelStart(CPlayer *p);
extern void CPlayer__UpdateOrbitCamera(CPlayer *p, CPlayer *q);
extern void Cinema_UpdateCameraIfActive(sptr);

void CPlayerPool__LevelStart(CPlayerPool *list) {
    CPlayer *p;

    if (gEngineApp_m_Settings.mode == 13 && SavedPlayers_IsActive() != 0) {
        SavedPlayers_Restore();
        SavedPlayers_Deactivate();
    }
    for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        CPlayer__LevelStart(p);
        CPlayer__UpdateOrbitCamera(p, p);
        Cinema_UpdateCameraIfActive(p->m_pCamera);
    }
}
