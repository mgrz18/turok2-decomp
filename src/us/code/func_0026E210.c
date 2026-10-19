/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802773D4_de_15.c, func_80278E7C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00225354(struct WorldRecAt1B40C *arg0, Record14 *arg1);
extern Shared_World D_800F7078;
extern s32 func_00224A68(void *arg0, void *arg1, s32 arg2);
void func_0026E210(void *arg0) {
    extern s32 func_0026DFB0(void *arg0);
    func_80278EEC_S1 *object = arg0;
    object->unkE |= 0x10;
    func_00224A68(&D_800F7078, arg0, 1);
    if ((D_800F7078.mode == 4) && (object->unkE & 2)) {
        func_00225354(&D_800F7078, arg0);
        return;
    }
    func_0026DFB0(arg0);
}
