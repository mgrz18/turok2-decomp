/* Second VM module copy of CPlayerPool__LevelStart (same source, linked 0x0042E7B4). */
#include "common.h"
#include "structs/player.h"

extern s32 D_8011AC88; /* game state */

extern s32 SavedPlayers_IsActive(void);
extern void SavedPlayers_Deactivate(void);
extern void SavedPlayers_Restore(void);
extern void func_0042DD94(CPlayer *p);
extern void CPlayer__UpdateOrbitCamera(CPlayer *p, CPlayer *q);
extern void Cinema_UpdateCameraIfActive(s32);

void CPlayerPool__LevelStart_2(CPlayerPool *list) {
    CPlayer *p;

    if (D_8011AC88 == 13 && SavedPlayers_IsActive() != 0) {
        SavedPlayers_Restore();
        SavedPlayers_Deactivate();
    }
    for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        func_0042DD94(p);
        CPlayer__UpdateOrbitCamera(p, p);
        Cinema_UpdateCameraIfActive(p->m_pCamera);
    }
}
