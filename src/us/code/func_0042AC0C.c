/* the real function is 0x0042AC0C-0x0042B3E4 (size 0x7D8, 502 words); splat cut it into CScene__LoadLevel +
 * D_0042B304. All 502 words match (checked over the real extent). Reusing one scratch pointer for the
 * CPlayerPool__GetFirstActive result and the actor array keeps the last loop's load in $v0, `actor += 0x288` gives the player
 * loop its register order, and the sound switch with one shared call keeps w in $s3. */
#include "common.h"
#include "structs/world.h"
#include "structs/game.h"
#include "structs/player.h"
#include "globals/game.h"
#include "globals/engine.h"
#include "globals/render.h"
#include "globals/sysrc.h"
#include "globals/audio.h"
#include "protos/iset.h"
#include "protos/scene.h"
#include "protos/player.h"
#include "protos/game.h"
#ifdef TARGET_PORT
#include "globals/onscreen.h"
#endif


/* Loads a level into the world: frees the previous level's blocks, resets the subsystems, then opens the
 * level's data blocks and spawns its actors. */

extern u8 gEngineApp_m_SwooshSystem[];

extern s32 ProPack__Decompress(s32, sptr);
extern void CCollisionInfo__Reset(void);
extern void CDecompressor__Flush(CDecompressor *thisx);
extern void **CCache__Allocate(s32 unused, s32 size, s32 mode, char *name);
extern void CCache__Deallocate(sptr unused, void **block);
extern void CCache__Flush(s32);
extern void **CCache__LoadPersistantIndex(s32 unused, void **out, sptr key, char *name);
extern void **CCache__LoadPersistantSubBlock(s32 unused, void *file, s32 index, s32 key, CScene *owner, void *callback, char *name);
extern void CCache__SetFrameDelay(s32);
extern void CGameObjectInstance__UpdateAnimation(u8 *actor);
extern void CGameObjectInstance__InitAIType(u8 *actor);
extern void CGameObjectInstance__UpdateAI(u8 *actor);
extern void Sound__StopFlags(s32);
extern void Sound__DetachAll(void);
extern void LoopSounds_Construct(void *, void *, s32);
extern void LoopSounds_StopAll(sptr);
extern s32 Demo_GetPlayerCount(void);
extern void CCameraPool__Advance(void *);
extern s32 Cinema_IsActive(void);
extern void Cinema_RequestOnce(s32 sound);
extern void MatrixHeap_Construct(void *heap, void *data, s32 size);
extern void CScene__RestoreFiredEvents(CScene *w);
extern void CScene__ReapplyFiredEvents(CScene *w);
#ifdef TARGET_PORT /* unprototyped: the host passes the pass-through arguments */
extern void CScene__FlushDelayedEvents();
#else
extern void CScene__FlushDelayedEvents(void);
#endif
extern void SceneActorPool_Construct(CLevelActorPool *);
extern void CInstancePool__Construct(void *);
extern void CDLightSet__Construct(void *);
#ifdef TARGET_PORT
/* the particle data is a pointer: m_pParticleEffects is the cache handle (read pointer-wide) */
extern void CParticleSystem__Construct(CParticleSystem *, sptr);
#else
extern void CParticleSystem__Construct(CParticleSystem *, s32);
#endif
extern void SwooshSystem_Construct(void *);
extern void CPlayerPool__LevelStart(void *);
extern void CPlayer__StopLoopingSounds(void *);
extern void COnScreen__ResetBossBars(void *);
extern void CCameraPool__ResetEmitterPools(void *);

#ifdef TARGET_PORT
void T2Port_RngAtLoad(int level);
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CScene__FlushDelayedEvents receives CScene__LoadLevel's own argument
 * registers (pass-through); the host passes them. */
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

    T2Port_RngAtLoad(level); /* DEBUG-ONLY: T2_RNG_LOAD=T:HEX (port/game/t2debug.c, round 17 f3) */
    if (w->m_nLevel != -1) {
        CScene__FlushDelayedEvents(w);
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
    CDecompressor__Flush(&gDecompressor);
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
    CParticleSystem__Construct(&w->m_ParticleSystem, *(sptr *) w->m_pParticleEffects);
    SceneActorPool_Construct(&w->m_ActorPool);
    CInstancePool__Construct(&w->m_InstancePool);
    SwooshSystem_Construct(gEngineApp_m_SwooshSystem);
    CCollisionInfo__Reset();
    CDLightSet__Construct(&w->m_DLightSet);
    LoopSounds_Construct(&gLoopSounds, gLoopSoundEntries, 0xF);
    /* the ROM finds COnScreen 0x1C8 bytes before the swoosh system (its N64 size); on the host
     * the two are not that far apart: the boss-bar reset ran on the middle of COnScreen (f4) */
    COnScreen__ResetBossBars(&gEngineApp_m_OnScreen);
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
    if (gEngineApp_m_Settings.multiplayer != 0) {
        size += 0x200;
    }
    block = CCache__Allocate(0, size << 6, 3, "Matrices");
    w->m_hHeap = block;
    MatrixHeap_Construct(w->m_Heap, *block, size);
    key = CISet__GetROMAddress(w->m_pLevels, w->m_LevelsKey, level);
    w->m_hLevelIndex = CCache__LoadPersistantIndex(0, (void **)&w->m_pLevelFile, key, "level index");
    block = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 1, key, w, CScene__DecompressCollision, "collision");
    w->m_hRegionData = block;
    w->m_pRegionData = *block;
    block = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 4, key, w, ProPack__Decompress, "grid bounds");
    w->m_hCollision = block;
    w->m_pCollision = *block;
    w->m_LevelInfoKey = CISet__GetROMAddress(w->m_pLevelFile, key, 5);
    w->m_hLevelInfo = CCache__LoadPersistantIndex(0, (void **)&w->m_pLevelInfo, w->m_LevelInfoKey, "grid sections index");
    w->m_hLinks = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 7, key, w, CScene__DecompressLinks, "links");
    w->m_hPaths = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 2, key, w, CScene__DecompressPaths, "level paths");
    block = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 0, key, w, NULL, "level info");
    w->m_hLevelHeader = block;
    w->m_pLevelHeader = *block;
    CScene__LoadLightSettings(w);
    gFogEnabled = (w->m_pLevelHeader->m_Flags >> 2) & 1;
    block = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 6, key, w, ProPack__Decompress, "sky layers");
    w->m_hSections = block;
    data = *block;
    w->m_pSections = data + 8;
    w->m_nSections = *(s32 *)(data + 4);
    w->m_Mode = 1;
    w->m_pActors = NULL;
    w->m_nActors = 0;
    w->m_hActors = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 3, key, w, CScene__DecompressAnimatedInstances, "animated instances");
    w->m_Mode = 2;
    CScene__RestoreFiredEvents(w);
    count = w->m_nActors;
    i = w->m_nFirstActor;
    w->m_Mode = 3;
    for (; i < count; i++) {
        CGameObjectInstance__InitAIType((u8 *)w->m_pActors + i * (s32) sizeof(CGameObjectInstance));
    }
    player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    actor = player;
    while (player != NULL) {
        CGameObjectInstance__InitAIType(actor);
        actor += T2_OFFSETOF(CPlayer, m_WeaponActor);
        CGameObjectInstance__InitAIType(actor);
        player = *(u8 **)(player + T2_OFFSETOF(CPlayer, m_pNext));
        actor = player;
    }
    CScene__ReapplyFiredEvents(w);
    count = w->m_nActors;
    for (i = w->m_nFirstActor; i < count; i++) {
        ptr = (u8 *)w->m_pActors;
        p = ptr + i * (s32) sizeof(CGameObjectInstance);
        CGameObjectInstance__UpdateAI(p);
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
#else
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
        CScene__FlushDelayedEvents();
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
    CDecompressor__Flush(&gDecompressor);
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
    CParticleSystem__Construct(&w->m_ParticleSystem, *w->m_pParticleEffects);
    SceneActorPool_Construct(&w->m_ActorPool);
    CInstancePool__Construct(&w->m_InstancePool);
    SwooshSystem_Construct(gEngineApp_m_SwooshSystem);
    CCollisionInfo__Reset();
    CDLightSet__Construct(&w->m_DLightSet);
    LoopSounds_Construct(&gLoopSounds, gLoopSoundEntries, 0xF);
    COnScreen__ResetBossBars(gEngineApp_m_SwooshSystem - 0x1C8);
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
    if (gEngineApp_m_Settings.multiplayer != 0) {
        size += 0x200;
    }
    block = CCache__Allocate(0, size << 6, 3, "Matrices");
    w->m_hHeap = block;
    MatrixHeap_Construct(w->m_Heap, *block, size);
    key = CISet__GetROMAddress(w->m_pLevels, w->m_LevelsKey, level);
    w->m_hLevelIndex = CCache__LoadPersistantIndex(0, (void **)&w->m_pLevelFile, key, "level index");
    block = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 1, key, w, CScene__DecompressCollision, "collision");
    w->m_hRegionData = block;
    w->m_pRegionData = *block;
    block = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 4, key, w, ProPack__Decompress, "grid bounds");
    w->m_hCollision = block;
    w->m_pCollision = *block;
    w->m_LevelInfoKey = CISet__GetROMAddress(w->m_pLevelFile, key, 5);
    w->m_hLevelInfo = CCache__LoadPersistantIndex(0, (void **)&w->m_pLevelInfo, w->m_LevelInfoKey, "grid sections index");
    w->m_hLinks = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 7, key, w, CScene__DecompressLinks, "links");
    w->m_hPaths = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 2, key, w, CScene__DecompressPaths, "level paths");
    block = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 0, key, w, NULL, "level info");
    w->m_hLevelHeader = block;
    w->m_pLevelHeader = *block;
    CScene__LoadLightSettings(w);
    gFogEnabled = (w->m_pLevelHeader->m_Flags >> 2) & 1;
    block = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 6, key, w, ProPack__Decompress, "sky layers");
    w->m_hSections = block;
    data = *block;
    w->m_pSections = data + 8;
    w->m_nSections = *(s32 *)(data + 4);
    w->m_Mode = 1;
    w->m_pActors = NULL;
    w->m_nActors = 0;
    w->m_hActors = CCache__LoadPersistantSubBlock(0, w->m_pLevelFile, 3, key, w, CScene__DecompressAnimatedInstances, "animated instances");
    w->m_Mode = 2;
    CScene__RestoreFiredEvents(w);
    count = w->m_nActors;
    i = w->m_nFirstActor;
    w->m_Mode = 3;
    for (; i < count; i++) {
        CGameObjectInstance__InitAIType((u8 *)w->m_pActors + i * (s32) sizeof(CGameObjectInstance));
    }
    player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    actor = player;
    while (player != NULL) {
        CGameObjectInstance__InitAIType(actor);
        actor += T2_OFFSETOF(CPlayer, m_WeaponActor);
        CGameObjectInstance__InitAIType(actor);
        player = *(u8 **)(player + T2_OFFSETOF(CPlayer, m_pNext));
        actor = player;
    }
    CScene__ReapplyFiredEvents(w);
    count = w->m_nActors;
    for (i = w->m_nFirstActor; i < count; i++) {
        ptr = (u8 *)w->m_pActors;
        p = ptr + i * (s32) sizeof(CGameObjectInstance);
        CGameObjectInstance__UpdateAI(p);
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
#endif
