/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00217594.c, func_00217594.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
f32 func_002119FC(f32, f32);
void func_00268C48(void *, s32, s32, s32, s32, f32);
extern f32 D_800A6994;
extern f32 D_800A6998;
typedef struct { u32 mask; s32 val; } B04Ent;
typedef struct { u8 pad0[4]; s32 x, y, z; } B04O; /* Turok 2: pos at 0x4 */
void func_00246B24(B04O *o, s32 a1, u32 mask, B04Ent *e) {
    for (; e->mask != 0; e++) {
        if (mask & e->mask) {
            func_00268C48(o, o->x, o->y, o->z, e->val, func_002119FC(D_800A6994, D_800A6998));
        }
    }
}
