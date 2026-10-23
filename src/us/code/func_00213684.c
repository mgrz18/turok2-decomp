#include "common.h"

s32 *func_002017D4(s32 *file, s32 index); /* returns chunk `index` of a chunked file */

typedef struct {
    f32 x, y, z;
} Vec3f;

/* Adds count packed offsets (signed bytes x:16-23, y:8-15, z:0-7, scaled by 2) to points[]. */
void func_00213684(s32 *file, s32 count, Vec3f *points) {
    s32 *chunk;
    s32 *packed;
    s32 word;
    s32 dx, dy, dz;
    s32 i;

    chunk = func_002017D4(file, 1);
    if (*chunk != 0) {
        packed = func_002017D4(chunk, 0) + 2;
        for (i = 0; i < count; i++) {
            word = packed[i];
            dx = (s32) ((word & 0xFF0000) << 8) >> 23;
            dy = ((s32) (word << 16) >> 24) * 2;
            dz = (s32) (word << 24) >> 23;
            points[i].x += dx;
            points[i].y += dy;
            points[i].z += dz;
        }
    }
}
