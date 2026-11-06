#include "common.h"
#include "structs/vector.h"

typedef struct Swing {
    u8 pad0[0x12];
    s8 step;                    /* 0x12: counts down */
    u8 pad13;
    s8 lastStep;                /* 0x14 */
    u8 pad15[0x18 - 0x15];
    CVector3 axis;                  /* 0x18 */
} Swing;

extern f32 D_800ACB98[];        /* swing amplitude per step */

void CMtxF__Copy(f32 *dst, f32 *src);
void CVector3__Scale(CVector3 *dst, CVector3 *src, f32 scale);
void CMtxF__Translate(f32 *mtx, f32 x, f32 y, f32 z);
void CMtxF__SetAxisAngle(f32 *mtx, f32 angle, f32 x, f32 y, f32 z);
void CMtxF__MultiplyAffine(f32 *dst, f32 *a, f32 *b);

/* Rotates the matrix about its own origin by the swing angle of this step. */
void func_0021F7F4(s32 arg0, Swing *swing, f32 *matrix, s32 active, s32 dir) {
    CVector3 origin;
    CVector3 axis;
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
        CMtxF__Copy(inverse, matrix);
        flip = -1.0f;
        CVector3__Scale(&origin, &origin, flip);
        CMtxF__Translate(inverse, origin.x, origin.y, origin.z);
        CMtxF__SetAxisAngle(rotation, angle, axis.x, axis.y, axis.z);
        CMtxF__MultiplyAffine(matrix, inverse, rotation);
        CVector3__Scale(&origin, &origin, flip);
        CMtxF__Translate(matrix, origin.x, origin.y, origin.z);
        if (swing->lastStep == active) {
            swing->step--;
        }
    }
}
