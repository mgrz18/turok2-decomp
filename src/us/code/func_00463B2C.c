/* Second VM module copy of func_0042AC0C (same source, linked 0xE0 lower): its own callees are named by their link
 * addresses (func_0042B304/func_0042B5D0/func_0042B960/func_0042CC24 are the callbacks the asm calls D_0042B304,
 * D_0042B5D0, .L0042B960 and func_0042CBE4 + 0x40). */
#include "common.h"

/* Loads a level into the world: frees the previous level's blocks, resets the subsystems, then opens the
 * level's data blocks and spawns its actors. */

typedef struct World {
    /* 0x00000 */ s32 state;
    /* 0x00004 */ u8 pad4[0x8];
    /* 0x0000C */ s32 file;
    /* 0x00010 */ u8 pad10[0x10];
    /* 0x00020 */ s32 infoKey;
    /* 0x00024 */ u8 pad24[0x14];
    /* 0x00038 */ s32 *levelCount;
    /* 0x0003C */ u8 pad3C[0x10];
    /* 0x0004C */ void *levelFile;
    /* 0x00050 */ void *info;
    /* 0x00054 */ void *collision;
    /* 0x00058 */ void *matrices;
    /* 0x0005C */ u8 pad5C[0x38];
    /* 0x00094 */ s32 *paths;
    /* 0x00098 */ void **indexBlock;
    /* 0x0009C */ void **matrixBlock;
    /* 0x000A0 */ void **collisionBlock;
    /* 0x000A4 */ void **infoBlock;
    /* 0x000A8 */ void **boundsBlock;
    /* 0x000AC */ void **sectionsBlock;
    /* 0x000B0 */ void **actorBlock;
    /* 0x000B4 */ void **linksBlock;
    /* 0x000B8 */ u8 padB8[0x10];
    /* 0x000C8 */ void **heapBlock;
    /* 0x000CC */ void **pathsBlock;
    /* 0x000D0 */ u8 heap[0x10];
    /* 0x000E0 */ u8 *actors; /* LevelActor[], 0x288 bytes each */
    /* 0x000E4 */ s32 firstActor;
    /* 0x000E8 */ s32 actorCount;
    /* 0x000EC */ u8 padEC[0x18B0 - 0xEC];
    /* 0x018B0 */ u8 sub18B0[0x10518 - 0x18B0];
    /* 0x10518 */ u8 actorPool[0x13B28 - 0x10518];
    /* 0x13B28 */ u8 sub13B28[0x18E50 - 0x13B28];
    /* 0x18E50 */ u8 *bounds;
    /* 0x18E54 */ u8 pad18E54[0x8];
    /* 0x18E5C */ u8 sub18E5C[0x40];
    /* 0x18E9C */ u8 *sections;
    /* 0x18EA0 */ s32 sectionCount;
    /* 0x18EA4 */ u8 pad18EA4[0x18];
    /* 0x18EBC */ u8 sub18EBC[0xE8];
    /* 0x18FA4 */ s32 streamAll;
    /* 0x18FA8 */ s32 level;
    /* 0x18FAC */ u8 pad18FAC[0xC];
    /* 0x18FB8 */ s32 announce;
    /* 0x18FBC */ u8 pad18FBC[0x14];
    /* 0x18FD0 */ s32 levelId;
} World;

extern char D_800A5EB4[]; /* "Matrices" */
extern char D_800A5EC0[]; /* "level index" */
extern char D_800A5ECC[]; /* "collision" */
extern char D_800A5ED8[]; /* "grid bounds" */
extern char D_800A5EE4[]; /* "grid sections index" */
extern char D_800A5EF8[]; /* "links" */
extern char D_800A5F00[]; /* "level paths" */
extern char D_800A5F0C[]; /* "level info" */
extern char D_800A5F18[]; /* "sky layers" */
extern char D_800A5F24[];
extern s32 D_800AB8A0;
extern s32 D_800B6D20;
extern u8 D_800D6D68[];
extern u8 D_800E68B0[];
extern u8 D_800E6A90[];
extern u8 D_801102B8[];
extern u8 D_80119870[];
extern u8 *D_80119890; /* player list */
extern u8 D_8011AAF1;  /* multiplayer */

extern s32 func_00200AE0(s32, s32);
extern void CCollisionInfo__Reset(void);
extern s32 func_00201848(s32 *levelCount, s32 file, s32 index);
extern void func_00201C08(void *);
extern void **func_00204EDC(s32 unused, s32 size, s32 mode, char *name);
extern void func_002052D8(s32 unused, void **block);
extern void func_002053A8(s32);
extern void **func_00205B94(s32 unused, void **out, s32 key, char *name);
extern void **func_00205D0C(s32 unused, void *file, s32 index, s32 key, World *owner, void *callback, char *name);
extern void func_00206068(s32);
extern void func_0021A344(u8 *actor);
extern void func_0021E5D4(u8 *actor);
extern void func_0021F1B4(u8 *actor);
extern void func_0022629C(World *w, s32 level, void *out, s32 mask);
extern void func_00226390(World *w);
extern void func_00226724(World *w);
extern void *func_002532A8(void *players);
extern void func_002758DC(s32);
extern void func_00275B58(void);
extern void func_00275BE8(void *, void *, s32);
extern void func_00275EFC(s32);
extern s32 func_00275F7C(void);
extern s32 func_0027BC34(void);
extern void func_0027ECF0(void *);
extern s32 func_00284174(void);
extern void func_00412B28(s32 sound);
extern void func_00429E40(void *heap, void *data, s32 size);
extern void func_0042A9A0(World *w);
extern s32 func_0042B304(s32, s32);
extern s32 func_0042B5D0(s32, s32);
extern s32 func_0042B960(s32, s32);
extern void func_0042BE68(World *w);
extern void func_0042C6BC(World *w);
extern void func_0042C760(World *w);
extern void func_0042C88C(void);
extern s32 func_0042CC24(s32, s32);
extern void func_0042CEB0(void *);
extern void func_0042CFC0(void *);
extern void func_0042D040(void *);
extern void func_0042D140(void *, s32);
extern void func_0042D330(void *);
extern void func_0042E7B4(void *);
extern void func_0042EA24(void *);
extern void func_0042EADC(void *);
extern void func_0042FE0C(void *);

void func_00463B2C(World *w, s32 level, s32 announce) {
    void **block;
    u8 *data;
    u8 *player;
    u8 *actor;
    u8 *p;
    u8 *ptr; /* scratch pointer: the leaving player's state, then the actor array */
    s32 key;
    s32 size;
    s32 i;
    s32 count;
    s32 sound;

    if (w->level != -1) {
        func_0042C88C();
    }
    if (func_00284174() == 0 && func_0027BC34() == 0 && announce != 0 && w->announce != 0) {
        switch (w->levelId) {
            case 2000:
                sound = 0x8E;
                break;
            case 3000:
                sound = 0x8F;
                break;
            case 4000:
                sound = 0x90;
                break;
            case 5000:
                sound = 0x91;
                break;
            case 6000:
                sound = 0x92;
                break;
            default:
                goto loaded;
        }
        func_00412B28(sound);
    }
loaded:
    func_00226724(w);
    w->state = 0;
    func_00201C08(D_800D6D68);
    func_002053A8(0);
    if (w->level != -1) {
        ptr = func_002532A8(D_80119870);
        if (ptr != NULL) {
            func_0042EA24(ptr);
        }
        func_00275EFC(func_00275F7C());
        func_00275B58();
        func_002758DC(0x1000);
        func_002052D8(0, w->indexBlock);
        func_002052D8(0, w->matrixBlock);
        func_002052D8(0, w->collisionBlock);
        func_002052D8(0, w->infoBlock);
        func_002052D8(0, w->boundsBlock);
        func_002052D8(0, w->actorBlock);
        func_002052D8(0, w->linksBlock);
        func_002052D8(0, w->pathsBlock);
        func_002052D8(0, w->sectionsBlock);
        func_002052D8(0, w->heapBlock);
    }
    func_002053A8(0);
    D_800B6D20 = 0;
    func_00206068(8);
    func_00226390(w);
    func_0042D140(w->sub18B0, *w->paths);
    func_0042CEB0(w->actorPool);
    func_0042CFC0(w->sub13B28);
    func_0042D330(D_801102B8);
    CCollisionInfo__Reset();
    func_0042D040(w->sub18EBC);
    func_00275BE8(D_800E6A90, D_800E68B0, 0xF);
    func_0042EADC(D_801102B8 - 0x1C8);
    if (*w->levelCount < level) {
        level = *w->levelCount - 1;
        announce = 0;
    }
    w->level = level;
    if (announce == 0) {
        w->announce = 0;
    }
    func_0022629C(w, level, w->sub18E5C, 0x3F);
    switch (w->levelId) {
        case 0x123A:
        case 0x1BBC:
        case 0x1C20:
        case 0x1C84:
        case 0x1CE8:
        case 0x1CE9:
        case 0x1D4C:
        case 0x1D4D:
        case 0x1DB1:
        case 0x1DB2:
            w->streamAll = 1;
            break;
        default:
            w->streamAll = 0;
            break;
    }
    switch (w->levelId) {
        case 0x123A:
        case 0x1BBC:
        case 0x1C20:
        case 0x1C84:
        case 0x1CE8:
        case 0x1CE9:
        case 0x1D4C:
        case 0x1D4D:
        case 0x1DB2:
            size = 0xC80;
            break;
        case 0x1DB1:
            size = 0x4B0;
            break;
        default:
            size = 0x320;
            break;
    }
    if (D_8011AAF1 != 0) {
        size += 0x200;
    }
    block = func_00204EDC(0, size << 6, 3, D_800A5EB4);
    w->heapBlock = block;
    func_00429E40(w->heap, *block, size);
    key = func_00201848(w->levelCount, w->file, level);
    w->indexBlock = func_00205B94(0, &w->levelFile, key, D_800A5EC0);
    block = func_00205D0C(0, w->levelFile, 1, key, w, func_0042B5D0, D_800A5ECC);
    w->matrixBlock = block;
    w->matrices = *block;
    block = func_00205D0C(0, w->levelFile, 4, key, w, func_00200AE0, D_800A5ED8);
    w->collisionBlock = block;
    w->collision = *block;
    w->infoKey = func_00201848(w->levelFile, key, 5);
    w->infoBlock = func_00205B94(0, &w->info, w->infoKey, D_800A5EE4);
    w->linksBlock = func_00205D0C(0, w->levelFile, 7, key, w, func_0042B304, D_800A5EF8);
    w->pathsBlock = func_00205D0C(0, w->levelFile, 2, key, w, func_0042CC24, D_800A5F00);
    block = func_00205D0C(0, w->levelFile, 0, key, w, NULL, D_800A5F0C);
    w->boundsBlock = block;
    w->bounds = *block;
    func_0042A9A0(w);
    D_800AB8A0 = (w->bounds[0x10] >> 2) & 1;
    block = func_00205D0C(0, w->levelFile, 6, key, w, func_00200AE0, D_800A5F18);
    w->sectionsBlock = block;
    data = *block;
    w->sections = data + 8;
    w->sectionCount = *(s32 *)(data + 4);
    w->state = 1;
    w->actors = NULL;
    w->actorCount = 0;
    w->actorBlock = func_00205D0C(0, w->levelFile, 3, key, w, func_0042B960, D_800A5F24);
    w->state = 2;
    func_0042C6BC(w);
    count = w->actorCount;
    i = w->firstActor;
    w->state = 3;
    for (; i < count; i++) {
        func_0021E5D4(w->actors + i * 0x288);
    }
    player = D_80119890;
    actor = player;
    while (player != NULL) {
        func_0021E5D4(actor);
        actor += 0x288;
        func_0021E5D4(actor);
        player = *(u8 **)(player + 0x1320);
        actor = player;
    }
    func_0042C760(w);
    count = w->actorCount;
    for (i = w->firstActor; i < count; i++) {
        ptr = w->actors;
        p = ptr + i * 0x288;
        func_0021F1B4(p);
        func_0021A344(p);
    }
    w->state = 4;
    func_0042E7B4(D_80119870);
    func_0042FE0C(D_80119870 + 0x40);
    func_0027ECF0(D_80119870 + 0x40);
    if (D_80119870[0x1281] != 0 || w->streamAll != 0) {
        func_0042BE68(w);
    }
}
