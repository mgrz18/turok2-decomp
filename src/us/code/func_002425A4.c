#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Quad {
    Vec3 corner[4]; /* 0x00 */
    Vec3 normal;    /* 0x30 */
} Quad;

void func_00241960(void *object, Vec3 *points, s32 arg2);
void func_0023C9CC(Quad *quad, Vec3 *points);
f32 func_0023C8A0(Quad *quad, f32 x, f32 z);

/* Height at (x, z) of the face built from points 0-3 of the object's box. */
f32 func_002425A4(void *object, f32 x, f32 z) {
    Vec3 points[8];
    Quad quad;

    func_00241960(object, points, 0);
    func_0023C9CC(&quad, points);
    return func_0023C8A0(&quad, x, z);
}
