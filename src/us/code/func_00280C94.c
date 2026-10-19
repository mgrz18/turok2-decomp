/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80233930_de_11.c, func_80238F24_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00220A1C(void *, void *);
extern float D_800A9BDC;
extern float D_800A9BE4;
extern float D_800A9BE0;
extern Shared_World D_800F7078;
extern s32 Cinema_PathPlaying(void);
extern f32 func_0026BE60(s32 arg0, f32 arg1, f32 arg2);
extern s32 D_800B6D00;
void func_00280C94(GameplayCamera *arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6,
                   f32 arg7, f32 arg8, f32 arg9, Shared_Quad arg10, s32 arg11, f32 arg12,
                   f32 arg13) {
    s32 object;
    f32 value;
    f32 x;
    f32 y;
    f32 z;
    f32 w;
    arg0->unk24 = arg1;
    arg0->unk28 = arg2;
    arg0->unk2C = arg3;
    arg0->unk30 = arg4;
    arg0->unk34 = arg5;
    arg0->unk38 = arg6;
    arg0->unk3C = arg7;
    arg0->unk40 = arg8;
    arg0->unk44 = arg9;
    arg0->unk48 = arg10;
    arg0->unk58 = (void *)arg11;
    arg0->unk5C = arg12;
    arg0->unk60 = arg13;
    if (Cinema_PathPlaying() != 0) {
        arg0->unk58 = (void *)func_00220A1C(&D_800F7078, &arg0->unk38);
    }
    object = (s32)arg0->unk58;
    x = arg0->unk38;
    y = arg0->unk3C;
    z = arg0->unk40;
    w = arg0->unk44;
    arg0->unk64 = D_800B6D00;
    if (object != 0 && Cinema_PathPlaying() == 0) {
        value = ((y + w) - func_0026BE60(object, x, z)) * D_800A9BDC;
        if (value < D_800A9BE0 && D_800A9BE4 < value) {
            arg0->unk64 = ((MenuRules *)(object))->locked;
        }
    }
    if (Cinema_PathPlaying() != 0) {
        arg0->unk64 = D_800B6D00;
    }
    if (arg0->unk5C > 0.0f) {
        arg0->unk64 = D_800B6D00;
    }
}
