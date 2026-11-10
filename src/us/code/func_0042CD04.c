#include "common.h"
#include "protos/iset.h"
#include "structs/world.h"

typedef struct ListItem {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 ref; /* an index, made a pointer into the level's table */
    /* 0x10 */ u8 data[0xC];
} ListItem; /* 0x1C */

typedef struct ItemList {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 count;
    /* 0x08 */ ListItem items[1];
} ItemList;

typedef struct Entry {
    u8 data[0x20];
} Entry;


extern s32 ProPackDecompress(s32 ***handle, s32);
extern void Angle_Wrap(void *);

/* Resolves the references of every item of every group of the loaded resource and keeps it in the
 * level (CScene.unk_68; the references point into block 2 of m_pRegionData). Typed on CScene: the
 * m2c view put them at their N64 offsets 0x58/0x68, which on the host overwrote m_pCollision.
 * The item's ref stays an s32 (the ROM record); on the host it stays the region index
 * (CScene__GetPathRegion below). */
void CScene__DecompressPaths(CScene *level, s32 ***handle) {
    s32 *groups;
    s32 ngroups;
    s32 i;
    s32 j;
    s32 count;
    ItemList *list;
    s32 ref;
    ListItem *item;
    Entry *entries;

    if (ProPackDecompress(handle, 0) != 0) {
        groups = **handle;
        ngroups = groups[0];
        for (i = 0; i < ngroups; i++) {
            list = CISet__GetBlock(CISet__GetBlock(groups, i), 1);
            count = list->count;
            item = list->items;
            for (j = 0; j < count; j++) {
                ref = item->ref;
#ifndef TARGET_PORT
                if (ref != -1) {
                    entries = (Entry *)((u8 *)CISet__GetBlock(level->m_pRegionData, 2) + 8);
                    item->ref = (sptr)&entries[ref];
                } else {
                    item->ref = 0;
                }
#endif
                Angle_Wrap(item->data);
                item++;
            }
        }
        level->unk_68 = groups;
    }
}

#ifdef TARGET_PORT
#include "structs/region.h"
extern CScene gScene;
/* The port: a path point's region. The item keeps the ROM's region index (-1: none) in its 4-byte
 * slot, which cannot hold a host pointer (--high-memory, macOS), and the region table holds host
 * CGameRegions (0x40 bytes, not the N64's 0x20): the readers (AI_GetPathTargetInfo,
 * AI_UpdatePathHeading) resolve it here, as CScene__GetRegion does. */
sptr CScene__GetPathRegion(s32 ref) {
    if (ref == -1) {
        return 0;
    }
    return (sptr) ((CGameRegion *) ((u8 *) CISet__GetBlock(gScene.m_pRegionData, 2) + 8) + ref);
}
#endif
