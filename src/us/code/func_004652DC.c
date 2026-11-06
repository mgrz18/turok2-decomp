/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/8044ACCC_13.CPP, func_8044CFF8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"

typedef struct { s32 field_0; } Shape_8011AADC;
extern Shape_8011AADC D_8011AADC;
extern void func_0042C338(char *);
extern void func_0042C38C(char *);

void CScene__InitPersistentData_2(CWorld *world, s32 level) {
    func_0042C338((char *)world);
    func_0042C38C((char *)world);
    world->m_PreviewState = 0;
    world->m_PreviewTimer = 0x22;
    world->m_PreviewOptions = D_8011AADC.field_0;
}
