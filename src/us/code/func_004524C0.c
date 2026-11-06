/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E830_de.c, func_8043E948_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void SingleMenu_Init_2(struct Shared_PakMenu *arg0);
extern void Controller_WatchPak(s32 kind);
extern void Pak_ProbeSlot_2(s32 kind);
extern void Controller_WatchPak(int);
extern void Controller_WatchPak(int arg0);
void SingleMenu_Init_2(struct Shared_PakMenu *arg0)
{
    s32 kind = arg0->owner->unk4;
    Controller_WatchPak(kind);
    Pak_ProbeSlot_2(kind);
}
