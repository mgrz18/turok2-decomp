/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00409A18.c, func_00409A18.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/pak.h"

extern PakProfile D_80130968;
extern u8 D_80130974[8];
void PakMenu_SaveCharacter(CPlayer *a) {
    CPlayerOptions *p = a->m_pOptions;
    s32 i;
    D_80130968.character = p->m_nCharacter;
    D_80130968.suicides = p->m_Suicides;
    D_80130968.deaths = p->m_Deaths;
    D_80130968.kills = p->m_Kills;
    D_80130968.tags = p->m_Tags;
    D_80130968.reserved[0] = 0;
    D_80130968.reserved[1] = 0;
    D_80130968.reserved[2] = 0;
    for (i = 0; i < 8; i++) {
        D_80130974[i] = p->m_Name[i];
    }
}
