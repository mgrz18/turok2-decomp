/* WRONG CUT: REAL func_0042BF48 size 0x24C (splat cut it into func_0042BF48 + func_0042C0B4). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_13.CPP, func_8044CD8C_de, with Turok 2's layouts. */
#include "common.h"

typedef struct Counted {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 count;
} Counted;

typedef struct StreamCell {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 occupied;
} StreamCell; /* 0x14 */

typedef struct ObjA {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ void **block;
    /* 0x28 */ u8 pad28[0x68];
} ObjA; /* 0x90 */

typedef struct ObjB {
    /* 0x000 */ u8 pad0[0x198];
} ObjB; /* 0x198 */

typedef struct Level {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s32 key;
    /* 0x24 */ u8 pad24[0x2C];
    /* 0x50 */ void *file;
    /* 0x54 */ void *grid;
} Level;

extern char D_800A5F6C[];
extern void func_00220D00(void);

extern void *func_002017D4(void *table, s32 index);
extern s32 func_0020185C(void *file, s32 key, s32 index, s32 *size);
extern void **func_0020367C(s32 unused, s32 key, s32 size, s32 mode, Level *owner, void *tag, char *name, s32 direct);
extern void func_00214D04(ObjA *a, s32);
extern void *func_00214DBC(ObjA *a);
extern void *func_00215944(ObjB *b);
extern void func_0020BE08(void *handle);
extern void func_002051F4(s32, void *);

/* Streams in every grid cell of the level that is not loaded yet and spawns its objects. */
void func_0042BF48(Level *level) {
    s32 size;
    s32 width;
    s32 row;
    s32 lastCol;
    s32 lastRow;
    s32 rows;
    StreamCell *cells;
    StreamCell *cell;
    s32 col;
    s32 first;
    s32 key;
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

    width = ((Counted *)func_002017D4(level->grid, 0))->count;
    row = 0;
    rows = ((Counted *)func_002017D4(level->grid, 2))->count;
    cells = (StreamCell *)((Counted *)func_002017D4(level->grid, 4) + 1);
    lastRow = rows - 1;
    lastCol = width - 1;
    /* FAKEMATCH (as in Rage Wars): a local holding the constant keeps the column loop's entry test as slt */
    first = 0;
    for (; row <= lastRow; row++) {
        for (col = first; col <= lastCol; col++) {
            cell = &cells[col + row * width];
            if (cell->occupied != 0) {
                continue;
            }
            key = func_0020185C(level->file, level->key, col + row * width, &size);
            block = func_0020367C(0, key, size, 4, level, func_00220D00, D_800A5F6C, 1);
            if (block == 0) {
                continue;
            }
            node = *block;
            if (*(s32 *)node != 0) {
                list = func_002017D4(node, 0);
                n = list->count;
                objsA = (ObjA *)(list + 1);
                for (i = 0; i < n; i++) {
                    a = &objsA[i];
                    a->block = block;
                    func_00214D04(a, 0);
                    handle = func_00214DBC(a);
                    if (handle != 0) {
                        func_0020BE08(handle);
                        func_002051F4(0, handle);
                    }
                }
                list = func_002017D4(node, 1);
                m = list->count;
                objsB = (ObjB *)(list + 1);
                for (j = 0; j < m; j++) {
                    b = &objsB[j];
                    handle = func_00215944(b);
                    if (handle != 0) {
                        func_0020BE08(handle);
                        func_002051F4(0, handle);
                    }
                }
            }
            func_002051F4(0, block);
        }
    }
}
