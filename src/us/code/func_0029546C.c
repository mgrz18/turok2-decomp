/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B450C_de_6.c, func_802B450C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 alResamplePull;
extern s32 alResampleParam;
void alFilterNew(void *arg0, void *a, void *b, s32 c);
s32 alHeapDBAlloc(s32 a, s32 b, s32 c, s32 d, s32 e);
void alResampleNew(ALResampler *arg0, s32 arg1) {
    s32 result;
    f32 k;
    alFilterNew(arg0, &alResamplePull, &alResampleParam, 1);
    result = alHeapDBAlloc(0, 0, arg1, 1, 0x20);
    k = 1.0f;
    arg0->state = (void *)result;
    arg0->delta = 0;
    arg0->first = 1;
    arg0->motion = 0;
    arg0->upitch = 0;
    arg0->ctrlList = 0;
    arg0->ctrlTail = 0;
    arg0->ratio = k;
}
