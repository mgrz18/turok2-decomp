/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80246E44_de_13.c, func_8024B3A8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_002242F8(void *arg0, s32 arg1);
extern void func_0021E6AC(void *obj, InputRecAt27 *rec, s32 mode, s32 index);
extern f32 D_800A5CEC[];
extern Shared_World D_800F7078;
extern f32 D_800A5CEC[];
extern void *func_00225E88(void *, s32);
extern s32 func_00220408(void *, void *);
extern void func_00219BC0(void *, u16, u16, s32, void *, s32, s32, f32,
                          Vec3, u8, Vec3, Vec3, s32);
void func_0021E6AC(void *arg0, InputRecAt27 *arg1, s32 mode, s32 index) {
    void *resource0;
    s32 resource1;
    s32 lookup;
    s32 amountRaw;
    f32 fzero;
    f32 amount;
    Vec3 zero;
    resource0 = func_00225E88(&D_800F7078, arg1->resource22);
    if (arg1->resource20 == 0xFFFF) {
        resource1 = 0;
    } else {
        resource1 = func_002242F8(&D_800F7078, arg1->resource20);
    }
    amountRaw = arg1->amount24;
    fzero = 0.0f;
    amount = amountRaw * D_800A5CEC[1];
    zero.x = zero.y = zero.z = fzero;
    lookup = func_00220408(&D_800F7078, arg0);
    func_00219BC0(arg0, arg1->unk1C, arg1->unk1E, arg1->unk0,
                  resource0, arg1->unk27 == 0xFF ? -1 : arg1->unk27,
                  resource1, amount, *(Vec3 *)arg1->vec4, arg1->unk26,
                  *(Vec3 *)arg1->vec10, zero, lookup);
}
