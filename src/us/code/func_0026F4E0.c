/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80295FF4_de_8.c, func_80296A48_de.
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
struct Vector4f;
typedef struct Vector4f Vector4f;
struct Vector4f;
struct Vector4f {
    float x;
    float y;
    float z;
    float w;
};
s32 func_0026F4E0(Vector4f *planes, s32 n, Vec3 *points) {
    s32 i;
    s32 j;
    s32 inside;
    for (i = 0; i < 6; i++) {
        inside = 0;
        for (j = 0; j < n; j++) {
            if (planes[i].x * points[j].x + planes[i].y * points[j].y + planes[i].z * points[j].z <= planes[i].w) {
                inside = 1;
                break;
            }
        }
        if (!inside) {
            return 0;
        }
    }
    return 1;
}
