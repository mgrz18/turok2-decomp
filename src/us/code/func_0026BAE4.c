/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de_4.c, func_80275A58_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
extern void func_0026BAE4(void *arg0);
extern float D_800A7FE8;
void func_0026BAE4(void *arg0) {
    s32 *ip = (s32 *)arg0;
    f32 *fp = (f32 *)arg0;
    f32 scale = D_800A7FE8;
    fp[0] = (f32)ip[0] * scale;
    fp[1] = (f32)ip[1] * scale;
    fp[2] = (f32)ip[2] * scale;
    fp[3] = (f32)ip[3] * scale;
}
