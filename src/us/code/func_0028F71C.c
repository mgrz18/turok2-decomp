/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AD270_de_9.c, func_802AE7BC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
typedef double f64;
struct ObjRecAt40V1;
typedef struct ObjRecAt40V1 ObjRecAt40V1;
struct ObjRecAt40V1;
struct ObjRecAt40V1 {
    char p[0x40];
    s32 unk40;
};
extern double D_800AA258;
extern f32 func_0028F71C(void **arg0, s32 arg1, s32 arg2);
extern double D_800AA250;
extern float D_800AA260;
struct ObjRecAt40V1;
f32 func_0028F71C(void **arg0, s32 arg1, s32 arg2) {
    f64 angle;
    f32 arg1f;
    f32 scaled;
    s32 temp_v0;
    f64 val;
    angle = (f64)arg2;
    arg1f = (f32)arg1;
    if (arg2 < 0) {
        angle += D_800AA250;
    }
    temp_v0 = ((ObjRecAt40V1 *)((*arg0)))->unk40;
    scaled = arg1f * (f32)angle;
    val = (f64)temp_v0;
    if (temp_v0 < 0) {
        val += D_800AA258;
    }
    return scaled / ((f32)val * D_800AA260);
}
