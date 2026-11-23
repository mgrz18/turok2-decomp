/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B450C_de_6.c, func_802B450C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern Acmd *alResamplePull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);
extern s32 alResampleParam(void *filter, s32 paramID, void *param);
void alFilterNew(void *f, void *handler, void *setParam, s32 type);
/* the heap and the state block are host pointers (64-bit: the s32 versions truncated them) */
void *alHeapDBAlloc(u8 *file, s32 line, void *hp, s32 num, s32 size);
void alResampleNew(ALResampler *arg0, sptr arg1) {
    sptr result;
    f32 k;
    alFilterNew(arg0, alResamplePull, alResampleParam, 1);
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
