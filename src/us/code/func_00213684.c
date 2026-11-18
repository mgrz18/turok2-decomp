#include "common.h"
#include "structs/vector.h"

s32 *CISet__GetBlock(s32 *file, s32 index); /* returns chunk `index` of a chunked file */

/* Adds count packed offsets (signed bytes x:16-23, y:8-15, z:0-7, scaled by 2) to points[]. */
void Model_ApplyPackedOffsets(s32 *file, s32 count, CVector3 *points) {
    s32 *chunk;
    s32 *packed;
    s32 word;
    s32 dx, dy, dz;
    s32 i;

    chunk = CISet__GetBlock(file, 1);
    if (*chunk != 0) {
        packed = CISet__GetBlock(chunk, 0) + 2;
        for (i = 0; i < count; i++) {
#ifdef TARGET_PORT
            /* Geometry block 1 is not in the archive's swap plan (tools/assets/swap_spec.json,
             * "geometry": op unk), so its words are still big-endian: read each as the N64 does.
             * A host load took the green offset for red, red for green and the zero top byte for
             * blue (round 20 i1: the Mother's arena drawn olive instead of brown-grey). */
            {
                const u8 *be = (const u8 *) &packed[i];
                word = (s32) (((u32) be[0] << 24) | ((u32) be[1] << 16) | ((u32) be[2] << 8) | be[3]);
            }
#else
            word = packed[i];
#endif
            dx = (s32) ((word & 0xFF0000) << 8) >> 23;
            dy = ((s32) (word << 16) >> 24) * 2;
            dz = (s32) (word << 24) >> 23;
            points[i].x += dx;
            points[i].y += dy;
            points[i].z += dz;
        }
    }
}
