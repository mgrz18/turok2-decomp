/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AD270_de_9.c, func_802AE8C8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct CopyDest802B3998;
typedef struct CopyDest802B3998 CopyDest802B3998;
struct CopySource802B3998;
struct CopySource802B3998 {
    int word0;
    int word4;
    int word8;
    int wordsC[16];
    int words4C[16];
    unsigned char bytes8C[16];
    unsigned char bytes9C[16];
    int wordsAC[16];
};
struct CopySource802B3998;
typedef struct CopySource802B3998 CopySource802B3998;
struct CopyDest802B3998;
struct CopyDest802B3998 {
    int pad0;
    int word4;
    int pad8;
    int wordC;
    int word10;
    int pad14;
    int words18[16];
    int words58[16];
    unsigned char bytes98[16];
    unsigned char bytesA8[16];
    int wordsB8[16];
};
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
