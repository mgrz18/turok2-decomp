#include "common.h"

/* Expands a packed animation resource in place: rebuilds the six-chunk file
 * with chunks 0-4 copied unchanged and chunk 5 (the compressed animation)
 * replaced by a decoded one, whose position and rotation tracks
 * func_00216604 fills in. A resource that fails to decode is dropped.
 * packInit/packAdd/decodeAnim are func_00218548/func_00218590/func_00218748 inlined. */

typedef struct Pack {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 offsets[1]; /* count + 1 byte offsets, 0xDEADBEEF past the last one set */
} Pack;

typedef struct AnimHeader {
    /* 0x00 */ s32 radius;
    /* 0x04 */ s32 frames;
    /* 0x08 */ s32 encoding;
    /* 0x0C */ f32 fC;
    /* 0x10 */ s32 rotTracks;
    /* 0x14 */ s32 posTracks;
    /* 0x18 */ f32 frameCount;
    /* 0x1C */ f32 frameStep;
} AnimHeader;

typedef struct Resource {
    /* 0x0 */ s32 *file;
    /* 0x4 */ s32 size;
} Resource;

extern f32 D_800E6790;
extern s32 D_800E6794;
extern s32 D_800E6798;
extern f32 D_800E679C;
extern f32 D_800E67A0;
extern s32 D_800E67A4;
extern s32 D_800E67A8;

s32 func_00200B00(Resource **res, s32 arg1);
void *func_002017D4(void *file, s32 index);
void *func_002017E8(void *file, s32 index, s32 *size);
void func_002052D8(s32, Pack **blk);
void func_002057B0(s32, Resource **res, Pack **blk);
Pack **func_00205D7C(s32, Resource **res, s32 size);
s32 func_00216604(void *source, Pack *table, s32 tracks, s32 channels, s32 radius, s32 encoding);
void func_0029E460(void *dst, void *src, s32 size);

static __inline__ void packInit(Pack *pack, s32 count) {
    s32 index;
    s32 *ptr;
    s32 magic;

    index = 1;
    pack->count = count;
    pack->offsets[0] = ((count * 4) + 0xF) & ~7;
    if (count > 0) {
        magic = 0xDEADBEEF;
        ptr = pack->offsets;
        do {
            ptr[1] = magic;
            index += 1;
            ptr += 1;
        } while (count >= index);
    }
}

static __inline__ void *packAdd(Pack *pack, void *src, s32 size) {
    s32 i;
    void *data;

    for (i = 0; i <= pack->count; i++) {
        if (pack->offsets[i] == 0xDEADBEEF) {
            break;
        }
    }
    i--;
    pack->offsets[i + 1] = pack->offsets[i] + size;
    data = (u8 *) pack + pack->offsets[i];
    if (src != NULL) {
        func_0029E460(data, src, size);
    }
    return data;
}

static __inline__ s32 decodeAnim(void *src, void *dst) {
    s32 ok;
    AnimHeader *h = func_002017D4(src, 0);
    void *posSrc = func_002017D4(src, 1);
    void *rotSrc = func_002017D4(src, 2);
    Pack *posDst = func_002017D4(dst, 1);
    Pack *rotDst = func_002017D4(dst, 2);

    D_800E6798 = 1;
    D_800E6794 = 0;
    D_800E679C = -2000.0f;
    D_800E67A0 = 2000.0f;
    D_800E6790 = h->fC;
    if (func_00216604(posSrc, posDst, h->posTracks, 3, h->radius, h->encoding) == 0) {
        ok = 0;
    } else {
        D_800E6798 = 0;
        D_800E6794 = 1;
        D_800E679C = -1.0f;
        D_800E67A0 = 1.0f;
        D_800E6790 = h->fC;
        ok = func_00216604(rotSrc, rotDst, h->rotTracks, 4, h->radius, h->encoding) != 0;
    }
    return ok;
}

void func_00216A9C(s32 arg0, Resource **res) {
    s32 size0;
    s32 size1;
    s32 size2;
    s32 size3;
    s32 size4;
    s32 size5;
    s32 size6;
    s32 size7;
    void *chunk4;
    s32 posSize;
    s32 rotSize;
    s32 *file;
    s32 oldSize;
    void *chunk0;
    void *chunk1;
    void *chunk2;
    void *chunk3;
    void *anim;
    AnimHeader *h;
    s32 n;
    s32 animSize;
    Pack **blk;
    Pack *pack;
    Pack *out;

    if (func_00200B00(res, 1) == 0) {
        return;
    }
    file = (*res)->file;
    oldSize = (*res)->size;
    chunk0 = func_002017E8(file, 0, &size0);
    chunk1 = func_002017E8(file, 1, &size1);
    chunk2 = func_002017E8(file, 2, &size2);
    chunk3 = func_002017E8(file, 3, &size3);
    chunk4 = func_002017E8(file, 4, &size4);
    anim = func_002017E8(file, 5, &size5);
    h = func_002017D4(anim, 0);
    func_002017E8(anim, 1, &size6);
    func_002017E8(anim, 2, &size7);
    func_002017D4(chunk3, 0);
    n = h->frames;
    D_800E67A4 = n;
    D_800E67A8 = n;
    h->frameCount = n;
    h->frameStep = 1.0f;
    if (n >= 2) {
        h->frameStep = (f32) (n - 1) / (f32) (n - 1);
    }
    posSize = (h->posTracks << 4) * D_800E67A8 + ((h->posTracks * 4 + 0xF) & ~7);
    rotSize = (h->rotTracks << 4) * D_800E67A8 + ((h->rotTracks * 4 + 0xF) & ~7);
    animSize = rotSize + posSize + 0x38;
    blk = func_00205D7C(0, res, oldSize - size5 + animSize);
    if (blk != NULL) {
        pack = *blk;
        packInit(pack, 6);
        packAdd(pack, chunk0, size0);
        packAdd(pack, chunk1, size1);
        packAdd(pack, chunk2, size2);
        packAdd(pack, chunk3, size3);
        packAdd(pack, chunk4, size4);
        out = packAdd(pack, NULL, animSize);
        packInit(out, 3);
        packAdd(out, h, 0x20);
        packAdd(out, NULL, posSize);
        packAdd(out, NULL, rotSize);
        if (!decodeAnim(anim, out)) {
            func_002052D8(0, blk);
            func_002057B0(0, res, NULL);
            return;
        }
    }
    func_002057B0(0, res, blk);
}
