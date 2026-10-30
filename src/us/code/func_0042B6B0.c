/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_27.CPP, func_8044C3D4_de, with Turok 2's layouts. */
#include "common.h"

typedef struct CollisionBuildVertex {
    f32 x;
    f32 y;
    f32 z;
    f32 ceiling;
} CollisionBuildVertex;

typedef struct CollisionBuildTriangle {
    /* 0x00 */ u16 kind;
    /* 0x02 */ u16 flags;
    /* 0x04 */ CollisionBuildVertex *v[3];
    /* 0x10 */ struct CollisionBuildTriangle *adj[3];
    /* 0x1C */ s32 mask;
} CollisionBuildTriangle;

typedef struct CollisionBuildFace {
    u8 data[0x14];
} CollisionBuildFace;

typedef struct CollisionBuildHeader {
    s32 count;
    s32 offsets[5];
} CollisionBuildHeader;

typedef struct CollisionBuildList {
    s32 kind;
    s32 count;
    CollisionBuildFace items[1];
} CollisionBuildList;

typedef struct CollisionBuildVertexList {
    s32 kind, count;
    CollisionBuildVertex items[1];
} CollisionBuildVertexList;

typedef struct CollisionBuildTriangleList {
    s32 kind, count;
    CollisionBuildTriangle items[1];
} CollisionBuildTriangleList;

typedef struct CollisionBuildMeshHandle {
    void *data;
} CollisionBuildMeshHandle;

typedef struct CollisionBuildWorld {
    /* 0x00000 */ char pad0[0x70];
    /* 0x00070 */ void *tree;
    /* 0x00074 */ char pad74[0x18FA8 - 0x74];
    /* 0x18FA8 */ s32 group;
} CollisionBuildWorld;

extern s32 D_800B6D00;

extern s32 func_00200B00(CollisionBuildMeshHandle **handle, s32);
extern void *func_002017E8(void *node, s32 index, s32 *size);
extern void *func_002017D4(void *node, s32 index);
extern void func_00201818(void *node, s32 index);
extern CollisionBuildMeshHandle *func_00205D7C(s32, CollisionBuildMeshHandle **handle, s32 size);
extern void func_002057B0(s32, CollisionBuildMeshHandle **handle, CollisionBuildMeshHandle *block);
extern void func_0020192C(void *src, void *dst, s32 arg2);
extern void func_0026BAE4(CollisionBuildVertex *v);
extern void func_0026BB30(CollisionBuildTriangle *tri, CollisionBuildFace *face, CollisionBuildVertex *verts,
                          s32 nverts, CollisionBuildTriangle *tris, s32 ntris);
extern s32 Cinema_PathPlaying(void);

static __inline__ s32 testBit(s32 n, u8 *bits) {
    s32 mask = 1 << (n & 7);

    return bits[n / 8] & mask;
}

/* Packs a collision mesh into one allocated block: copies the node's vertex list, the triangle source
   and the trailing chunk behind a five-offset header, builds a 0x20-byte triangle record per source
   face and flags triangles whose group bit, the cinema state or neighbours mark them with the default
   surface mask. */
void func_0042B6B0(CollisionBuildWorld *world, CollisionBuildMeshHandle **handle) {
    s32 size0;
    s32 size1;
    s32 size3;
    void *chunk;
    s32 tableSize;
    char *base;
    CollisionBuildVertex *verts;
    CollisionBuildTriangle *tris;
    CollisionBuildMeshHandle *block;
    void *node;
    void *first;
    CollisionBuildVertexList *vlist;
    CollisionBuildList *flist;
    s32 ntris;
    CollisionBuildHeader *hdr;
    s32 offset;
    char *dst;
    char *vdst;
    CollisionBuildVertexList *copiedVertices;
    CollisionBuildTriangleList *copiedTriangles;
    s32 nverts;
    s32 i;
    s32 j;
    s32 k;
    CollisionBuildTriangle *tri;
    CollisionBuildFace *face;
    void *group;
    s32 index;
    u8 *bits;

    if (func_00200B00(handle, 1) != 0) {
        node = (*handle)->data;
        first = func_002017E8(node, 0, &size0);
        vlist = func_002017E8(node, 1, &size1);
        flist = func_002017D4(node, 2);
        ntris = flist->count;
        chunk = func_002017E8(node, 3, &size3);
        tableSize = (ntris << 5) | 8;
        block = func_00205D7C(0, handle, size0 + 0x18 + size1 + tableSize + size3);
        if (block != 0) {
            offset = 0x18;
            hdr = block->data;
            i = 0;
            hdr->count = 0;
            hdr->offsets[0] = offset;
            func_0020192C(first, (char *)hdr + offset, 0);
            hdr->count = 1;
            offset += size0;
            vdst = (char *)hdr + offset;
            hdr->offsets[1] = offset;
            func_0020192C(vlist, vdst, 0);
            hdr->count = 2;
            offset += size1;
            dst = (char *)hdr + offset;
            hdr->offsets[2] = offset;
            func_0020192C(flist, dst, 1);
            face = flist->items;
            copiedTriangles = (void *)dst;
            copiedTriangles->kind = 0x20;
            copiedTriangles->count = ntris;
            base = (char *)hdr;
            tris = copiedTriangles->items;
            nverts = vlist->count;
            copiedVertices = (void *)vdst;
            verts = copiedVertices->items;
            for (i = 0; i < nverts; i++) {
                func_0026BAE4(&verts[i]);
            }
            for (j = 0; j < ntris; j++) {
                tri = &tris[j];
                func_0026BB30(tri, &face[j], verts, nverts, tris, ntris);
                index = world->group;
                group = func_002017D4(func_002017D4(func_002017D4(world->tree, 0), index), 0);
                func_002017D4(group, 0);
                func_00201818(group, 1);
                bits = func_002017D4(group, 1);
                if (testBit(j, bits)) {
                    tri->flags |= 0x400;
                }
                if (!(tri->mask & D_800B6D00)) {
                    tri->mask = D_800B6D00;
                }
                if (Cinema_PathPlaying() != 0) {
                    tri->mask = D_800B6D00;
                }
                if (tri->flags & 0x2000) {
                    tri->mask = D_800B6D00;
                }
            }
            for (j = 0; j < ntris; j++) {
                tri = &tris[j];
                for (k = 0; k < 3; k++) {
                    if (tri->adj[k] != 0 && (tri->adj[k]->flags & 0x2000)) {
                        { s32 *surface = &tri->mask; *surface = D_800B6D00; }
                    }
                }
            }
            offset += tableSize;
            hdr->count = 3;
            hdr->offsets[3] = offset;
            func_0020192C(chunk, base + offset, 0);
            hdr->count = 4;
            offset += size3;
            hdr->offsets[4] = offset;
        }
        func_002057B0(0, handle, block);
    }
}
