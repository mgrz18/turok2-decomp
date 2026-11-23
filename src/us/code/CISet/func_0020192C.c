#include "common.h"
#include "structs/table.h"
#include "protos/iset.h"

extern void *func_0029E460(void *dst, void *src, s32 size); /* memcpy */

/* Transpose src into dst; with copyBack, copy the result back over src. */
void CUSet__Transpose(ByteImage *src, ByteImage *dst, s32 copyBack) {
    u8 *in = src->data;
    u8 *out;
    s32 width, height;
    s32 x, y;

    func_0029E460(dst, src, 8);
    out = dst->data;
    height = src->height;
    width = src->width;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            *out++ = in[x * height + y];
        }
    }
    if (copyBack) {
        func_0029E460(src, dst, height * width + 8);
    }
}
