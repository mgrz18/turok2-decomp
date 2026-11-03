/* WRONG CUT: splat named this D_00206984 (0x206984, 428 bytes): its first 12 bytes are junk fill
 * ("m_YScale|" text), the real function starts at the 16-aligned 0x206990 (ROM symbol D_00206990), 416 bytes.
 * A load callback (func_0020793C/func_0020798C pass &D_00206990): loads a block of RGBA5551 textures and,
 * by the mode in the top two bits of the request word 2, recolours every pixel:
 * 1 swaps red and green, 2 copies red into green, 3 copies green into red. */

#include "common.h"

s32 func_00200B00(void **res, s32 flag);
u8 *func_002017D4(s32 *file, s32 index);                 /* address of a sub-block */
u16 *func_002017E8(s32 *table, s32 index, u32 *size);    /* address and size of an entry */

typedef struct {
    s32 **file;   /* 0x0 */
    s32 unk4;     /* 0x4 */
    u32 flags;    /* 0x8: top two bits = recolour mode */
} LoadRequest;

void func_00206990(s32 unused, LoadRequest *req) {
    s32 *file;
    s32 *table;
    u16 *pixels;
    u16 *pixel;
    s32 mode;
    u32 size;
    s32 count;
    s32 i;
    s32 j;
    s32 n;
    s32 type;
    u32 r;
    u32 g;
    u32 b;
    u32 a;

    if (func_00200B00((void **) req, 0) == 0) {
        return;
    }
    if ((req->flags >> 30) == 0) {
        return;
    }
    file = *req->file;
    mode = req->flags >> 30;
    if (*file == 0) {
        return;
    }
    type = *func_002017D4(file, 0);
    if (type >= 2) {
        return;
    }
    if (type < 0) {
        return;
    }
    table = (s32 *) func_002017D4(file, 2);
    count = *table;
    for (i = 0; i < count; i++) {
        pixels = func_002017E8(table, i, &size);
        n = size >> 1;
        for (j = 0; j < n; j++) {
            pixel = &pixels[j];
            r = (*pixel >> 8) & 0xF8;
            g = (*pixel >> 3) & 0xF8;
            b = (*pixel << 2) & 0xF8;
            a = *pixel & 1;
            switch (mode) {
            case 1:
                *pixel = (g << 8) | (r << 3) | (b >> 2) | a;
                break;
            case 2:
                *pixel = (r << 8) | (r << 3) | (b >> 2) | a;
                break;
            case 3:
                *pixel = (g << 8) | (g << 3) | (b >> 2) | a;
                break;
            }
        }
    }
}
