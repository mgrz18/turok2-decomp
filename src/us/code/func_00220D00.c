/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0028795C.c, func_0028795C, with Turok 2's layouts. */
#include "common.h"
#include "structs/table.h"
#include "structs/world.h"
#include "structs/level_actor.h"

/* Instantiates a level's actor data: from the level file's four tables
 * (two kinds of placed objects, the level actors, and a fourth kind of
 * 0x48-byte entries), builds one block with a directory of section offsets
 * and a {stride, count} header per section, fills each record from its
 * source entry, marks the actors already killed in this level (the level's
 * bit array, flag 1 at +0x16C), then links every actor to the 0xC0 records.
 *
 * The empty do { } while (0) blocks are what the ROM needs: GCC weights a
 * pseudo's references by loop depth, so they decide which of the loop
 * counters and record pointers get $s0-$s2 (m before qpd, as in the ROM). */

typedef struct {
    s32 count;              /* 0x00: sections so far */
    s32 offsets[5];         /* 0x04 */
} Block;

typedef struct { u8 pad0[8]; u16 type; u8 padA[0x12]; } SrcA;          /* 0x1C */
typedef struct { u8 pad0[0x28]; u16 type; u8 pad2A[0xA]; } SrcB;       /* 0x34 */
typedef struct { u8 pad0[0x14]; u16 type; u8 pad16[0xA]; } SrcC;       /* 0x20 */
typedef struct { u8 pad0[0x48]; } SrcD;                                /* 0x48 */
typedef struct { u8 pad0[0x90]; } ItemA;                               /* 0x90 */
typedef struct { u8 pad0[0xC0]; } ItemD;                               /* 0xC0 */

s32 ProPackDecompress(s32 ***, s32);
void *CISet__GetBlock(void *, s32);
void CISet__GetBlockSize(void *, s32);
s32 CISet__GetROMAddress(s32 *, s32, u16);
void **CCache__AllocateMemoryForDecompression(s32, s32 ***, s32);
void CCache__DeallocAndReplaceCompressedData(s32, s32 ***, void **);
void CUSet__Transpose(StrideTable *, void *, s32);
void CStaticObject__ConstructCompact(ItemA *, SrcA *, s32, void *, f32);
void CStaticObject__Construct(ItemA *, SrcB *, s32, void *);
void CLevelActor__Construct(CLevelActor *, SrcC *, s32, void *);
void LightVolume_Construct(ItemD *, SrcD *);
f32 GameObject_GetCollisionHeight(CLevelActor *);
void CScene__ComputeLights(void **, s32, f32 *, void *);

void CScene__DecompressGridSection(CWorld *world, s32 ***file) {
    StrideTable *tE;
    void *links[128];
    void **pp;
    f32 pos[3];
    StrideTable *tA;
    StrideTable *tB;
    StrideTable *tC;
    StrideTable *tD;
    u8 *base;
    StrideTable *tAux;
    StrideTable *bits;
    s32 nA;
    s32 nB;
    s32 nC;
    s32 nD;
    s32 n;
    s32 szA;
    s32 szC;
    s32 szD;
    s32 tot;
    s32 tt;
    register s32 szx asm("$2");
    u8 *nv;
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    ItemD *lp;              /* walks the 0xC0 records for links[] */
    s32 mask;
    u8 *killed;
    s32 level;
    s32 off;
    s32 sn;
    void *root;
    Block *block;
    u8 *sec;
    u8 *secC;
    u8 *secD;
    ItemA *itemsA;
    ItemD *itemsD;
    SrcA *pA;
    SrcC *pC;
    SrcD *pD;
    SrcB *qb;
    ItemD *qid;
    SrcD *qpd;
    s32 index;
    SrcB *pB;
    CLevelActor *actors;
    void *aux;
    void **handle;

    if (ProPackDecompress(file, 1) != 0) {
        root = **file;
        tA = CISet__GetBlock(root, 0);
        nA = tA->count;
        tB = CISet__GetBlock(root, 1);
        nB = tB->count;
        n = nA + nB;
        szA = (n * 0x90) | 8;
        tC = CISet__GetBlock(root, 2);
        nC = tC->count;
        szx = nC * 0x198;
        szC = szx + 8;
        tD = CISet__GetBlock(root, 3);
        nD = tD->count;
        szD = (nD * 0xC0) | 8;
        tot = szC + 0x18;
        tt = szA + tot;
        handle = CCache__AllocateMemoryForDecompression(0, file, tt + szD);
        if (handle != NULL) {
            block = *handle;
            off = 0x18;
            sn = 1;
            sec = (u8 *)block + off;
            block->count = 0;
            block->offsets[0] = off;
            CUSet__Transpose(tA, sec, 1);
            CUSet__Transpose(tB, sec, 1);
            nv = tA->entries;
            ((StrideTable *)sec)->stride = 0x90;
            ((StrideTable *)sec)->count = n;
            tAux = CISet__GetBlock(world->m_pRegionData, 2);
            pB = (SrcB *)tB->entries;
            itemsA = (ItemA *)((u8 *)block + 0x20);
            base = (u8 *)block;
            aux = tAux->entries;
            pA = (SrcA *)nv;
            i = 0;
            if (nA > 0) {
                do {
                    CStaticObject__ConstructCompact(&itemsA[i], &pA[i], CISet__GetROMAddress(world->m_pObjects, world->m_ObjectsKey, pA[i].type), aux, world->m_pLevelHeader->m_Scale);
                    i++;
                } while (i < nA);
            }
            if (nB > 0) {
                qb = pB;
                j = 0;
                do {
                    CStaticObject__Construct(&itemsA[j + nA], qb, CISet__GetROMAddress(world->m_pObjects, world->m_ObjectsKey, qb->type), aux);
                    qb++;
                    j++;
                } while (j < nB);
            }
            off += szA;
            block->count = sn;
            sn++;
            secC = base + off;
            block->offsets[block->count] = off;
            CUSet__Transpose(tC, secC, 1);
            ((StrideTable *)secC)->stride = 0x198;
            ((StrideTable *)secC)->count = nC;
            actors = (CLevelActor *)(secC + 8);
            pC = (SrcC *)tC->entries;
            k = 0;
            if (nC > 0) {
                do {
                    CLevelActor__Construct(&actors[k], &pC[k], CISet__GetROMAddress(world->m_pObjects, world->m_ObjectsKey, pC[k].type), aux);
                    index = actors[k].m_nKilledIndex;
                    level = world->m_nLevel;
                    bits = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(world->m_pBitSections, 0), level), 1);
                    do {
                    } while (0);
                    CISet__GetBlock(bits, 0);
                    CISet__GetBlockSize(bits, 1);
                    killed = CISet__GetBlock(bits, 1);
                    mask = 1 << (index & 7);
                    if (killed[index / 8] & mask) {
                        actors[k].m_wFlags |= 1;
                    }
                    k++;
                } while (k < nC);
            }
            off += szC;
            block->count = sn;
            sn++;
            secD = base + off;
            block->offsets[block->count] = off;
            CUSet__Transpose(tD, secD, 1);
            ((StrideTable *)secD)->stride = 0xC0;
            m = 0;
            ((StrideTable *)secD)->count = nD;
            pD = (SrcD *)tD->entries;
            itemsD = (ItemD *)(secD + 8);
            if (nD > 0) {
                qpd = pD;
                qid = itemsD;
                do {
                    LightVolume_Construct(qid, qpd);
                    qpd++;
                    qid++;
                    m++;
                } while (m < nD);
            }
            off += szD;
            block->count = sn;
            sn++;
            block->offsets[block->count] = off;
            m = 0;
            do {
                if (nD > 0) {
                    pp = links;
                    lp = itemsD;
                    do {
                        *pp++ = lp;
                        lp++;
                        m++;
                    } while (m < nD);
                }
            } while (0);
            do {
                do {
                    k = 0;
                } while (0);
            } while (0);
            if (nC > 0) {
                do {
                    pos[0] = actors[k].header.m_vPos.x;
                    pos[1] = actors[k].header.m_vPos.y + GameObject_GetCollisionHeight(&actors[k]) * 0.5f;
                    pos[2] = actors[k].header.m_vPos.z;
                    CScene__ComputeLights(links, nD, pos, actors[k].m_Lights);
                    k++;
                } while (k < nC);
            }
        }
        CCache__DeallocAndReplaceCompressedData(0, file, handle);
    }
}
