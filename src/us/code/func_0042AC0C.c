/* the real function is 0x0042AC0C-0x0042B3E4 (size 0x7D8, 502 words); splat cut it into CScene__LoadLevel +
 * D_0042B304. All 502 words match (checked over the real extent). Reusing one scratch pointer for the
 * CPlayerPool__GetFirstActive result and the actor array keeps the last loop's load in $v0, `actor += 0x288` gives the player
 * loop its register order, and the sound switch with one shared call keeps w in $s3. */
#include "common.h"
#include "structs/world.h"
#include "structs/game.h"
#include "globals/render.h"

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
extern s32 gFrameCountGameplay;
extern u8 gDecompressor[];
extern u8 gLoopSoundEntries[];
extern u8 gLoopSounds[];
extern u8 gEngineApp_m_SwooshSystem[];
extern u8 *gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead; /* player list */
extern u8 gEngineApp_m_Settings_multiplayer;  /* multiplayer */

extern s32 ProPack__Decompress(s32, s32);
extern void CCollisionInfo__Reset(void);
extern s32 CISet__GetROMAddress(s32 *levelCount, s32 file, s32 index);
extern void CDecompressor__Flush(void *);
extern void **CCache__Allocate(s32 unused, s32 size, s32 mode, char *name);
extern void CCache__Deallocate(s32 unused, void **block);
extern void CCache__Flush(s32);
extern void **CCache__LoadPersistantIndex(s32 unused, void **out, s32 key, char *name);
extern void **CCache__LoadPersistantSubBlock(s32 unused, void *file, s32 index, s32 key, CScene *owner, void *callback, char *name);
extern void CCache__SetFrameDelay(s32);
extern void CGameObjectInstance__UpdateAnimation(u8 *actor);
extern void func_0021E5D4(u8 *actor);
extern void func_0021F1B4(u8 *actor);
extern void CScene__GetLevelName(CScene *w, s32 level, void *out, s32 mask);
extern void CScene__ClearListCounts(CScene *w);
extern void CScene__ReleaseSectors(CScene *w);
extern void *CPlayerPool__GetFirstActive(void *players);
extern void Sound__StopFlags(s32);
extern void Sound__DetachAll(void);
extern void LoopSounds_Construct(void *, void *, s32);
extern void LoopSounds_StopAll(s32);
extern s32 LoopSounds_Get(void);
extern s32 Demo_GetPlayerCount(void);
extern void CCameraPool__Advance(void *);
extern s32 Cinema_IsActive(void);
extern void Cinema_RequestOnce(s32 sound);
extern void MatrixHeap_Construct(void *heap, void *data, s32 size);
extern void CScene__LoadLightSettings(CScene *w);
extern s32 CScene__DecompressLinks(s32, s32);
extern s32 CScene__DecompressCollision(s32, s32);
extern s32 CScene__DecompressAnimatedInstances(s32, s32);
extern void CScene__StreamAllSections(CScene *w);
extern void func_0042C79C(CScene *w);
extern void func_0042C840(CScene *w);
extern void func_0042C96C(void);
extern s32 CScene__DecompressPaths(s32, s32);
extern void SceneActorPool_Construct(void *);
extern void CInstancePool__Construct(void *);
extern void func_0042D120(void *);
extern void CParticleSystem__Construct(void *, s32);
extern void func_0042D410(void *);
extern void CPlayerPool__LevelStart(void *);
extern void CPlayer__StopLoopingSounds(void *);
extern void func_0042EBBC(void *);
extern void CCameraPool__ResetEmitterPools(void *);

void CScene__LoadLevel(CScene *w, s32 level, s32 announce) {
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
        func_0042C96C();
    }
    if (Cinema_IsActive() == 0 && Demo_GetPlayerCount() == 0 && announce != 0 && w->m_bWarpSpawn != 0) {
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
        Cinema_RequestOnce(sound);
    }
loaded:
    CScene__ReleaseSectors(w);
    w->m_Mode = 0;
    CDecompressor__Flush(gDecompressor);
    CCache__Flush(0);
    if (w->m_nLevel != -1) {
        ptr = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
        if (ptr != NULL) {
            CPlayer__StopLoopingSounds(ptr);
        }
        LoopSounds_StopAll(LoopSounds_Get());
        Sound__DetachAll();
        Sound__StopFlags(0x1000);
        CCache__Deallocate(0, w->m_hLevelIndex);
        CCache__Deallocate(0, w->m_hRegionData);
        CCache__Deallocate(0, w->m_hCollision);
        CCache__Deallocate(0, w->m_hLevelInfo);
        CCache__Deallocate(0, w->m_hLevelHeader);
        CCache__Deallocate(0, w->m_hActors);
        CCache__Deallocate(0, w->m_hLinks);
        CCache__Deallocate(0, w->m_hPaths);
        CCache__Deallocate(0, w->m_hSections);
        CCache__Deallocate(0, w->m_hHeap);
    }
    CCache__Flush(0);
    gFrameCountGameplay = 0;
    CCache__SetFrameDelay(8);
    CScene__ClearListCounts(w);
    CParticleSystem__Construct(w->m_ParticleSystem, *w->m_pParticleEffects);
    SceneActorPool_Construct(w->m_ActorPool);
    CInstancePool__Construct(&w->m_InstancePool);
    func_0042D410(gEngineApp_m_SwooshSystem);
    CCollisionInfo__Reset();
    func_0042D120(w->unk_18EBC);
    LoopSounds_Construct(gLoopSounds, gLoopSoundEntries, 0xF);
    func_0042EBBC(gEngineApp_m_SwooshSystem - 0x1C8);
    if (*w->m_pLevels < level) {
        level = *w->m_pLevels - 1;
        announce = 0;
    }
    w->m_nLevel = level;
    if (announce == 0) {
        w->m_bWarpSpawn = 0;
    }
    CScene__GetLevelName(w, level, w->unk_18E5C, 0x3F);
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
    if (gEngineApp_m_Settings_multiplayer != 0) {
        size += 0x200;
    }
    block = CCache__Allocate(0, size << 6, 3, D_800A5EB4);
    w->m_hHeap = block;
    MatrixHeap_Construct(w->m_Heap, *block, size);
    key = CISet__GetROMAddress(w->m_pLevels, w->m_LevelsKey, level);
    w->m_hLevelIndex = CCache__LoadPersistantIndex(0, (void **)&w->m_pLevelFile, key, D_800A5EC0);
    block = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 1, key, w, CScene__DecompressCollision, D_800A5ECC);
    w->m_hRegionData = block;
    w->m_pRegionData = *block;
    block = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 4, key, w, ProPack__Decompress, D_800A5ED8);
    w->m_hCollision = block;
    w->m_pCollision = *block;
    w->m_LevelInfoKey = CISet__GetROMAddress(w->m_pLevelFile, key, 5);
    w->m_hLevelInfo = CCache__LoadPersistantIndex(0, (void **)&w->m_pLevelInfo, w->m_LevelInfoKey, D_800A5EE4);
    w->m_hLinks = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 7, key, w, CScene__DecompressLinks, D_800A5EF8);
    w->m_hPaths = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 2, key, w, CScene__DecompressPaths, D_800A5F00);
    block = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 0, key, w, NULL, D_800A5F0C);
    w->m_hLevelHeader = block;
    w->m_pLevelHeader = *block;
    CScene__LoadLightSettings(w);
    gFogEnabled = (w->m_pLevelHeader->m_Flags >> 2) & 1;
    block = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 6, key, w, ProPack__Decompress, D_800A5F18);
    w->m_hSections = block;
    data = *block;
    w->m_pSections = data + 8;
    w->m_nSections = *(s32 *)(data + 4);
    w->m_Mode = 1;
    w->m_pActors = NULL;
    w->m_nActors = 0;
    w->m_hActors = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 3, key, w, CScene__DecompressAnimatedInstances, D_800A5F24);
    w->m_Mode = 2;
    func_0042C79C(w);
    count = w->m_nActors;
    i = w->m_nFirstActor;
    w->m_Mode = 3;
    for (; i < count; i++) {
        func_0021E5D4((u8 *)w->m_pActors + i * 0x288);
    }
    player = gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead;
    actor = player;
    while (player != NULL) {
        func_0021E5D4(actor);
        actor += 0x288;
        func_0021E5D4(actor);
        player = *(u8 **)(player + 0x1320);
        actor = player;
    }
    func_0042C840(w);
    count = w->m_nActors;
    for (i = w->m_nFirstActor; i < count; i++) {
        ptr = (u8 *)w->m_pActors;
        p = ptr + i * 0x288;
        func_0021F1B4(p);
        CGameObjectInstance__UpdateAnimation(p);
    }
    w->m_Mode = 4;
    CPlayerPool__LevelStart(&gEngineApp_m_Game.m_PlayerList);
    CCameraPool__ResetEmitterPools(gEngineApp_m_Game.m_CameraPool);
    CCameraPool__Advance(gEngineApp_m_Game.m_CameraPool);
    if (gEngineApp_m_Game.m_Settings.multiplayer != 0 || w->m_bStreamAll != 0) {
        CScene__StreamAllSections(w);
    }
}
