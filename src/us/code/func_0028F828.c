/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AD270_de_9.c, func_802AE8C8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0028F828(CopyDest802B3998 *dst, CopySource802B3998 *src);
void func_0028F828(CopyDest802B3998 *dst, CopySource802B3998 *src) {
    int i;
    dst->word4 = src->word0;
    dst->wordC = src->word4;
    dst->word10 = src->word8;
    for (i = 0; i < 16; i++) {
        dst->words18[i] = src->wordsC[i];
        dst->words58[i] = src->words4C[i];
        dst->bytes98[i] = src->bytes8C[i];
        dst->bytesA8[i] = src->bytes9C[i];
        dst->wordsB8[i] = src->wordsAC[i];
    }
}
