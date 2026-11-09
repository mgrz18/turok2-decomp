/* Second VM module copy of CScene__ConstructMap (same source, linked 0xE0 lower). */
#include "common.h"
#include "protos/iset.h"

typedef struct Record {
    /* 0x00 */ s32 data[2];
    /* 0x08 */ s32 valid;
    /* 0x0C */ s32 rest[4];
} Record; /* 0x1C */

extern char D_800A5D70[];
extern char D_800A5D80[];
extern void Model_DecompressCallback(void);

extern s32 CScene__FindObjectType(void *level, s32 key);
extern s32 CScene__GetObject(void *level, s32 id);
extern s32 **CCache__Request(s32 unused, s32 key, s32 size, s32 mode, s32 owner, void *tag, char *name, s32 direct);
extern void CCache__Deallocate(s32, void *);

/* Looks key up in the level and loads its record into out (out->valid = 0 when there is none). */
void CScene__ConstructMap_2(void *level, Record *out, s32 key) {
    s32 id;
    s32 **h1;
    s32 **h2;
    s32 size;
    s32 res;

    id = CScene__FindObjectType(level, key);
    if (id == -1) {
        out->valid = 0;
        return;
    }
    res = CScene__GetObject(level, id);
    h1 = CCache__Request(0, res, 0x18, 0, 0, 0, D_800A5D70, 1);
    if (h1 != NULL) {
        h2 = CCache__Request(0, CISet__GetROMAddressAndSize(*h1, res, 1, &size), size, 0, 0, Model_DecompressCallback, D_800A5D80, 1);
        if (h2 != NULL) {
            *out = *(Record *)CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*h2, 2), 0), 0), 0);
            CCache__Deallocate(0, h2);
        }
        CCache__Deallocate(0, h1);
    }
}
