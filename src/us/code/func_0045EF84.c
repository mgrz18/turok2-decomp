/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00404858.c, func_00404858.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/pak.h"

extern u8 D_800F6750;
s32 func_00285878(s32 flag);
void func_002847E0();
void func_002858A4(void);
s32 func_00425808(s32 ch);
s32 func_004276A0(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name);
s32 func_0045EF84(s32 pad, s32 idx) {
    s32 r;
    if (D_80130C60[pad] != 3) {
        return -2;
    }
    func_00285878(1);
    func_002847E0();
    r = D_80130C70[pad];
    D_800F6750 = 2;
    if (r == 0) {
        r = func_004276A0(&D_80130C80[pad], D_800C2224[pad].notes[idx].company_code, D_800C2224[pad].notes[idx].game_code, D_800C2224[pad].notes[idx].game_name, D_800C2224[pad].notes[idx].ext_name);
        if (r != 0) {
            r = -1;
        }
        func_00425808(pad);
    }
    func_002858A4();
    return r;
}
