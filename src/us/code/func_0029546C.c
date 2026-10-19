/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B450C_de_6.c, func_802B450C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float D_800AA520;
extern s32 func_002964D0;
extern s32 func_00296690;
void func_00296340(void *arg0, void *a, void *b, s32 c);
s32 func_002912A0(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0029546C(ALResampler_s *arg0, s32 arg1) {
    s32 result;
    f32 k;
    func_00296340(arg0, &func_002964D0, &func_00296690, 1);
    result = func_002912A0(0, 0, arg1, 1, 0x20);
    k = D_800AA520;
    arg0->state = (void *)result;
    arg0->delta = 0;
    arg0->first = 1;
    arg0->motion = 0;
    arg0->upitch = 0;
    arg0->ctrlList = 0;
    arg0->ctrlTail = 0;
    arg0->ratio = k;
}
