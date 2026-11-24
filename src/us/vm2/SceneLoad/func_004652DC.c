/* WRONG CUT: REAL CScene__InitPersistentData_2 size 0x5C (splat cut its last word into func_0042C414). */
#include "common.h"
#include "structs/world.h"
#include "globals/game.h"


extern void CScene__LoadBitSections_2(void *menu);
extern void CScene__LoadBitSections2_2(void *menu);


void CScene__InitPersistentData_2(CScene *menu) {
    CScene__LoadBitSections_2(menu);
    CScene__LoadBitSections2_2(menu);
    menu->m_PreviewState = 0;
    menu->m_PreviewTimer = 34;
    menu->m_PreviewOptions = gEngineApp_m_Settings.unlockedCheats;
}
