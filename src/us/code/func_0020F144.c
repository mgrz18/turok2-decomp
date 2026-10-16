/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80271C54_de.c, func_80272180_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
extern void *func_0020F144(Vec3 *arg0, Vec3 *arg1, Vec3 *arg2, f32 arg3);
struct Vec3;
void *func_0020F144(Vec3 *arg0, Vec3 *arg1, Vec3 *arg2, f32 arg3) {
    Vec3 tmp;
    f32 temp_f2;
    f32 temp_f3;
    f32 temp_f4;
    f32 temp_f5;
    temp_f5 = arg1->x;
    temp_f4 = arg2->x;
    temp_f3 = (temp_f5 * temp_f4) + (arg1->y * arg2->y) + (arg1->z * arg2->z);
    temp_f2 = (arg3 * temp_f3) + temp_f3;
    tmp.x = temp_f5 - (temp_f2 * temp_f4);
    tmp.y = arg1->y - (temp_f2 * arg2->y);
    tmp.z = arg1->z - (temp_f2 * arg2->z);
    *arg0 = tmp;
    return arg0;
}
