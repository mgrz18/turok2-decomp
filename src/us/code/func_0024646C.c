/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002170A0.c, func_002170A0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
f32 func_002119FC(f32, f32);
void func_00268C48(void *, s32, s32, s32, s32, f32);
extern f32 D_800A697C, D_800A6980, D_800A6984, D_800A6988;
typedef struct { u32 mask; s32 val; } B04Ent;
extern B04Ent D_800AD950[];
extern B04Ent D_800ADA58[];
typedef struct { u8 pad0[4]; s32 x, y, z; /* pos at 0x4 */ } B04O;
void func_0024646C(B04O *o, u32 *flags, u32 bit, u32 m1, u32 m2) {
    B04Ent *e;
    if (!(*flags & bit)) {
        *flags |= bit;
        for (e = D_800AD950; e->mask != 0; e++) {
            if (m1 & e->mask)
                func_00268C48(o, o->x, o->y, o->z, e->val, func_002119FC(D_800A697C, D_800A6980));
        }
        for (e = D_800ADA58; e->mask != 0; e++) {
            if (m2 & e->mask)
                func_00268C48(o, o->x, o->y, o->z, e->val, func_002119FC(D_800A6984, D_800A6988));
        }
    }
}
