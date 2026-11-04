/* Second VM module copy of func_0042AC0C (same source, linked 0xE0 lower): its own callees are named by their link
 * addresses (func_0042B304/func_0042B5D0/func_0042B960/func_0042CC24 are the callbacks the asm calls D_0042B304,
 * D_0042B5D0, .L0042B960 and func_0042CBE4 + 0x40). */
#include "common.h"
#include "structs/world.h"
#include "structs/game.h"

/* Loads a level into the world: frees the previous level's blocks, resets the subsystems, then opens the
 * level's data blocks and spawns its actors. */

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
extern void **func_00205D0C(s32 unused, void *file, s32 index, s32 key, CWorld *owner, void *callback, char *name);
extern void func_00206068(s32);
extern void func_0021A344(u8 *actor);
extern void func_0021E5D4(u8 *actor);
extern void func_0021F1B4(u8 *actor);
extern void func_0022629C(CWorld *w, s32 level, void *out, s32 mask);
extern void func_00226390(CWorld *w);
extern void func_00226724(CWorld *w);
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
extern void func_0042A9A0(CWorld *w);
extern s32 func_0042B304(s32, s32);
extern s32 func_0042B5D0(s32, s32);
extern s32 func_0042B960(s32, s32);
extern void func_0042BE68(CWorld *w);
extern void func_0042C6BC(CWorld *w);
extern void func_0042C760(CWorld *w);
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

void func_00463B2C(CWorld *w, s32 level, s32 announce) {
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

    if (w->m_nLevel != -1) {
        func_0042C88C();
    }
    if (func_00284174() == 0 && func_0027BC34() == 0 && announce != 0 && w->m_bWarpSpawn != 0) {
        switch (w->m_LevelID) {
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
    w->m_Mode = 0;
    func_00201C08(D_800D6D68);
    func_002053A8(0);
    if (w->m_nLevel != -1) {
        ptr = func_002532A8(&D_80119870.m_PlayerList);
        if (ptr != NULL) {
            func_0042EA24(ptr);
        }
        func_00275EFC(func_00275F7C());
        func_00275B58();
        func_002758DC(0x1000);
        func_002052D8(0, w->m_hLevelIndex);
        func_002052D8(0, w->m_hRegionData);
        func_002052D8(0, w->m_hCollision);
        func_002052D8(0, w->m_hLevelInfo);
        func_002052D8(0, w->m_hLevelHeader);
        func_002052D8(0, w->m_hActors);
        func_002052D8(0, w->m_hLinks);
        func_002052D8(0, w->m_hPaths);
        func_002052D8(0, w->m_hSections);
        func_002052D8(0, w->m_hHeap);
    }
    func_002053A8(0);
    D_800B6D20 = 0;
    func_00206068(8);
    func_00226390(w);
    func_0042D140(w->m_ParticleSystem, *w->m_pParticleEffects);
    func_0042CEB0(w->m_ActorPool);
    func_0042CFC0(&w->m_InstancePool);
    func_0042D330(D_801102B8);
    CCollisionInfo__Reset();
    func_0042D040(w->unk_18EBC);
    func_00275BE8(D_800E6A90, D_800E68B0, 0xF);
    func_0042EADC(D_801102B8 - 0x1C8);
    if (*w->m_pLevels < level) {
        level = *w->m_pLevels - 1;
        announce = 0;
    }
    w->m_nLevel = level;
    if (announce == 0) {
        w->m_bWarpSpawn = 0;
    }
    func_0022629C(w, level, w->unk_18E5C, 0x3F);
    switch (w->m_LevelID) {
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
            w->m_bStreamAll = 1;
            break;
        default:
            w->m_bStreamAll = 0;
            break;
    }
    switch (w->m_LevelID) {
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
    w->m_hHeap = block;
    func_00429E40(w->m_Heap, *block, size);
    key = func_00201848(w->m_pLevels, w->m_LevelsKey, level);
    w->m_hLevelIndex = func_00205B94(0, (void **)&w->m_pLevelFile, key, D_800A5EC0);
    block = func_00205D0C(0, w->m_pLevelFile, 1, key, w, func_0042B5D0, D_800A5ECC);
    w->m_hRegionData = block;
    w->m_pRegionData = *block;
    block = func_00205D0C(0, w->m_pLevelFile, 4, key, w, func_00200AE0, D_800A5ED8);
    w->m_hCollision = block;
    w->m_pCollision = *block;
    w->m_LevelInfoKey = func_00201848(w->m_pLevelFile, key, 5);
    w->m_hLevelInfo = func_00205B94(0, (void **)&w->m_pLevelInfo, w->m_LevelInfoKey, D_800A5EE4);
    w->m_hLinks = func_00205D0C(0, w->m_pLevelFile, 7, key, w, func_0042B304, D_800A5EF8);
    w->m_hPaths = func_00205D0C(0, w->m_pLevelFile, 2, key, w, func_0042CC24, D_800A5F00);
    block = func_00205D0C(0, w->m_pLevelFile, 0, key, w, NULL, D_800A5F0C);
    w->m_hLevelHeader = block;
    w->m_pLevelHeader = *block;
    func_0042A9A0(w);
    D_800AB8A0 = (w->m_pLevelHeader->m_Flags >> 2) & 1;
    block = func_00205D0C(0, w->m_pLevelFile, 6, key, w, func_00200AE0, D_800A5F18);
    w->m_hSections = block;
    data = *block;
    w->m_pSections = data + 8;
    w->m_nSections = *(s32 *)(data + 4);
    w->m_Mode = 1;
    w->m_pActors = NULL;
    w->m_nActors = 0;
    w->m_hActors = func_00205D0C(0, w->m_pLevelFile, 3, key, w, func_0042B960, D_800A5F24);
    w->m_Mode = 2;
    func_0042C6BC(w);
    count = w->m_nActors;
    i = w->m_nFirstActor;
    w->m_Mode = 3;
    for (; i < count; i++) {
        func_0021E5D4((u8 *)w->m_pActors + i * 0x288);
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
    count = w->m_nActors;
    for (i = w->m_nFirstActor; i < count; i++) {
        ptr = (u8 *)w->m_pActors;
        p = ptr + i * 0x288;
        func_0021F1B4(p);
        func_0021A344(p);
    }
    w->m_Mode = 4;
    func_0042E7B4(&D_80119870.m_PlayerList);
    func_0042FE0C(D_80119870.m_CameraPool);
    func_0027ECF0(D_80119870.m_CameraPool);
    if (D_80119870.m_Settings.multiplayer != 0 || w->m_bStreamAll != 0) {
        func_0042BE68(w);
    }
}
