/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80409A88_5.CPP, func_80409F2C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/pak.h"

extern void Pak_ProbeSlot_2(s32);
void Pak_ProbeSlot_2(s32);
extern void PakMenu_ProbeOwnerPort_2(struct Menu *record);
void PakMenu_ProbeOwnerPort_2(struct Menu *record) {
    s32 value;
    if (gPakMenuAnyPak != 0) {
        value = gPakMenuPort;
    } else {
        value = record->controller->port;
    }
    Pak_ProbeSlot_2(value);
}
