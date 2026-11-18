/* WRONG CUT: splat named this D_00206984 (0x206984, 428 bytes): its first 12 bytes are junk fill
 * ("m_YScale|" text), the real function starts at the 16-aligned 0x206990 (ROM symbol D_00206990), 416 bytes.
 * A load callback (Texture_RequestSync/Texture_Request pass &D_00206990): loads a block of RGBA5551 textures and,
 * by the mode in the top two bits of the request word 2, recolours every pixel:
 * 1 swaps red and green, 2 copies red into green, 3 copies green into red. */

#include "common.h"

s32 ProPackDecompress(void **res, s32 flag);
u8 *CISet__GetBlock(s32 *file, s32 index);                 /* address of a sub-block */
u16 *CISet__GetBlockAndSize(s32 *table, s32 index, u32 *size);    /* address and size of an entry */

#ifdef TARGET_PORT
#include "structs/cache.h"
/* the request is the CCacheEntry being completed (CDecompressor__Main passes it): the view at
 * its host offsets (round 19 h2). The N64 view read the flags at 0x8, which on the host is the
 * high half of referencedMemEntry (0): no texture was ever recoloured on the port. */
typedef struct {
    union {
        struct { s32 **file; }; /* memEntry: its first word is the data pointer (N64 0x0) */
        struct { u8 _hp_flags[__builtin_offsetof(CCacheEntry, requestId)]; u32 flags; }; /* N64 0x8 */
    };
} LoadRequest;
#else
typedef struct {
    s32 **file;   /* 0x0 */
    s32 unk4;     /* 0x4 */
    u32 flags;    /* 0x8: top two bits = recolour mode */
} LoadRequest;
#endif

void Texture_DecompressCallback(s32 unused, LoadRequest *req) {
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

    if (ProPackDecompress((void **) req, 0) == 0) {
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
    type = *CISet__GetBlock(file, 0);
    if (type >= 2) {
        return;
    }
    if (type < 0) {
        return;
    }
    table = (s32 *) CISet__GetBlock(file, 2);
    count = *table;
    for (i = 0; i < count; i++) {
        pixels = CISet__GetBlockAndSize(table, i, &size);
        n = size >> 1;
        for (j = 0; j < n; j++) {
            pixel = &pixels[j];
#ifdef TARGET_PORT
            /* round 19 h2: the TLUT entries stay big-endian in the port's memory
             * (tools/assets/swap_spec.json "rdp": Fast3D reads them as bytes), so the recolour
             * reads and writes each RGBA5551 entry big-endian; a host u16 access would take the
             * green/blue byte for red and write the entry back byte-swapped. */
            {
                u8 *be = (u8 *) pixel;
                u32 v = ((u32) be[0] << 8) | be[1];
                r = (v >> 8) & 0xF8;
                g = (v >> 3) & 0xF8;
                b = (v << 2) & 0xF8;
                a = v & 1;
                switch (mode) {
                case 1:
                    v = (g << 8) | (r << 3) | (b >> 2) | a;
                    break;
                case 2:
                    v = (r << 8) | (r << 3) | (b >> 2) | a;
                    break;
                case 3:
                    v = (g << 8) | (g << 3) | (b >> 2) | a;
                    break;
                }
                be[0] = (u8) (v >> 8);
                be[1] = (u8) v;
            }
#else
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
#endif
        }
    }
}
