/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002168B8.c, func_002168B8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
f32 func_00244E84(void *, s32, s32, s32, void *, f32);
void func_00246440(void *, f32);
extern f32 D_800A696C;
extern f32 D_800B6D28;
typedef struct { u8 pad0[4]; s32 x, y, z; } B04Tg; /* Turok 2: pos at 0x4 */
void func_00245C5C(void *o, s32 a1, B04Tg *t, f32 lim) {
    f32 r, v, a;
    if (t != ((void *)0)) {
        r = func_00244E84(o, t->x, t->y, t->z, t, D_800A696C);
        v = r;
        if (lim < r) r = lim;
        else if (r < -lim) r = -lim;
        r = r * D_800B6D28;
        a = r;
        if (a < 0.0f) a = -a;
        if (v < 0.0f) {
            if (-v < a) goto blk;
        } else if (v < a) {
blk:
            r = v;
        }
        func_00246440(o, r);
    }
}
