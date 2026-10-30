#include "common.h"

/* A byte bitmap: width x height bytes after an 8-byte header. */
typedef struct {
    /* 0x0 */ s32 width;
    /* 0x4 */ s32 height;
    /* 0x8 */ u8 data[1];
} ByteImage;

extern void *func_0029E460(void *dst, void *src, s32 size); /* memcpy */

/* Transpose src into dst; with copyBack, copy the result back over src. */
void func_0020192C(ByteImage *src, ByteImage *dst, s32 copyBack) {
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
