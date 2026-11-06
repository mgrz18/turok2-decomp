#include "common.h"

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

typedef struct LevelState {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ void *table;
    /* 0x5C */ u8 pad5C[0xC];
    /* 0x68 */ s32 *groups;
} LevelState;

extern s32 ProPackDecompress(s32 ***handle, s32);
extern void *CISet__GetBlock(void *table, s32 index);
extern void Angle_Wrap(void *);

/* Resolves the references of every item of every group of the loaded resource and keeps it in the level. */
void CScene__DecompressPaths(LevelState *level, s32 ***handle) {
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
                if (ref != -1) {
                    entries = (Entry *)((u8 *)CISet__GetBlock(level->table, 2) + 8);
                    item->ref = (s32)&entries[ref];
                } else {
                    item->ref = 0;
                }
                Angle_Wrap(item->data);
                item++;
            }
        }
        level->groups = groups;
    }
}
