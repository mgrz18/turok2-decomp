/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80409A88_5.CPP, func_80409F2C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern void Pak_ProbeSlot_2(s32);
void Pak_ProbeSlot_2(s32);
extern s32 D_800C2028;
extern void PakMenu_ProbeOwnerPort_2(struct Menu *record);
extern s32 D_80130990;
void PakMenu_ProbeOwnerPort_2(struct Menu *record) {
    s32 value;
    if (D_80130990 != 0) {
        value = D_800C2028;
    } else {
        value = record->controller->port;
    }
    Pak_ProbeSlot_2(value);
}
extern s32 D_80130990;
