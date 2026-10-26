#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Swing {
    u8 pad0[0x12];
    s8 step;                    /* 0x12: counts down */
    u8 pad13;
    s8 lastStep;                /* 0x14 */
    u8 pad15[0x18 - 0x15];
    Vec3 axis;                  /* 0x18 */
} Swing;

extern f32 D_800ACB98[];        /* swing amplitude per step */

void func_0020FF80(f32 *dst, f32 *src);
void func_0020EF60(Vec3 *dst, Vec3 *src, f32 scale);
void func_00210318(f32 *mtx, f32 x, f32 y, f32 z);
void func_0020FCC4(f32 *mtx, f32 angle, f32 x, f32 y, f32 z);
void func_0020C618(f32 *dst, f32 *a, f32 *b);

/* Rotates the matrix about its own origin by the swing angle of this step. */
void func_0021F7F4(s32 arg0, Swing *swing, f32 *matrix, s32 active, s32 dir) {
    Vec3 origin;
    Vec3 axis;
    f32 rotation[16];
    f32 inverse[16];
    f32 angle;
    f32 flip;

    if (active != 0) {
        origin.x = matrix[12];
        origin.y = matrix[13];
        origin.z = matrix[14];
        axis = swing->axis;
        angle = D_800ACB98[25 - swing->step];
        angle *= 0.3f;
        angle *= (dir == 5 ? -2.0f : dir);
        func_0020FF80(inverse, matrix);
        flip = -1.0f;
        func_0020EF60(&origin, &origin, flip);
        func_00210318(inverse, origin.x, origin.y, origin.z);
        func_0020FCC4(rotation, angle, axis.x, axis.y, axis.z);
        func_0020C618(matrix, inverse, rotation);
        func_0020EF60(&origin, &origin, flip);
        func_00210318(matrix, origin.x, origin.y, origin.z);
        if (swing->lastStep == active) {
            swing->step--;
        }
    }
}
