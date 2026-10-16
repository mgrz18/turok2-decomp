/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80262480_de_3.c, func_802625F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct AnimRef {
               char pad0[4];
               u16 id;
               u16 frame;
               u16 count;
               u8 flags[2];
               u32 span;
               void **data;
} AnimRef;
extern f32 func_00210DB4(f32,f32,f32);
typedef struct ResidentDataChunk {
    s32 width;
    s32 count;
    u8 data[1];
} ResidentDataChunk;
extern float D_800A5AB8;
extern void *func_0021793C(void *arg0);
extern f32 func_00210DB4(f32, f32, f32);
void func_00219A20(AnimRef *arg0, f32 arg1) {
    extern char *func_002017D4(s32 *, s32);
    s16 *base;
    s32 count;
    s32 temp_f2;
    s32 temp_a0;
    s32 var_v1;
    f32 b_term;
    f32 e_term;
    base = (s16 *)((ResidentDataChunk *)func_002017D4(func_0021793C(arg0), 0))->data;
    count = (s16)arg0->count;
    var_v1 = count - 1;
    temp_f2 = (s32) arg1;
    temp_a0 = temp_f2 + 1;
    if (temp_a0 < var_v1) {
        var_v1 = temp_a0;
    }
    b_term = (f32) base[temp_f2] * D_800A5AB8;
    e_term = (f32) base[var_v1] * D_800A5AB8;
    func_00210DB4(arg1 - (f32) temp_f2, b_term, e_term);
}
