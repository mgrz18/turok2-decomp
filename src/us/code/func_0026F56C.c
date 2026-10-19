/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00297AD4.c, func_00297AD4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef struct B21Plane {
    f32 x, y, z, w;
} B21Plane;
s32 func_0026F56C(B21Plane *pl, s32 n, Vec3f *pts) {
    s32 i;
    s32 j;
    s32 found;
    B21Plane *p;
    for (i = 0; i < 6; i++) {
        if (i == 2) {
            continue;
        }
        p = &pl[i];
        found = 0;
        for (j = 0; j < n; j++) {
            if (p->x * pts[j].x + p->y * pts[j].y + p->z * pts[j].z <= p->w) {
                found = 1;
                break;
            }
        }
        if (!found) {
            return 0;
        }
    }
    return 1;
}
