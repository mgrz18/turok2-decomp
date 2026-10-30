/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0028795C.c, func_0028795C, with Turok 2's layouts. */
#include "common.h"

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
    s32 unk0;               /* 0x00 */
    s32 count;              /* 0x04 */
    u8 entries[1];          /* 0x08 */
} Table;

typedef struct {
    s32 count;              /* 0x00: sections so far */
    s32 offsets[5];         /* 0x04 */
} Block;

typedef struct {
    s32 stride;             /* 0x00 */
    s32 count;              /* 0x04 */
} Section;

typedef struct { u8 pad0[8]; u16 type; u8 padA[0x12]; } SrcA;          /* 0x1C */
typedef struct { u8 pad0[0x28]; u16 type; u8 pad2A[0xA]; } SrcB;       /* 0x34 */
typedef struct { u8 pad0[0x14]; u16 type; u8 pad16[0xA]; } SrcC;       /* 0x20 */
typedef struct { u8 pad0[0x48]; } SrcD;                                /* 0x48 */
typedef struct { u8 pad0[0x90]; } ItemA;                               /* 0x90 */
typedef struct { u8 pad0[0xC0]; } ItemD;                               /* 0xC0 */

typedef struct {
    u8 pad0[4];
    f32 pos[3];             /* 0x004 */
    u8 pad10[0x15C];
    u16 flags;              /* 0x16C */
    u16 index;              /* 0x16E: bit in the level's killed array */
    u8 pad170[8];
    u8 links[0x20];         /* 0x178 */
} Actor;                                                               /* 0x198 */

typedef struct {
    u8 pad0[0x14];
    s32 unk14;              /* 0x14 */
    u8 pad18[0x28];
    s32 unk40;              /* 0x40 */
    u8 pad44[0x14];
    void *unk58;            /* 0x58 */
    u8 pad5C[0x10];
    void *levels;           /* 0x6C */
    u8 pad70[0x18E50 - 0x70];
    f32 *scale;             /* 0x18E50 */
    u8 pad18E54[0x18FA8 - 0x18E54];
    s32 level;              /* 0x18FA8 */
} World;

s32 func_00200B00(s32 ***, s32);
void *func_002017D4(void *, s32);
void func_00201818(void *, s32);
s32 func_00201848(s32, s32, u16);
void **func_00205D7C(s32, s32 ***, s32);
void func_002057B0(s32, s32 ***, void **);
void func_0020192C(Table *, void *, s32);
void func_002141F0(ItemA *, SrcA *, s32, void *, f32);
void func_00214564(ItemA *, SrcB *, s32, void *);
void func_002150C0(Actor *, SrcC *, s32, void *);
void func_00211F64(ItemD *, SrcD *);
f32 func_002671B4(Actor *);
void func_00222A9C(void **, s32, f32 *, void *);

void func_00220D00(World *world, s32 ***file) {
    Table *tE;
    void *links[128];
    void **pp;
    f32 pos[3];
    Table *tA;
    Table *tB;
    Table *tC;
    Table *tD;
    u8 *base;
    Table *tAux;
    Table *bits;
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
    Actor *actors;
    void *aux;
    void **handle;

    if (func_00200B00(file, 1) != 0) {
        root = **file;
        tA = func_002017D4(root, 0);
        nA = tA->count;
        tB = func_002017D4(root, 1);
        nB = tB->count;
        n = nA + nB;
        szA = (n * 0x90) | 8;
        tC = func_002017D4(root, 2);
        nC = tC->count;
        szx = nC * 0x198;
        szC = szx + 8;
        tD = func_002017D4(root, 3);
        nD = tD->count;
        szD = (nD * 0xC0) | 8;
        tot = szC + 0x18;
        tt = szA + tot;
        handle = func_00205D7C(0, file, tt + szD);
        if (handle != NULL) {
            block = *handle;
            off = 0x18;
            sn = 1;
            sec = (u8 *)block + off;
            block->count = 0;
            block->offsets[0] = off;
            func_0020192C(tA, sec, 1);
            func_0020192C(tB, sec, 1);
            nv = tA->entries;
            ((Section *)sec)->stride = 0x90;
            ((Section *)sec)->count = n;
            tAux = func_002017D4(world->unk58, 2);
            pB = (SrcB *)tB->entries;
            itemsA = (ItemA *)((u8 *)block + 0x20);
            base = (u8 *)block;
            aux = tAux->entries;
            pA = (SrcA *)nv;
            i = 0;
            if (nA > 0) {
                do {
                    func_002141F0(&itemsA[i], &pA[i], func_00201848(world->unk40, world->unk14, pA[i].type), aux, *world->scale);
                    i++;
                } while (i < nA);
            }
            if (nB > 0) {
                qb = pB;
                j = 0;
                do {
                    func_00214564(&itemsA[j + nA], qb, func_00201848(world->unk40, world->unk14, qb->type), aux);
                    qb++;
                    j++;
                } while (j < nB);
            }
            off += szA;
            block->count = sn;
            sn++;
            secC = base + off;
            block->offsets[block->count] = off;
            func_0020192C(tC, secC, 1);
            ((Section *)secC)->stride = 0x198;
            ((Section *)secC)->count = nC;
            actors = (Actor *)(secC + 8);
            pC = (SrcC *)tC->entries;
            k = 0;
            if (nC > 0) {
                do {
                    func_002150C0(&actors[k], &pC[k], func_00201848(world->unk40, world->unk14, pC[k].type), aux);
                    index = actors[k].index;
                    level = world->level;
                    bits = func_002017D4(func_002017D4(func_002017D4(world->levels, 0), level), 1);
                    do {
                    } while (0);
                    func_002017D4(bits, 0);
                    func_00201818(bits, 1);
                    killed = func_002017D4(bits, 1);
                    mask = 1 << (index & 7);
                    if (killed[index / 8] & mask) {
                        actors[k].flags |= 1;
                    }
                    k++;
                } while (k < nC);
            }
            off += szC;
            block->count = sn;
            sn++;
            secD = base + off;
            block->offsets[block->count] = off;
            func_0020192C(tD, secD, 1);
            ((Section *)secD)->stride = 0xC0;
            m = 0;
            ((Section *)secD)->count = nD;
            pD = (SrcD *)tD->entries;
            itemsD = (ItemD *)(secD + 8);
            if (nD > 0) {
                qpd = pD;
                qid = itemsD;
                do {
                    func_00211F64(qid, qpd);
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
                    pos[0] = actors[k].pos[0];
                    pos[1] = actors[k].pos[1] + func_002671B4(&actors[k]) * 0.5f;
                    pos[2] = actors[k].pos[2];
                    func_00222A9C(links, nD, pos, actors[k].links);
                    k++;
                } while (k < nC);
            }
        }
        func_002057B0(0, file, handle);
    }
}
