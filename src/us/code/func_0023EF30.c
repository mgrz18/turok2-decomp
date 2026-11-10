/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/802ABDBC_16.CPP, func_802AD0B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "globals/pickup.h"

#ifdef TARGET_PORT
/* Host body: the rows are CPickupRow18 (globals/pickup.h), 0x18 bytes on the N64 only */
void *Pickup_FindWeaponEntry(s32 arg0) {
    CPickupRow18 *row = sWeaponPickups;
    s32 i = 0x1E;

    do {
        i -= 1;
        if (row->m_Id == arg0) {
            return row;
        }
        row++;
    } while (i != -1);
    return 0;
}
#else
typedef struct Shared__struct_D_800D34C0_0x18 Shared__struct_D_800D34C0_0x18;
struct Shared__struct_D_800D34C0_0x18 {
    char pad0[4];
    s16 id;
    char pad6[6];
    s16 unkC;
    s16 unkE;
    s16 unk10;
    char pad12[0x6];
};
void *Pickup_FindWeaponEntry(s32 arg0) {
    char *v1;
    s32 i;
    Shared__struct_D_800D34C0_0x18 *record;
    v1 = (char *) sWeaponPickups;
    i = 0x1E;
    do {
        record = (Shared__struct_D_800D34C0_0x18 *)v1;
        i -= 1;
        if (record->id != arg0) {
            v1 += 0x18;
        } else {
            return v1;
        }
    } while (i != -1);
    return 0;
}
#endif
