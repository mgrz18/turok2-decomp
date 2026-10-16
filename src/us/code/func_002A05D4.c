/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BE0D0_de_13.c, func_802BF514_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef float f32;
extern float func_002974C0(float x);
extern float func_002982F0(float x);
extern float D_80125430[];
extern void func_002A05D4();
extern float D_800AAC20;
extern float D_80125230[];
extern float D_80125030[];
extern float D_80125630[];
extern float D_800AAC28;
extern s32 D_80125830[];
void func_002A05D4(void) {
    s16 i;
    f32 angle;
    f32 s;
    f32 c;
    f32 step;
    f32 offset;
    f32 scale;
    f32 *sines;
    f32 *cosines;
    s32 *zeros;
    f32 *sums;
    f32 *differences;
    step = D_800AAC20;
    offset = (*(&D_800AAC20 + 1));
    scale = D_800AAC28;
    i = 0;
    sines = D_80125030;
    cosines = D_80125230;
    zeros = D_80125830;
    for (; i < 128; i++) {
        angle = (i * step + offset) * scale;
        sines[i] = func_002974C0(angle);
        cosines[i] = func_002982F0(angle);
        zeros[i] = 0;
    }
    i = 0;
    scale = (*(&D_800AAC28 + 1));
    sums = D_80125430;
    differences = D_80125630;
    for (; i < 128; i++) {
        angle = i * scale;
        s = func_002974C0(angle);
        c = func_002982F0(angle);
        sums[i] = s + c;
        differences[i] = c - s;
    }
}
