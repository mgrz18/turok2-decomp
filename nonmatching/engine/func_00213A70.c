/* NON-MATCHING: 19 of 264 words differ: only a register swap, total (ROM $s4) and blk (ROM $s5) are allocated the other way round. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00278530.c, func_00278530, with Turok 2's layouts. */
#include "common.h"

typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

typedef struct Box3 {
    Vec3f mn;
    Vec3f mx;
} Box3;

/* A world region record (0xC0 bytes). */
typedef struct WRec {
    /* 0x00 */ u8 pad0[0xA0];
    /* 0xA0 */ Box3 box;
    /* 0xB8 */ u8 padB8[0x2];
    /* 0xBA */ u8 bBA;
    /* 0xBB */ u8 padBB[0x5];
} WRec;

typedef struct WTab {
    /* 0x0 */ u8 pad0[0x4];
    /* 0x4 */ s32 count;
    /* 0x8 */ WRec r[1];
} WTab;

typedef struct M5Cell { f32 x0, z0, x1, z1; } M5Cell;
typedef struct M5Ent { s32 **a; M5Cell *cell; } M5Ent;
typedef struct M5Grid {
    /* 0x0000 */ u8 pad0[0x14A8];
    /* 0x14A8 */ s32 n;
    /* 0x14AC */ M5Ent ent[1];
} M5Grid;

extern char D_800A57B8[];
extern s32 D_800ACAE0;
extern Vec3f D_800DD790[];
extern M5Grid D_800F7078;
extern s32 D_8011AAD8;

void *func_002017D4(void *file, s32 index);
s32 *func_002017E8(s32 file, s32 index, s32 *size);
s32 **func_00204EDC(s32, s32 size, s32, char *name);
void func_0021285C(WRec **recs, s32 count, void *verts, s32 arg3, Vec3f *points, s32 arg5);
void func_00212DA4(void *verts, s32 arg3, Vec3f *points, s32 arg5);
void func_00212F98(void *verts, void *colors);
void func_002130A8(void *verts, s32 arg3, Vec3f *points, s32 arg5);
s32 func_002132B8(Box3 *q, WRec **out, s32 *cnt);
void func_00213684(s32 file, s32 count, Vec3f *points);
void func_0021375C(void *verts, Vec3f *points);
void func_0029E460(void *dst, void *src, s32 size);

/* Collects the region records whose box overlaps q (at most 64); returns whether one is flagged. */
static __inline__ s32 findRecords(Box3 *q, s32 *regions, WRec **out, s32 *cnt) {
    WTab *tab;
    WRec *rec;
    s32 n;
    s32 ret;
    s32 i;

    n = 0;
    ret = 0;
    if (regions == NULL) {
        return func_002132B8(q, out, cnt);
    }
    tab = func_002017D4(regions, 2);
    i = tab->count;
    i--;
    if (i != -1) {
        rec = tab->r;
        do {
            if (q->mx.x > rec->box.mn.x && q->mn.x < rec->box.mx.x
                && q->mx.z > rec->box.mn.z && q->mn.z < rec->box.mx.z
                && q->mx.y > rec->box.mn.y && q->mn.y < rec->box.mx.y) {
                out[n] = rec;
                n++;
                if (rec->bBA != 0) {
                    ret = 1;
                }
                if (n == 0x40) {
                    break;
                }
            }
            rec++;
            i--;
        } while (i != -1);
    }
    *cnt = n;
    return ret;
}

static __inline__ s32 countVerts(s32 *src) {
    s32 i;
    s32 total;

    total = 0;
    for (i = 0; i < *src; i++) {
        total += ((s32 *) func_002017D4(src, i))[1];
    }
    return total;
}

s32 **func_00213A70(s32 model, s32 *regions, Box3 *box, s32 arg3, s32 arg4) {
    WRec *recs[64];
    s32 count;
    s32 size;
    u8 *colors;
    s32 ret;
    s32 *src;
    s32 total;
    s32 total2;
    s32 i;
    s32 **blk;
    volatile s32 *hdr;
    void *dst;
    s32 n;
    s32 j;
    s32 k;
    s32 magic;
    s32 *copySrc;
    s32 copySize;
    M5Grid *g;

    colors = NULL;
    ret = findRecords(box, regions, recs, &count);
    src = func_002017E8(model, 0, &size);
    total = countVerts(src);
    for (j = 0; j < total; j++) {
        D_800DD790[j].x = 0;
        D_800DD790[j].y = 0;
        D_800DD790[j].z = 0;
    }
    if (ret != 0) {
        total2 = countVerts(src);
        blk = func_00204EDC(0, size + 0x10 + total2 * 4, 0, D_800A57B8 + 4);
        if (blk == NULL) {
            return NULL;
        }
        magic = 0xDEADBEEF;
        copySrc = src;
        hdr = *blk;
        hdr[0] = 1;
        copySize = size;
        hdr[1] = 0x10;
        hdr[3] = magic;
        hdr[2] = copySize + 0x10;
        dst = (u8 *) *blk + hdr[1];
        colors = (u8 *) *blk + hdr[2];
    } else {
        blk = func_00204EDC(0, size + 8, 0, D_800A57B8 + 4);
        if (blk == NULL) {
            return NULL;
        }
        copySrc = src;
        (*blk)[0] = 0;
        dst = (u8 *) *blk + 8;
        copySize = size;
    }
    func_0029E460(dst, copySrc, copySize);
    func_00213684(model, total, D_800DD790);
    if (D_800ACAE0 == 0) {
        goto plain;
    }
    if (D_8011AAD8 & 0x8000) {
        func_00212DA4(dst, arg3, D_800DD790, arg4);
        goto done;
    }
    if (count != 0) {
        goto lit;
    }
    n = 0;
    g = &D_800F7078;
    for (k = 0; k < g->n; k++) {
        n += ((s32 *) func_002017D4(*g->ent[k].a, 2))[1];
    }
    if (n != 0) {
        goto done;
    }
    do {
plain:
        func_002130A8(dst, arg3, D_800DD790, arg4);
        goto done;
lit:
        func_0021285C(recs, count, dst, arg3, D_800DD790, arg4);
    } while (0);
done:
    func_0021375C(dst, D_800DD790);
    if (ret != 0) {
        func_00212F98(dst, colors);
    }
    return blk;
}
