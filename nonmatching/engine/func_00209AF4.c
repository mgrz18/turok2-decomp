/* NON-MATCHING: 414 of 530 words differ: same control flow and instruction mix, but the ROM keeps extra copies of the table pointers (lod/part tables reloaded from two stack slots each, a register copy in the vertex loop; frame 0xA8 vs 0xA0) that this C does not produce, so register allocation and scheduling differ throughout. */
/* Close in structure to thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0026AD8C.c, with Turok 2's extra level-of-detail table. */
#include "common.h"

typedef struct Gfx {
    u32 w0;
    u32 w1;
} Gfx;

#define GFX_W(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->w0 = (a); _g->w1 = (b); }

/* Bytes of an offset table of n entries: the count, n offsets and the next free offset, 8-aligned. */
#define TABLE_SIZE(n) (((n) * 4 + 0xF) & ~7)

s32 func_00200B00(void ***handle, s32 arg1);
void *func_002017D4(void *table, s32 index);
void *func_002017E8(void *table, s32 index, s32 *size);
s32 func_0020192C(void *src, void *dst, s32 arg2);
void **func_00205D7C(s32 heap, void ***handle, s32 size);
void func_002057B0(s32 heap, void ***handle, void **block);
void func_0026D070(void *dst, void *material);
void *func_0029E460(void *dst, const void *src, u32 n); /* memcpy */

/* Builds the drawable form of a packed model resource. Its entries are offset tables (a count,
 * then the offset of each entry and of the end): the vertex frames, a block copied as is, the
 * levels of detail (each a table of parts: a material and a packed command stream) and, with
 * withExtra, three more blocks copied as is. The packed commands are expanded into a display list:
 * 0x80 one or two triangles (5-bit vertex indices), 0x20 a vertex load from the matching frame,
 * 0x40 a matrix load from segment 1, 0x60 a moveword. withTerminator adds a -1 terminator entry. */
void func_00209AF4(s32 unused, void ***handle, s32 withExtra, s32 withTerminator) {
    s32 vtxSize;
    s32 dataSize;
    s32 extra3Size;
    s32 extra4Size;
    s32 extra5Size;
    s32 frameSize;
    s32 lodCount;
    s32 lodOff;
    s32 vtxCounter;
    void *extra3;
    void *extra4;
    void *extra5;
    void *data;
    s32 *lods;
    s32 *lodTbl;
    s32 *parts;
    s32 *partTbl;
    u8 *base;
    Gfx *vtxBase;
    void **block;
    s32 *vtxTbl;
    s32 *frameTbl;
    s32 *frames;
    s32 vtxCount;
    s32 count;
    void *src;
    s32 size;
    s32 i;
    s32 j;
    s32 partCount;
    s32 n;
    u8 *p;
    u8 c;
    s32 *dst;
    s32 off;
    s32 partOff;
    s32 partSize;
    s32 *outPart;
    void *part;
    Gfx *start;
    Gfx *gfx;
    u32 w;
    s32 r;
    s32 g;
    s32 b;
    s32 hi;
    s32 lo;
    s32 first;
    s32 next;
    s32 segment1;
    s32 cnt;
    s32 b0;
    s32 b1;
    s32 b2;
    s32 b3;

    if (func_00200B00(handle, 1) == 0) {
        return;
    }
    count = 6;
    if (withExtra == 0) {
        count = 3;
        if (withTerminator != 0) {
            count = 4;
        }
    }
    size = TABLE_SIZE(count);
    src = **handle;
    frames = func_002017E8(src, 0, &vtxSize);
    vtxCount = *frames;
    size += vtxSize;
    data = func_002017E8(src, 1, &dataSize);
    size += dataSize;
    lods = func_002017D4(src, 2);
    lodCount = *lods;
    size += TABLE_SIZE(lodCount);
    for (i = 0; i < lodCount; i++) {
        parts = func_002017D4(lods, i);
        partCount = *parts;
        size += TABLE_SIZE(partCount);
        for (j = 0; j < partCount; j++) {
            s32 len;
            u8 op;

            part = func_002017D4(parts, j);
            size += 0x30;
            p = func_002017D4(part, 1);
            len = 0;
            op = *p;
            while (op != 0) {
                if (op & 0x80) {
                    p += 2;
                    len += 8;
                    if (*p & 0x80) {
                        p += 2;
                    }
                } else {
                    switch (op & 0x60) {
                    case 0x20:
                    case 0x40:
                        p += 2;
                        len += 8;
                        break;
                    case 0x60:
                        p += 5;
                        len += 8;
                        break;
                    }
                }
                op = *p;
            }
            size += (len + 0xF) & ~7;
        }
    }
    if (withTerminator != 0) {
        size += 8;
    }
    if (withExtra != 0) {
        extra3 = func_002017E8(src, 3, &extra3Size);
        size += extra3Size;
        extra4 = func_002017E8(src, 4, &extra4Size);
        size += extra4Size;
        extra5 = func_002017E8(src, 5, &extra5Size);
        size += extra5Size;
    }
    block = func_00205D7C(0, handle, size);
    if (block != NULL) {
        off = TABLE_SIZE(count);
        dst = *block;
        base = (u8 *)dst;
        n = TABLE_SIZE(vtxCount);
        vtxTbl = (s32 *)(base + off);
        dst[0] = 0;
        dst[1] = off;
        vtxTbl[0] = 0;
        vtxTbl[1] = n;
        frameTbl = vtxTbl;
        for (i = 0; i < vtxCount;) {
            func_0020192C(func_002017E8(frames, i, &frameSize), (u8 *)frameTbl + n, 0);
            next = i + 1;
            n += frameSize;
            frameTbl[0] = next;
            frameTbl[next + 1] = n;
            i = next;
        }
        off += n;
        dst[0] = 1;
        dst[2] = off;
        func_0029E460(base + off, data, dataSize);
        lodOff = TABLE_SIZE(lodCount);
        dst[0] = 2;
        off += dataSize;
        dst[3] = off;
        lodTbl = (s32 *)(base + off);
        lodTbl[0] = 0;
        lodTbl[1] = lodOff;
        segment1 = 0x01000000;
        for (i = 0; i < lodCount;) {
            parts = func_002017D4(lods, i);
            vtxCounter = 0;
            partCount = *parts;
            partTbl = (s32 *)((u8 *)lodTbl + lodOff);
            partOff = TABLE_SIZE(partCount);
            partTbl[0] = 0;
            partTbl[1] = partOff;
            vtxBase = (Gfx *)((u8 *)func_002017D4(frameTbl, i) + 8);
            for (j = 0; j < partCount;) {
                part = func_002017D4(parts, j);
                partSize = 0x10;
                outPart = (s32 *)((u8 *)partTbl + partOff);
                outPart[0] = 0;
                outPart[1] = partSize;
                func_0026D070((u8 *)outPart + partSize, func_002017D4(part, 0));
                partSize = 0x30;
                outPart[0] = 1;
                outPart[2] = partSize;
                p = func_002017D4(part, 1);
                start = (Gfx *)((u8 *)outPart + partSize);
                gfx = start;
                c = *p;
                while (c != 0) {
                    if (c & 0x80) {
                        hi = *p++;
                        lo = *p++;
                        w = (hi << 8) | lo;
                        r = (w >> 10) & 0x1F;
                        g = (w >> 5) & 0x1F;
                        b = w & 0x1F;
                        if (*p & 0x80) {
                            hi = *p++;
                            lo = *p++;
                            w = (hi << 8) | lo;
                            GFX_W(gfx++, (r << 17) | (g << 9) | (b << 1) | 0x06000000,        /* gSP2Triangles */
                                  (((w >> 9) & 0x3E) << 16) | ((w << 4) & 0x3E00) | ((w & 0x1F) << 1));
                        } else {
                            GFX_W(gfx++, (r << 17) | (g << 9) | (b << 1) | 0x05000000, 0);  /* gSP1Triangle */
                        }
                    } else {
                        switch (c & 0x60) {
                        case 0x20:
                            hi = *p++;
                            lo = *p;
                            w = (hi << 8) | lo;
                            first = (w >> 5) & 0x1F;
                            cnt = w & 0x1F;
                            if (cnt == 0) {
                                cnt = 32;
                            }
                            p++;
                            GFX_W(gfx++, (cnt << 12) | (((first + cnt) << 1) | segment1),     /* gSPVertex */
                                  (u32)&vtxBase[vtxCounter * 2] - (u32)vtxTbl + 0x02000000);
                            vtxCounter += cnt;
                            break;
                        case 0x40:
                            p++;
                            cnt = *p++;
                            GFX_W(gfx++, 0xDA380003, (cnt << 6) + segment1);                    /* gSPMatrix */
                            break;
                        case 0x60:
                            first = c & 0x1F;
                            p++;
                            b0 = *p++;
                            b1 = *p++;
                            b2 = *p++;
                            b3 = *p++;
                            GFX_W(gfx++, (first << 1) | 0x02140000, (b0 << 24) | (b1 << 16) | (b2 << 8) | b3);
                            break;
                        }
                    }
                    c = *p;
                }
                partSize += ((u8 *)gfx - (u8 *)start + 0xF) & ~7;
                partOff += partSize;
                next = j + 1;
                GFX_W(gfx++, 0xDF000000, 0);                                                   /* gSPEndDisplayList */
                outPart[0] = 2;
                outPart[3] = partSize;
                partTbl[0] = next;
                partTbl[next + 1] = partOff;
                j = next;
            }
            next = i + 1;
            lodOff += partOff;
            lodTbl[0] = next;
            lodTbl[next + 1] = lodOff;
            i = next;
        }
        off += lodOff;
        dst[0] = 3;
        dst[4] = off;
        if (withTerminator != 0) {
            s32 *e = (s32 *)(base + off);

            e[0] = -1;
            e[1] = 0;
            dst[0] = 4;
            dst[5] = off;
        }
        if (withExtra != 0) {
            func_0029E460(base + off, extra3, extra3Size);
            dst[0] = 4;
            off += extra3Size;
            dst[5] = off;
            func_0029E460(base + off, extra4, extra4Size);
            dst[0] = 5;
            off += extra4Size;
            dst[6] = off;
            func_0029E460(base + off, extra5, extra5Size);
            dst[0] = 6;
            off += extra5Size;
            dst[7] = off;
        }
    }
    func_002057B0(0, handle, block);
}
