/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002798D4.c, func_002798D4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef struct Pair16 {
                s16 a;
                s16 b;
} Pair16;
typedef struct PBuf {
    s16 n;
    Pair16 e[0x18];
} PBuf;
s32 func_0026D5E4(PBuf *b, s16 x, s16 y) {
    if (b->n == 0x18) {
        return 0;
    }
    b->e[b->n].a = x;
    b->e[b->n].b = y;
    b->n++;
    return 1;
}
