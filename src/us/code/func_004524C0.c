/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E830_de.c, func_8043E948_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_004524C0(struct Shared_PakMenu *arg0);
extern void func_00285A68(s32 kind);
extern void func_004263A0(s32 kind);
extern void func_00285A68(int);
extern void func_00285A68(int arg0);
void func_004524C0(struct Shared_PakMenu *arg0)
{
    s32 kind = arg0->owner->unk4;
    func_00285A68(kind);
    func_004263A0(kind);
}
