/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00409A18.c, func_00409A18.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/pak.h"
#include "globals/pak.h"

void PakMenu_SaveCharacter(CPlayer *a) {
    CPlayerOptions *p = a->m_pOptions;
    s32 i;
    gPakProfile.character = p->m_nCharacter;
    gPakProfile.suicides = p->m_Suicides;
    gPakProfile.deaths = p->m_Deaths;
    gPakProfile.kills = p->m_Kills;
    gPakProfile.tags = p->m_Tags;
    gPakProfile.reserved[0] = 0;
    gPakProfile.reserved[1] = 0;
    gPakProfile.reserved[2] = 0;
    for (i = 0; i < 8; i++) {
        gPakProfile.name[i] = p->m_Name[i];
    }
}
