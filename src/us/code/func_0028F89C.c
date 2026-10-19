/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AD270_de_9.c, func_802AE93C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0028F89C(CopyDest802B3998 *src, CopySource802B3998 *dst);
void func_0028F89C(CopyDest802B3998 *src, CopySource802B3998 *dst) {
    s32 i;
    dst->word0 = src->word4;
    dst->word4 = src->wordC;
    dst->word8 = src->word10;
    for (i = 0; i < 16; i++) {
        dst->wordsC[i] = src->words18[i];
        dst->words4C[i] = src->words58[i];
        dst->bytes8C[i] = src->bytes98[i];
        dst->bytes9C[i] = src->bytesA8[i];
        dst->wordsAC[i] = src->wordsB8[i];
    }
}
