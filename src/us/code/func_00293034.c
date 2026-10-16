/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B6F50.c, func_802B20D4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef double f64;
struct ObjRecAt18V1;
struct ObjRecAt18V1 {
    char pad18[0x18];
    s16 field18;
};
struct ObjRecAt18V1;
typedef struct ObjRecAt18V1 ObjRecAt18V1;
extern double D_800AA428;
extern f32 func_00293034(ObjRecAt18V1 *arg0, s32 arg1, s32 arg2);
extern f32 D_800AA430;
extern const float D_800AA430;
f32 func_00293034(ObjRecAt18V1 *arg0, s32 arg1, s32 arg2) {
    f64 v;
    f32 new_var;
    v = (f64)arg2;
    new_var = (f32)arg1;
    if (arg2 < 0) {
        v = v + D_800AA428;
    }
    return (new_var * (f32)v) / ((f32)arg0->field18 * D_800AA430);
}
