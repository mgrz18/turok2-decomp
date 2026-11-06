/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80442A58_de_3.c, func_80444314_eu.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/menu.h"
#include "globals/scene.h"

extern s32 Level_GetHub(s32);
void MissionMenu_InitFailed(void) {
    sMissionHub = Level_GetHub(gScene_m_LevelID);
    sMissionFailed = 1;
}
