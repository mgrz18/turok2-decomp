/* WRONG CUT: REAL CScene__InitPersistentData size 0x5C (splat cut its last word into func_0042C414). */
#include "common.h"
#include "structs/world.h"
#include "globals/game.h"


extern void CScene__LoadBitSections(void *menu);
extern void CScene__LoadBitSections2(void *menu);


void CScene__InitPersistentData(CScene *menu) {
    CScene__LoadBitSections(menu);
    CScene__LoadBitSections2(menu);
    menu->m_PreviewState = 0;
    menu->m_PreviewTimer = 34;
    menu->m_PreviewOptions = gEngineApp_m_Settings.unlockedCheats;
}
