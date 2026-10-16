/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AD270_de_9.c, func_802AE93C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
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
