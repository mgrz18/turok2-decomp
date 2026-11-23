/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/8044ACCC_13.CPP, func_8044CFF8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"
#include "globals/game.h"

extern void CScene__LoadBitSections_2(char *);
extern void CScene__LoadBitSections2_2(char *);

void CScene__InitPersistentData_2(CScene *world, s32 level) {
    CScene__LoadBitSections_2((char *)world);
    CScene__LoadBitSections2_2((char *)world);
    world->m_PreviewState = 0;
    world->m_PreviewTimer = 0x22;
    world->m_PreviewOptions = gEngineApp_m_Settings.unlockedCheats;
}
