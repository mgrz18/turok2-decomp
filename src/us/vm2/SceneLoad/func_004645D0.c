/* The second VM module's copy of src/us/vm1/SceneLoad/func_0042B6B0.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_27.CPP, func_8044C3D4_de, with Turok 2's layouts. */
#include "common.h"
#include "globals/camera.h"
#include "protos/iset.h"
#include "protos/cache.h"

#include "structs/world.h"
#include "structs/collision.h"
#include "structs/twin_sceneload.h"
#define WORLD_TREE(w) (((CScene *)(w))->m_pBitSections2)
#define WORLD_GROUP(w) (((CScene *)(w))->m_nLevel)









extern s32 ProPackDecompress(CollisionBuildMeshHandle **handle, s32);
extern void UnpackScaledVec4(CollisionBuildVertex *v);
extern void CGameRegion__Unpack(CollisionBuildTriangle *tri, CollisionBuildFace *face, CollisionBuildVertex *verts,
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
void CScene__DecompressCollision_2(CollisionBuildWorld *world, CollisionBuildMeshHandle **handle) {
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

    if (ProPackDecompress(handle, 1) != 0) {
        node = (*handle)->data;
        first = CISet__GetBlockAndSize(node, 0, &size0);
        vlist = CISet__GetBlockAndSize(node, 1, &size1);
        flist = CISet__GetBlock(node, 2);
        ntris = flist->count;
        chunk = CISet__GetBlockAndSize(node, 3, &size3);
#ifdef TARGET_PORT /* host triangles (CGameRegion) are larger than 0x20 */
        tableSize = ntris * sizeof(CollisionBuildTriangle) + 8;
#else
        tableSize = (ntris << 5) | 8;
#endif
        block = CCache__AllocateMemoryForDecompression(0, handle, size0 + 0x18 + size1 + tableSize + size3);
        if (block != 0) {
            offset = 0x18;
            hdr = block->data;
            i = 0;
            hdr->count = 0;
            hdr->offsets[0] = offset;
            CUSet__Transpose(first, (char *)hdr + offset, 0);
            hdr->count = 1;
            offset += size0;
            vdst = (char *)hdr + offset;
            hdr->offsets[1] = offset;
            CUSet__Transpose(vlist, vdst, 0);
            hdr->count = 2;
            offset += size1;
            dst = (char *)hdr + offset;
            hdr->offsets[2] = offset;
            CUSet__Transpose(flist, dst, 1);
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
                UnpackScaledVec4(&verts[i]);
            }
            for (j = 0; j < ntris; j++) {
                tri = &tris[j];
                CGameRegion__Unpack(tri, &face[j], verts, nverts, tris, ntris);
                index = WORLD_GROUP(world);
                group = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(WORLD_TREE(world), 0), index), 0);
                CISet__GetBlock(group, 0);
                CISet__GetBlockSize(group, 1);
                bits = CISet__GetBlock(group, 1);
                if (testBit(j, bits)) {
                    tri->flags |= 0x400;
                }
                if (!(tri->mask & gDefaultVisBits)) {
                    tri->mask = gDefaultVisBits;
                }
                if (Cinema_PathPlaying() != 0) {
                    tri->mask = gDefaultVisBits;
                }
                if (tri->flags & 0x2000) {
                    tri->mask = gDefaultVisBits;
                }
            }
            for (j = 0; j < ntris; j++) {
                tri = &tris[j];
                for (k = 0; k < 3; k++) {
                    if (tri->adj[k] != 0 && (tri->adj[k]->flags & 0x2000)) {
                        { s32 *surface = &tri->mask; *surface = gDefaultVisBits; }
                    }
                }
            }
            offset += tableSize;
            hdr->count = 3;
            hdr->offsets[3] = offset;
            CUSet__Transpose(chunk, base + offset, 0);
            hdr->count = 4;
            offset += size3;
            hdr->offsets[4] = offset;
        }
        CCache__DeallocAndReplaceCompressedData(0, handle, block);
    }
}
