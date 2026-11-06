/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00278530.c, func_00278530, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/world.h"

typedef struct Box3 {
    CVector3 mn;
    CVector3 mx;
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

extern char D_800A57B8[];
extern s32 gLevelLightsEnabled;
extern CVector3 sLitVertexOffsets[];
extern CScene gScene;
extern s32 gEngineApp_m_Settings;

void *CISet__GetBlock(void *file, s32 index);
s32 *CISet__GetBlockAndSize(s32 file, s32 index, s32 *size);
s32 **CCache__Allocate(s32, s32 size, s32, char *name);
void Actors_AccumulateVertexLighting(WRec **recs, s32 count, void *verts, s32 arg3, CVector3 *points, s32 arg5);
void Lighting_DebugHueColors(void *verts, s32 arg3, CVector3 *points, s32 arg5);
void Lighting_CopyVertexColors(void *verts, void *colors);
void Lighting_ApplyDirectional(void *verts, s32 arg3, CVector3 *points, s32 arg5);
s32 Lighting_CollectVolumes(Box3 *q, WRec **out, s32 *cnt);
void Model_ApplyPackedOffsets(s32 file, s32 count, CVector3 *points);
void Lighting_WriteVertexColors(void *verts, CVector3 *points);
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
        return Lighting_CollectVolumes(q, out, cnt);
    }
    tab = CISet__GetBlock(regions, 2);
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
        total += ((s32 *) CISet__GetBlock(src, i))[1];
    }
    return total;
}

/* The do { } while (0) blocks around the clearing loop and the copy weight
 * total's references (GCC counts refs by loop depth), so total gets $s4
 * before blk, as in the ROM. */
s32 **Lighting_BuildLitVertices(s32 model, s32 *regions, Box3 *box, s32 arg3, s32 arg4) {
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
    CScene *g;

    colors = NULL;
    ret = findRecords(box, regions, recs, &count);
    src = CISet__GetBlockAndSize(model, 0, &size);
    total = countVerts(src);
    do {
        for (j = 0; j < total; j++) {
            sLitVertexOffsets[j].x = 0;
            sLitVertexOffsets[j].y = 0;
            sLitVertexOffsets[j].z = 0;
        }
    } while (0);
    if (ret != 0) {
        total2 = countVerts(src);
        blk = CCache__Allocate(0, size + 0x10 + total2 * 4, 0, D_800A57B8 + 4);
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
        blk = CCache__Allocate(0, size + 8, 0, D_800A57B8 + 4);
        if (blk == NULL) {
            return NULL;
        }
        copySrc = src;
        (*blk)[0] = 0;
        dst = (u8 *) *blk + 8;
        copySize = size;
    }
    do {
        func_0029E460(dst, copySrc, copySize);
        Model_ApplyPackedOffsets(model, total, sLitVertexOffsets);
    } while (0);
    if (gLevelLightsEnabled == 0) {
        goto plain;
    }
    if (gEngineApp_m_Settings & 0x8000) {
        Lighting_DebugHueColors(dst, arg3, sLitVertexOffsets, arg4);
        goto done;
    }
    if (count != 0) {
        goto lit;
    }
    n = 0;
    g = &gScene;
    for (k = 0; k < g->m_nSectors; k++) {
        n += ((s32 *) CISet__GetBlock(*g->m_Sectors[k].m_hData, 2))[1];
    }
    if (n != 0) {
        goto done;
    }
    do {
plain:
        Lighting_ApplyDirectional(dst, arg3, sLitVertexOffsets, arg4);
        goto done;
lit:
        Actors_AccumulateVertexLighting(recs, count, dst, arg3, sLitVertexOffsets, arg4);
    } while (0);
done:
    Lighting_WriteVertexColors(dst, sLitVertexOffsets);
    if (ret != 0) {
        Lighting_CopyVertexColors(dst, colors);
    }
    return blk;
}
