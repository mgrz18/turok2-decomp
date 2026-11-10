/* WRONG CUT: REAL CScene__StreamAllSections size 0x24C (splat cut it into CScene__StreamAllSections + func_0042C0B4). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_13.CPP, func_8044CD8C_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/world.h"
#include "protos/iset.h"
#include "protos/cache.h"
#include "protos/scene.h"

typedef struct Counted {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 count;
} Counted;

typedef struct StreamCell {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 occupied;
} StreamCell; /* 0x14 */

#ifdef TARGET_PORT
/* The port: the grid sections hold their static objects and level actors at the host sizes
 * (CScene__DecompressGridSection), so these views are host twins of CStaticObject (block =
 * m_pOwner) and CLevelActor. With the N64 stride and offset the third attract demo's level load
 * (level 109) wrote the handle into the wrong objects and passed a garbage m_pOwner to
 * CCache__RequestDecompression. */
#include "structs/level_actor.h"
typedef struct ObjA {
    u8 pad0[__builtin_offsetof(CStaticObject, m_pOwner)];
    void **block;
    u8 pad28[sizeof(CStaticObject) - __builtin_offsetof(CStaticObject, m_pOwner) - sizeof(void **)];
} ObjA;
_Static_assert(sizeof(ObjA) == sizeof(CStaticObject), "ObjA size");

typedef struct ObjB {
    u8 pad0[sizeof(CLevelActor)];
} ObjB;
#else
typedef struct ObjA {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ void **block;
    /* 0x28 */ u8 pad28[0x68];
} ObjA; /* 0x90 */

typedef struct ObjB {
    /* 0x000 */ u8 pad0[0x198];
} ObjB; /* 0x198 */
#endif


extern void CStaticObject__RequestLitVertices(ObjA *a, s32);
extern void *CStaticObject__LoadGeometry(ObjA *a);
extern void *CLevelActor__LoadGeometry(ObjB *b);
extern void Model_RequestTextures(void *handle);

/* Streams in every grid cell of the level that is not loaded yet and spawns its objects. The loops run over
 * a rectangle of cells (firstRow..lastRow, firstCol..lastCol), here the whole grid: the column loop starting
 * from the variable firstCol, not the constant 0, is what keeps its entry test as the ROM's slt (Rage Wars'
 * decomp matches the same function with a lone `first` local it flags as a fake match). */
void CScene__StreamAllSections(CScene *level) {
    s32 size;
    s32 width;
    s32 row;
    s32 lastCol;
    s32 lastRow;
    s32 rows;
    StreamCell *cells;
    StreamCell *cell;
    s32 col;
    s32 firstCol;
    s32 firstRow;
    sptr key;
    void **block;
    void *node;
    Counted *list;
    s32 i;
    s32 j;
    s32 n;
    s32 m;
    ObjA *a;
    ObjA *objsA;
    ObjB *objsB;
    ObjB *b;
    void *handle;

    width = ((Counted *)CISet__GetBlock(level->m_pCollision, 0))->count;
    rows = ((Counted *)CISet__GetBlock(level->m_pCollision, 2))->count;
    cells = (StreamCell *)((Counted *)CISet__GetBlock(level->m_pCollision, 4) + 1);
    firstRow = 0;
    lastRow = rows - 1;
    firstCol = 0;
    lastCol = width - 1;
    for (row = firstRow; row <= lastRow; row++) {
        for (col = firstCol; col <= lastCol; col++) {
            cell = &cells[col + row * width];
            if (cell->occupied != 0) {
                continue;
            }
            key = CISet__GetROMAddressAndSize(level->m_pLevelInfo, level->m_LevelInfoKey, col + row * width, &size);
            block = CCache__Request(0, key, size, 4, level, CScene__DecompressGridSection, "grid section", 1);
            if (block == 0) {
                continue;
            }
            node = *block;
            if (*(s32 *)node != 0) {
                list = CISet__GetBlock(node, 0);
                n = list->count;
                objsA = (ObjA *)(list + 1);
                for (i = 0; i < n; i++) {
                    a = &objsA[i];
                    a->block = block;
                    CStaticObject__RequestLitVertices(a, 0);
                    handle = CStaticObject__LoadGeometry(a);
                    if (handle != 0) {
                        Model_RequestTextures(handle);
                        CCache__Unlock(0, handle);
                    }
                }
                list = CISet__GetBlock(node, 1);
                m = list->count;
                objsB = (ObjB *)(list + 1);
                for (j = 0; j < m; j++) {
                    b = &objsB[j];
                    handle = CLevelActor__LoadGeometry(b);
                    if (handle != 0) {
                        Model_RequestTextures(handle);
                        CCache__Unlock(0, handle);
                    }
                }
            }
            CCache__Unlock(0, block);
        }
    }
}
