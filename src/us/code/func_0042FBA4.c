/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_9.CPP, func_8044A370_de, with Turok 2's layouts. */
#include "common.h"

typedef struct List {
    void *head;
    void *tail;
    s32 count;
    s32 a;
    s32 b;
} List; /* 0x14 */

typedef struct Entry {
    /* 0x00 */ void *next;
    /* 0x04 */ void *prev;
    /* 0x08 */ s32 a;
    /* 0x0C */ s32 b;
    /* 0x10 */ s32 c;
    /* 0x14 */ f32 value;
    /* 0x18 */ List lists[3];
} Entry; /* 0x54 */

typedef struct Camera {
    /* 0x000 */ void *next;
    /* 0x004 */ void *prev;
    /* 0x008 */ s32 index;
    /* 0x00C */ u8 rest[0xEC0 - 0xC];
} Camera; /* 0xEC0 */

typedef struct CameraText {
    u8 data[0x40];
} CameraText;

typedef struct Cameras {
    /* 0x0000 */ Camera **handle;
    /* 0x0004 */ Camera *cameras;
    /* 0x0008 */ s32 count;
    /* 0x000C */ List active;
    /* 0x0020 */ List free;
    /* 0x0034 */ s32 word34;
    /* 0x0038 */ s32 word38;
    /* 0x003C */ s32 word3C;
    /* 0x0040 */ u8 view[8];
    /* 0x0048 */ s32 word48;
    /* 0x004C */ u8 rest[0xF00 - 0x4C];
    /* 0x0F00 */ CameraText **textHandle;
    /* 0x0F04 */ CameraText *texts;
    /* 0x0F08 */ s32 textCount;
    /* 0x0F0C */ List textList;
    /* 0x0F20 */ Entry entries[8];
    /* 0x11C0 */ List entryList;
    /* 0x11D4 */ List spareList;
    /* 0x11E8 */ s32 empty;
} Cameras;

extern char D_800A9A00[]; /* "Cameras" */
extern char D_800A9A08[]; /* "Camera Texts" */

void func_0042F9B0(void *);
void CList__Construct(List *list, s32 prevOffset, s32 nextOffset);
s32 CList__AddTail(List *list, void *node);
void func_00275AE0(void *);
void func_002053A8(s32);
void func_002052D8(s32, void *);
void *func_00204EDC(s32, s32, s32, char *);
void *func_00266C5C(void *destination, s32 value, u32 size);

static __inline__ void clearList(List *list) {
    list->tail = 0;
    list->count = 0;
    list->a = 0;
    list->b = 0;
}

static __inline__ void initEntry(Entry *entry, f32 value) {
    entry->a = 0;
    entry->b = 0;
    entry->c = 0;
    entry->value = value;
    clearList(&entry->lists[0]);
    clearList(&entry->lists[1]);
    clearList(&entry->lists[2]);
}

void func_0042FBA4(Cameras *owner, s32 count) {
    s32 i;
    f32 value;

    owner->word34 = 0;
    owner->word38 = 0;
    owner->word48 = 0;
    func_0042F9B0(owner->view);
    owner->word38 = 2;
    CList__Construct(&owner->entryList, 0, 4);
    CList__Construct(&owner->spareList, 0, 4);
    value = 1.0f;
    for (i = 0; i < 8; i++) {
        initEntry(&owner->entries[i], value);
        CList__AddTail(&owner->entryList, &owner->entries[i]);
    }
    CList__Construct(&owner->active, 0, 4);
    CList__Construct(&owner->free, 0, 4);
    CList__Construct(&owner->textList, 0, 4);
    owner->empty = 1;
    func_00275AE0(owner->view);
    func_002053A8(0);
    if (owner->handle) {
        func_002052D8(0, owner->handle);
        owner->handle = 0;
        owner->cameras = 0;
        owner->count = 0;
    }
    func_002053A8(0);
    if (owner->textHandle) {
        func_002052D8(0, owner->textHandle);
        owner->textHandle = 0;
        owner->texts = 0;
        owner->textCount = 0;
    }
    if (count) {
        owner->count = count;
        owner->handle = func_00204EDC(0, count * sizeof(Camera), 0x23, D_800A9A00);
        owner->cameras = *owner->handle;
        func_00266C5C(owner->cameras, 0, count * sizeof(Camera));
        for (i = 0; i < owner->count; i++) {
            owner->cameras[i].index = i;
            CList__AddTail(&owner->active, &owner->cameras[i]);
        }
        func_00275AE0(owner->cameras);
        owner->empty = 0;
    }
    owner->textCount = 16;
    owner->textHandle = func_00204EDC(0, 0x400, 0x23, D_800A9A08);
    owner->texts = *owner->textHandle;
    func_00266C5C(owner->texts, 0, 0x400);
    for (i = 0; i < owner->textCount; i++) {
        CList__AddTail(&owner->textList, &owner->texts[i]);
    }
}
