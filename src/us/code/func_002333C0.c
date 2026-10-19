/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A25D0_de_3.c, func_802A25D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float D_800A6440;
extern void *func_00235FF4(void *arg0, s32 arg1);
extern void func_002101A0(char *object, f32 *output);
extern void func_002101BC(void *arg0, f32 *arg1);
extern void func_0020D274(void *arg0, f32 *arg1);
extern void func_0020EF2C(Vec3 *arg0, Vec3 *arg1, Vec3 *arg2);
extern f32 func_00298470(f32);
void *func_002333C0(void *arg0, void *arg1, void *arg2) {
    Vec3 sp10;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 var_f1;
    void *temp_s1;
    char *var_s0;
    char *temp_s2;
    temp_s2 = func_00235FF4(arg0, (s32)arg1);
    var_s0 = temp_s2;
    if (temp_s2 != 0) {
        ((ObjectLinks54 *)(arg1))->unk_50 = 0;
        ((ObjectLinksB4 *)(var_s0))->unk_8 = ((struct ObjectState20 *) ((ObjectLinks54 *) arg1)->unk_8)->unk_8;
        ((ObjectLinksB4 *)(var_s0))->unk_B0 = 0;
        func_002101A0(arg2, &((ObjectLinksB4 *)(var_s0))->unk_10.v0);
        if (((ObjectLinks54 *)(arg1))->unk_3C & 4) {
            func_002101BC(arg2, &((ObjectLinksB4 *)(var_s0))->unk_1C);
        } else {
            func_0020D274(arg2, &((ObjectLinksB4 *)(var_s0))->unk_28);
            func_0020D274(arg2, &((ObjectLinksB4 *)(var_s0))->unk_68);
        }
        temp_s1 = *(void **)var_s0;
        ((ObjectLinksB4 *)(var_s0))->unk_AC.v0 = 0;
        ((ObjectLinksB4 *)(var_s0))->unk_A8 = 0.0f;
        if (temp_s1 != 0) {
            func_0020EF2C(&sp10, &((ObjectLinksB4 *)(var_s0))->unk_10.v1, &((ObjectStateB0 *)(temp_s1))->unk_10);
            temp_f0 = func_00298470((sp10.x * sp10.x) + (sp10.y * sp10.y) + (sp10.z * sp10.z));
            ((ObjectStateB0 *)(temp_s1))->unk_AC = temp_f0;
            ((ObjectLinks54 *)(arg1))->unk_4C += temp_f0;
        }
        if (((struct ObjectState20 *) ((ObjectLinks54 *) arg1)->unk_8)->unk_1C == 0) {
            if (((ObjectLinks54 *)(arg1))->unk_4C > 0.0f) {
                var_f1 = 0.0f;
                var_s0 = ((ObjectLinks54 *)(arg1))->unk_40;
                if (var_s0 != 0) {
                    do {
                        ((ObjectLinksB4 *)(var_s0))->unk_A8 = var_f1 / ((ObjectLinks54 *)(arg1))->unk_4C;
                        temp_f0_2 = ((ObjectLinksB4 *)(var_s0))->unk_AC.v1;
                        var_s0 = ((ObjectLinksB4 *)(var_s0))->unk_4;
                        var_f1 += temp_f0_2;
                    } while (var_s0 != 0);
                }
            }
        } else {
            if (temp_s1 != 0) {
                ((ObjectLinks54 *)(arg1))->unk_28 += ((ObjectStateB0 *)(temp_s1))->unk_AC * D_800A6440;
            }
            ((ObjectLinksB4 *)(var_s0))->unk_A8 = ((ObjectLinks54 *)(arg1))->unk_28;
        }
    }
    return temp_s2;
}
