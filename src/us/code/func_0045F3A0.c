/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80403BCC_7.CPP, func_80404E28_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/pak.h"

extern s32 Controller_Lock(s32 arg0);
extern u8 D_800F677B[];
extern void Controller_ReadAll(void);
extern u8 D_800F6750;
extern u8 D_800F677B[];
extern u8 D_800F6798[];
extern s32 osMotorInit(u8 *a0, OSPfs *a1, s32 a2);
extern s32 Pak_Mount_2(s32 a0);
extern void Controller_Unlock(void);
void Pak_ProbeSlot_2(s32 kind)
{
    RumblePak *cur;
    s32 ready;
    ((void (*)(s32))Controller_Lock)(1);
    Controller_ReadAll();
    D_800F6750 = 2;
    cur = &D_80130AA0[kind];
    if (D_800F677B[kind * 4] != 0) {
        cur->present = 0;
        ready = 0;
    } else {
        cur->present = osMotorInit(D_800F6798, &cur->pfs, kind) == 0;
        ready = cur->present;
    }
    if (ready != 0) {
        D_80130C60[kind] = 2;
    } else if (Pak_Mount_2(kind) != 0) {
        D_80130C60[kind] = 3;
    } else {
        D_80130C60[kind] = 1;
    }
    Controller_Unlock();
}
