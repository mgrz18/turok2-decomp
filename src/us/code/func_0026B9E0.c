/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de_3.c, func_80275954_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
void func_0026B9E0(f32 *arg0, s32 arg1, Vec3 *arg2)
{
    s32 i;
    i = 0;
    if (arg1 != 0) {
        arg0[0] = arg2->x;
        arg0[1] = arg2->y;
        arg0[2] = arg2->z;
        arg0[3] = arg2->x;
        arg0[4] = arg2->y;
        arg1 -= 1;
        arg0[5] = arg2->z;
        arg2 += 1;
        if (arg1 > 0) {
bounds_loop:
                {
                    f32 value = arg2->x;
                    f32 bound = arg0[0];
                    if (!(value <= bound)) {
                        value = bound;
                    }
                    arg0[0] = value;
                }
                {
                    f32 value = arg2->y;
                    f32 bound = arg0[1];
                    if (!(value <= bound)) {
                        value = bound;
                    }
                    arg0[1] = value;
                }
                {
                    f32 value = arg2->z;
                    f32 bound = arg0[2];
                    if (!(value <= bound)) {
                        value = bound;
                    }
                    arg0[2] = value;
                }
                {
                    f32 value = arg2->x;
                    f32 bound = arg0[3];
                    if (!(bound <= value)) {
                        value = bound;
                    }
                    arg0[3] = value;
                }
                {
                    f32 value = arg2->y;
                    f32 bound = arg0[4];
                    if (!(bound <= value)) {
                        value = bound;
                    }
                    arg0[4] = value;
                }
                {
                    f32 value = arg2->z;
                    f32 bound = arg0[5];
                    if (!(bound <= value)) {
                        value = bound;
                    }
                    arg0[5] = value;
                }
                i += 1;
                arg2 += 1;
            if (i < arg1) goto bounds_loop;
        }
    }
}
