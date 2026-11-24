#include "common.h"
#include "protos/cache.h"
/* The request CCache__LoadPersistantSubBlock leaves in v0 (protos/cache.h declares the N64 definition
 * void): the N64 symbol by another C name, returning it; the port's definition returns it. */
#ifdef TARGET_PORT
#define CCache__LoadPersistantSubBlock_v(...) ((void **) CCache__LoadPersistantSubBlock(__VA_ARGS__))
#else
void **CCache__LoadPersistantSubBlock_v() __asm__("CCache__LoadPersistantSubBlock");
#endif
#include "structs/world.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/iset.h"

/* Level start: resets the world, opens the cartridge index and the level resource tables (levels, texture
 * sets, object types, objects, intelligence, particle effects, binaries, swooshes, lights, curves, rumbles,
 * graphic sets, cinemas, link groups), then loads five geometry headers (objects 0xDAC-0xDB0). */

s32 ProPack__Decompress(s32 unused, void **res);
void Model_DecompressCallback(sptr arg0, sptr arg1);
void CScene__DecompressIntelligence_2(s32 unused, void ***resource);
void CScene__DecompressParticles_2(sptr arg0, void ***arg1);
extern s32 CScene__FindObjectType(CScene *w, s32 id);
extern s32 CScene__GetObject(CScene *w, s32 index);
extern void CScene__ClearListCounts();
extern void CScene__LoadLevel_2(CScene *w, s32, s32);
void CScene__InitPersistentData_2(CScene *menu);
void CScene__ConstructWarp_2(CScene *arg0);
void CPlayerPool__LevelStart_2(void *pool);

/* Copies the header of object id's geometry (or clears it when the object is missing). */
static __inline__ void loadGeomHeader(CScene *w, CGeomHeader *out, s32 id) {
    s32 size;
    s32 index;
    s32 file;
    void **objIndex;
    void **geometry;

    index = CScene__FindObjectType(w, id);
    if (index == -1) {
        out->m_Data[2] = 0;
        return;
    }
    file = CScene__GetObject(w, index);
    objIndex = CCache__Request(0, file, 0x18, 0, 0, NULL, "object index", 1);
    if (objIndex != NULL) {
        geometry = CCache__Request(0, CISet__GetROMAddressAndSize((sptr)*objIndex, file, 1, &size), size, 0, 0, Model_DecompressCallback,
                                 "geometry", 1);
        if (geometry != NULL) {
            *out = *(CGeomHeader *)CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*geometry, 2), 0), 0), 0);
            CCache__Deallocate(0, geometry);
        }
        CCache__Deallocate(0, objIndex);
    }
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CScene__ClearListCounts receives CScene__Construct_2's own argument
 * registers (pass-through); the host passes them. */
void CScene__Construct_2(CScene *w, s32 file) {
    sptr cart;
    sptr res;
    f32 *vec;

    CScene__ClearListCounts(w);
    w->m_Mode = 0;
    CScene__ConstructWarp_2(w);
    w->unk_04 = 0;
    w->unk_08 = 0;
    w->m_nLevel = -1;
    w->unk_18EB8 = 0;
    w->m_SkyAngle = 0;
    CPlayerPool__LevelStart_2(&gEngineApp_m_Game.m_PlayerList);
    vec = &w->m_qSky.x;
    w->m_qSky.x = vec[1] = vec[2] = 0.0f;
    w->m_qSky.w = 1.0f;
    ((void (*)(s32)) CCache__Flush)(0); /* the definition takes none */
    CCache__LoadPersistantIndex(0, &cart, file, "cartridge index");
    res = CISet__GetROMAddress(cart, file, 5);
    w->m_LevelsKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pLevels, res, "levels index");
    res = CISet__GetROMAddress(cart, file, 3);
    w->m_TextureSetsKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pTextureSets, res, "texture sets index");
    w->m_pObjectTypes = *CCache__LoadPersistantSubBlock_v(0, cart, 2, file, w, NULL, "object types");
    res = CISet__GetROMAddress(cart, file, 0);
    w->m_ObjectsKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pObjects, res, "objects index");
    w->m_pIntelligence = *CCache__LoadPersistantSubBlock_v(0, cart, 1, file, w, CScene__DecompressIntelligence_2, "intelligence");
    w->m_pParticleEffects = (s32 *)CCache__LoadPersistantSubBlock_v(0, cart, 4, file, w, CScene__DecompressParticles_2, "particle effects");
    w->m_pObjectTypes2 = *CCache__LoadPersistantSubBlock_v(0, cart, 0xA, file, w, NULL, "object types");
    res = CISet__GetROMAddress(cart, file, 9);
    w->m_BinariesKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pBinaries, res, "binaries index");
    w->m_pSwooshes = *CCache__LoadPersistantSubBlock_v(0, cart, 0xB, file, w, NULL, "particle swooshes");
    w->m_pLights = *CCache__LoadPersistantSubBlock_v(0, cart, 0xC, file, w, NULL, "dynamic lights");
    w->m_pBitSections = gLoadSavePersistentData;
    w->unk_28 = CISet__GetROMAddressAndSize(cart, file, 6, &w->unk_30);
    w->m_pBitSections2 = gGamePersistentData;
    w->unk_2C = CISet__GetROMAddressAndSize(cart, file, 7, &w->unk_34);
    w->m_pCurves = *CCache__LoadPersistantSubBlock_v(0, cart, 0xD, file, w, ProPack__Decompress, "curves");
    w->m_pRumbles = *CCache__LoadPersistantSubBlock_v(0, cart, 0xE, file, w, ProPack__Decompress, "rumbles");
    w->m_pRumbleTypes = *CCache__LoadPersistantSubBlock_v(0, cart, 0xF, file, w, ProPack__Decompress, "rumble types");
    w->m_pGraphicSetTypes = *CCache__LoadPersistantSubBlock_v(0, cart, 0x11, file, w, NULL, "graphicset type");
    res = CISet__GetROMAddress(cart, file, 0x10);
    w->m_GraphicSetsKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pGraphicSets, res, "graphicset index");
    res = CISet__GetROMAddress(cart, file, 0x12);
    w->m_CinemasKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pCinemas, res, "Cinemas index");
    w->m_pLinkGroupTypes = *CCache__LoadPersistantSubBlock_v(0, cart, 0x13, file, w, NULL, "link group types");
    ((void (*)(CScene *, s32)) CScene__InitPersistentData_2)(w, file); /* the definition takes one */
    gPersistentDataLoaded = 1;
    loadGeomHeader(w, &w->m_Geom[0], 0xDAC);
    loadGeomHeader(w, &w->m_Geom[1], 0xDAD);
    loadGeomHeader(w, &w->m_Geom[2], 0xDAE);
    loadGeomHeader(w, &w->m_Geom[3], 0xDAF);
    loadGeomHeader(w, &w->m_Geom[4], 0xDB0);
    CScene__LoadLevel_2(w, 0, 0);
}

#include "structs/light.h"

struct LightSettings;
struct LightSettings {
    char pad0[0xA];
    u8 color[3];
    u8 ambient[3];
    char pad10;
    signed char dir[3];
};
struct SceneRecAt1B2B4;
struct SceneRecAt1B2B4 {
    char pad0[0x18E50]; /* Turok 2 offset (Rage Wars: 0x1B2B0) */
    struct LightSettings *light;
    s16 dir[3];
};
typedef struct SceneRecAt1B2B4 SceneRecAt1B2B4;
extern float D_800A5EB0;
extern f32 func_00298470(f32);
extern void CVector3__Scale(f32 *, f32 *, f32);

void CScene__LoadLightSettings_2(SceneRecAt1B2B4 *scene) {
    s32 i;
    u8 c;
    f32 v[3];
    f32 length;
    u8 *light;
    for (i = 0; i < 3; i++) {
        c = scene->light->ambient[i];
        light = gSceneLights.m_AmbCol;
        light[i] = c;
        gSceneLights.m_AmbColCopy[i] = c;
        c = scene->light->color[i];
        light[i + 8] = c;
        gSceneLights.m_ColCopy[i] = c;
        gSceneLights.m_Dir[i] = scene->light->dir[i];
    }
    v[0] = scene->light->dir[0];
    v[1] = scene->light->dir[1];
    v[2] = scene->light->dir[2];
    length = func_00298470(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    if (length != 0.0f) {
        CVector3__Scale(v, v, D_800A5EB0 / length);
        scene->dir[0] = v[0];
        scene->dir[1] = v[1];
        scene->dir[2] = v[2];
    }
}

#include "globals/swoosh.h"
#include "structs/player.h"
#include "globals/render.h"
#include "globals/sysrc.h"
#include "globals/audio.h"
#include "structs/vector.h"

/* protos/scene.h's prototypes of what this file calls (the module defines them all; the
 * merged file declares them from the definitions). */
struct CCamera;
struct CGameObjectInstance;
struct CScene;
struct CSceneRecord;
struct LevelEvent;
void CScene__ClearListCounts(void *);
void CScene__DecompressAnimatedInstances_2(CScene *world, void *handleArg);
void CScene__DecompressCollision_2(void *worldArg, void *handleArg);
void CScene__DecompressLinks_2(void *worldArg, void ***handle);
void CScene__DecompressPaths_2(CScene *level, s32 ***handle);
void CScene__GetLevelName(struct CScene *, s32, void *, s32);
void CScene__LoadLevel_2(struct CScene *, s32, s32);
void CScene__ReleaseSectors(struct CScene *);
void CScene__StreamAllSections_2(CScene *level);
#include "protos/player.h"
#include "protos/game.h"
#ifdef TARGET_PORT
#include "globals/onscreen.h"
#endif

extern void CCollisionInfo__Reset(void);
extern void CDecompressor__Flush(CDecompressor *thisx);
void CGameObjectInstance__UpdateAnimation(void *actor);
extern void CGameObjectInstance__InitAIType(u8 *actor);
void CGameObjectInstance__UpdateAI(void *actor);
extern void Sound__StopFlags(s32);
extern void Sound__DetachAll(void);
extern void LoopSounds_Construct(void *, void *, s32);
extern void LoopSounds_StopAll(sptr);
extern s32 Demo_GetPlayerCount(void);
extern void CCameraPool__Advance(void *);
extern s32 Cinema_IsActive(void);
extern void Cinema_RequestOnce_2(s32 sound);
extern void MatrixHeap_Construct_2(void *heap, void *data, s32 size);
extern void CScene__RestoreFiredEvents_2(CScene *w);
extern void CScene__ReapplyFiredEvents_2(CScene *w);
void CScene__FlushDelayedEvents_2(struct CScene *);
extern void SceneActorPool_Construct_2(CLevelActorPool *);
extern void CInstancePool__Construct_2(void *);
extern void CDLightSet__Construct_2(void *);
/* the particle data is a pointer: m_pParticleEffects is the cache handle (read pointer-wide) */
extern void CParticleSystem__Construct_2(CParticleSystem *, sptr);
extern void SwooshSystem_Construct_2(void *);
extern void CPlayer__StopLoopingSounds_2(void *);
extern void COnScreen__ResetBossBars_2(void *);
extern void CCameraPool__ResetEmitterPools_2(void *);
#ifdef TARGET_PORT
void T2Port_RngAtLoad(int level);
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CScene__FlushDelayedEvents_2 receives CScene__LoadLevel_2's own argument
 * registers (pass-through); the host passes them. */
void CScene__LoadLevel_2(CScene *w, s32 level, s32 announce) {
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
        CScene__FlushDelayedEvents_2(w);
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
        Cinema_RequestOnce_2(sound);
    }
loaded:
    CScene__ReleaseSectors(w);
    w->m_Mode = 0;
    CDecompressor__Flush(&gDecompressor);
    ((void (*)(s32)) CCache__Flush)(0); /* the definition takes none */
    if (w->m_nLevel != -1) {
        ptr = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
        if (ptr != NULL) {
            CPlayer__StopLoopingSounds_2(ptr);
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
    ((void (*)(s32)) CCache__Flush)(0); /* the definition takes none */
    gFrameCountGameplay = 0;
    CCache__SetFrameDelay(8);
    CScene__ClearListCounts(w);
    CParticleSystem__Construct_2(&w->m_ParticleSystem, *(sptr *) w->m_pParticleEffects);
    SceneActorPool_Construct_2(&w->m_ActorPool);
    CInstancePool__Construct_2(&w->m_InstancePool);
    SwooshSystem_Construct_2(&gEngineApp_m_SwooshSystem);
    CCollisionInfo__Reset();
    CDLightSet__Construct_2(&w->m_DLightSet);
    LoopSounds_Construct(&gLoopSounds, gLoopSoundEntries, 0xF);
    /* the ROM finds COnScreen 0x1C8 bytes before the swoosh system (its N64 size); on the host
     * the two are not that far apart: the boss-bar reset ran on the middle of COnScreen (f4) */
    COnScreen__ResetBossBars_2(&gEngineApp_m_OnScreen);
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
    MatrixHeap_Construct_2(w->m_Heap, *block, size);
    key = CISet__GetROMAddress(w->m_pLevels, w->m_LevelsKey, level);
    w->m_hLevelIndex = CCache__LoadPersistantIndex(0, (void **)&w->m_pLevelFile, key, "level index");
    block = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 1, key, w, CScene__DecompressCollision_2, "collision");
    w->m_hRegionData = block;
    w->m_pRegionData = *block;
    block = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 4, key, w, ProPack__Decompress, "grid bounds");
    w->m_hCollision = block;
    w->m_pCollision = *block;
    w->m_LevelInfoKey = CISet__GetROMAddress(w->m_pLevelFile, key, 5);
    w->m_hLevelInfo = CCache__LoadPersistantIndex(0, (void **)&w->m_pLevelInfo, w->m_LevelInfoKey, "grid sections index");
    w->m_hLinks = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 7, key, w, CScene__DecompressLinks_2, "links");
    w->m_hPaths = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 2, key, w, CScene__DecompressPaths_2, "level paths");
    block = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 0, key, w, NULL, "level info");
    w->m_hLevelHeader = block;
    w->m_pLevelHeader = *block;
    CScene__LoadLightSettings_2(w);
    gFogEnabled = (w->m_pLevelHeader->m_Flags >> 2) & 1;
    block = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 6, key, w, ProPack__Decompress, "sky layers");
    w->m_hSections = block;
    data = *block;
    w->m_pSections = data + 8;
    w->m_nSections = *(s32 *)(data + 4);
    w->m_Mode = 1;
    w->m_pActors = NULL;
    w->m_nActors = 0;
    w->m_hActors = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 3, key, w, CScene__DecompressAnimatedInstances_2, "animated instances");
    w->m_Mode = 2;
    CScene__RestoreFiredEvents_2(w);
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
    CScene__ReapplyFiredEvents_2(w);
    count = w->m_nActors;
    for (i = w->m_nFirstActor; i < count; i++) {
        ptr = (u8 *)w->m_pActors;
        p = ptr + i * (s32) sizeof(CGameObjectInstance);
        CGameObjectInstance__UpdateAI(p);
        CGameObjectInstance__UpdateAnimation(p);
    }
    w->m_Mode = 4;
    CPlayerPool__LevelStart_2(&gEngineApp_m_Game.m_PlayerList);
    CCameraPool__ResetEmitterPools_2(gEngineApp_m_Game.m_CameraPool);
    CCameraPool__Advance(gEngineApp_m_Game.m_CameraPool);
    if (gEngineApp_m_Game.m_Settings.multiplayer != 0 || w->m_bStreamAll != 0) {
        CScene__StreamAllSections_2(w);
    }
}
#else
void CScene__LoadLevel_2(CScene *w, s32 level, s32 announce) {
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
        ((void (*)(void)) CScene__FlushDelayedEvents_2)(); /* see the ABI note above */
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
        Cinema_RequestOnce_2(sound);
    }
loaded:
    CScene__ReleaseSectors(w);
    w->m_Mode = 0;
    CDecompressor__Flush(&gDecompressor);
    ((void (*)(s32)) CCache__Flush)(0); /* the definition takes none */
    if (w->m_nLevel != -1) {
        ptr = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
        if (ptr != NULL) {
            CPlayer__StopLoopingSounds_2(ptr);
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
    ((void (*)(s32)) CCache__Flush)(0); /* the definition takes none */
    gFrameCountGameplay = 0;
    CCache__SetFrameDelay(8);
    CScene__ClearListCounts(w);
    CParticleSystem__Construct_2(&w->m_ParticleSystem, *w->m_pParticleEffects);
    SceneActorPool_Construct_2(&w->m_ActorPool);
    CInstancePool__Construct_2(&w->m_InstancePool);
    SwooshSystem_Construct_2(&gEngineApp_m_SwooshSystem);
    CCollisionInfo__Reset();
    CDLightSet__Construct_2(&w->m_DLightSet);
    LoopSounds_Construct(&gLoopSounds, gLoopSoundEntries, 0xF);
    COnScreen__ResetBossBars_2((u8 *) &gEngineApp_m_SwooshSystem - 0x1C8);
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
    MatrixHeap_Construct_2(w->m_Heap, *block, size);
    key = CISet__GetROMAddress(w->m_pLevels, w->m_LevelsKey, level);
    w->m_hLevelIndex = CCache__LoadPersistantIndex(0, (void **)&w->m_pLevelFile, key, "level index");
    block = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 1, key, w, CScene__DecompressCollision_2, "collision");
    w->m_hRegionData = block;
    w->m_pRegionData = *block;
    block = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 4, key, w, ProPack__Decompress, "grid bounds");
    w->m_hCollision = block;
    w->m_pCollision = *block;
    w->m_LevelInfoKey = CISet__GetROMAddress(w->m_pLevelFile, key, 5);
    w->m_hLevelInfo = CCache__LoadPersistantIndex(0, (void **)&w->m_pLevelInfo, w->m_LevelInfoKey, "grid sections index");
    w->m_hLinks = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 7, key, w, CScene__DecompressLinks_2, "links");
    w->m_hPaths = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 2, key, w, CScene__DecompressPaths_2, "level paths");
    block = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 0, key, w, NULL, "level info");
    w->m_hLevelHeader = block;
    w->m_pLevelHeader = *block;
    CScene__LoadLightSettings_2(w);
    gFogEnabled = (w->m_pLevelHeader->m_Flags >> 2) & 1;
    block = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 6, key, w, ProPack__Decompress, "sky layers");
    w->m_hSections = block;
    data = *block;
    w->m_pSections = data + 8;
    w->m_nSections = *(s32 *)(data + 4);
    w->m_Mode = 1;
    w->m_pActors = NULL;
    w->m_nActors = 0;
    w->m_hActors = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 3, key, w, CScene__DecompressAnimatedInstances_2, "animated instances");
    w->m_Mode = 2;
    CScene__RestoreFiredEvents_2(w);
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
    CScene__ReapplyFiredEvents_2(w);
    count = w->m_nActors;
    for (i = w->m_nFirstActor; i < count; i++) {
        ptr = (u8 *)w->m_pActors;
        p = ptr + i * (s32) sizeof(CGameObjectInstance);
        CGameObjectInstance__UpdateAI(p);
        CGameObjectInstance__UpdateAnimation(p);
    }
    w->m_Mode = 4;
    CPlayerPool__LevelStart_2(&gEngineApp_m_Game.m_PlayerList);
    CCameraPool__ResetEmitterPools_2(gEngineApp_m_Game.m_CameraPool);
    CCameraPool__Advance(gEngineApp_m_Game.m_CameraPool);
    if (gEngineApp_m_Game.m_Settings.multiplayer != 0 || w->m_bStreamAll != 0) {
        CScene__StreamAllSections_2(w);
    }
}
#endif
#include "structs/list.h"
#include "protos/list.h"

struct IntPair_0042B3E4;
struct IntPair_0042B3E4 {
    int field_0;
    int field_4;
};
struct Bucket;
typedef struct Bucket Bucket;
struct Bucket {
    struct Bucket *prev;
    struct Bucket *next;
    struct Record14 *object;
    f32 remaining;
};
struct Face;
struct Face {
    char pad0[0xE];
    u8 flags;
    u8 hidden;
    char pad10[3];
    u8 group;
};
typedef struct Face Face;
#ifdef TARGET_PORT /* the same fields at the host offsets of CScene (structs/world.h) */
#include "structs/world.h"
struct WorldRecAt1B40C {
    char pad0[__builtin_offsetof(CScene, m_pBitSections)];
    void *tree;
    char pad70[__builtin_offsetof(CScene, m_nRecords) - __builtin_offsetof(CScene, m_pBitSections) - sizeof(void *)];
    s32 nfaces;
    s32 nedges;
    void *verts;
    void *edgeData;
    struct Face *faces;
    struct Face *edges;
    CList listA;
    CList listB;
    CDelayedEvent buckets[0x30];
    char pad14A8[__builtin_offsetof(CScene, m_nLevel) - __builtin_offsetof(CScene, m_nSectors)];
    s32 group;
};
_Static_assert(__builtin_offsetof(struct WorldRecAt1B40C, listA) == __builtin_offsetof(CScene, m_DelayedEvents), "listA");
_Static_assert(__builtin_offsetof(struct WorldRecAt1B40C, faces) == __builtin_offsetof(CScene, m_pRecords), "faces");
_Static_assert(__builtin_offsetof(struct WorldRecAt1B40C, group) == __builtin_offsetof(CScene, m_nLevel), "group");
#define BUCKET_LINKS T2_OFFSETOF(CDelayedEvent, m_pLast), T2_OFFSETOF(CDelayedEvent, m_pNext)
#else
#define BUCKET_LINKS 0, 4
struct WorldRecAt1B40C;
struct WorldRecAt1B40C {
    char pad0[0x6C];
    void *tree;              /* 0x6C (0x80 in Rage Wars) */
    char pad70[0x1168 - 0x70];
    s32 nfaces;              /* 0x1168 (0x11C0): Turok 2 tables sit 0x58 earlier */
    s32 nedges;
    void *verts;
    void *edgeData;
    struct Face *faces;
    struct Face *edges;
    CList listA;     /* 0x1180 */
    CList listB;     /* 0x1194 */
    Bucket buckets[0x30];    /* 0x11A8 */
    char pad14A8[0x18FA8 - 0x14A8];
    s32 group;               /* 0x18FA8 (0x1B40C) */
};
#endif

struct WorldRecAt1B40C;
typedef struct WorldRecAt1B40C WorldRecAt1B40C;
s32 ProPackDecompress(void *handle, s32 arg1);

static __inline__ s32 groupHas(WorldRecAt1B40C *world, s32 n) {
    s32 index;
    void *g;
    u8 *bits;
    s32 mask;
    index = world->group;
    g = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(world->tree, 0), index), 2);
    CISet__GetBlock(g, 0);
    CISet__GetBlockSize(g, 1);
    bits = CISet__GetBlock(g, 1);
    mask = 1 << (n & 7);
    return bits[n / 8] & mask;
}

#define world ((WorldRecAt1B40C *) worldArg)

/* the definition takes the callers' type (one prototype for the merged object) (its view's type differs per build); the Rage Wars view reads it */
void CScene__DecompressLinks_2(void *worldArg, void ***handle) {
    void *node;
    struct IntPair_0042B3E4 *l0;
    struct IntPair_0042B3E4 *l1;
    struct IntPair_0042B3E4 *l2;
    struct IntPair_0042B3E4 *l3;
    s32 k;
    s32 i;
    s32 n;
    s32 count;
    Face *face;
    if (ProPackDecompress(handle, 0) != 0) {
        node = **handle;
        l0 = CISet__GetBlock(node, 0);
        l1 = CISet__GetBlock(node, 1);
        l2 = CISet__GetBlock(node, 2);
        l3 = CISet__GetBlock(node, 3);
        count = l0->field_4;
        world->verts = &((ResidentDataChunk *)(l0))->data[0];
        world->faces = (Face *)&((ResidentDataChunk *)(l1))->data[0];
        world->nfaces = count;
        count = l2->field_4;
        world->edgeData = &((ResidentDataChunk *)(l2))->data[0];
        world->edges = (Face *)&((ResidentDataChunk *)(l3))->data[0];
        world->nedges = count;
        CList__Construct(&world->listA, BUCKET_LINKS);
        CList__Construct(&world->listB, BUCKET_LINKS);
        for (k = 0; k < 0x30; k++) {
            CList__AddTail(&world->listB, &world->buckets[k]);
        }
        n = world->nfaces;
        for (i = 0; i < n; i++) {
            face = &world->faces[i];
            if (face->hidden != 0) {
                face->flags &= 0xFE;
            }
            if (groupHas(world, face->group)) {
                face->flags |= 0xF0;
            }
        }
        n = world->nedges;
        for (i = 0; i < n; i++) {
            face = &world->edges[i];
            if (face->hidden != 0) {
                face->flags &= 0xFE;
            }
            if (groupHas(world, face->group)) {
                face->flags |= 0xF0;
            }
        }
    }
}

#undef world
#include "globals/camera.h"
#include "structs/collision.h"
#include "structs/twin_sceneload.h"
#define WORLD_TREE(w) (((CScene *)(w))->m_pBitSections2)
#define WORLD_GROUP(w) (((CScene *)(w))->m_nLevel)

extern void UnpackScaledVec4(CollisionBuildVertex *v);
extern void CGameRegion__Unpack(CollisionBuildTriangle *tri, CollisionBuildFace *face, CollisionBuildVertex *verts,
                          s32 nverts, CollisionBuildTriangle *tris, s32 ntris);
extern s32 Cinema_PathPlaying(void);

static __inline__ s32 testBit(s32 n, u8 *bits) {
    s32 mask = 1 << (n & 7);

    return bits[n / 8] & mask;
}

/* Packs a collision mesh into one allocated block: copies the node's vertex list, the triangle source
   and the trailing chunk behind a five-offset header, builds a 0x20-byte triangle record per source
   face and flags triangles whose group bit, the cinema state or neighbours mark them with the default
   surface mask. */
/* the callers' (void *, void *) (one prototype for the merged object); the views read them */
void CScene__DecompressCollision_2(void *worldArg, void *handleArg) {
#define world ((CollisionBuildWorld *) worldArg)
#define handle ((CollisionBuildMeshHandle **) handleArg)
    s32 size0;
    s32 size1;
    s32 size3;
    void *chunk;
    s32 tableSize;
    char *base;
    CollisionBuildVertex *verts;
    CollisionBuildTriangle *tris;
    CollisionBuildMeshHandle *block;
    void *node;
    void *first;
    CollisionBuildVertexList *vlist;
    CollisionBuildList *flist;
    s32 ntris;
    CollisionBuildHeader *hdr;
    s32 offset;
    char *dst;
    char *vdst;
    CollisionBuildVertexList *copiedVertices;
    CollisionBuildTriangleList *copiedTriangles;
    s32 nverts;
    s32 i;
    s32 j;
    s32 k;
    CollisionBuildTriangle *tri;
    CollisionBuildFace *face;
    void *group;
    s32 index;
    u8 *bits;

    if (ProPackDecompress(handle, 1) != 0) {
        node = (*handle)->data;
        first = CISet__GetBlockAndSize(node, 0, &size0);
        vlist = CISet__GetBlockAndSize(node, 1, &size1);
        flist = CISet__GetBlock(node, 2);
        ntris = flist->count;
        chunk = CISet__GetBlockAndSize(node, 3, &size3);
#ifdef TARGET_PORT /* host triangles (CGameRegion) are larger than 0x20 */
        tableSize = ntris * sizeof(CollisionBuildTriangle) + 8;
#else
        tableSize = (ntris << 5) | 8;
#endif
        block = CCache__AllocateMemoryForDecompression(0, handle, size0 + 0x18 + size1 + tableSize + size3);
        if (block != 0) {
            offset = 0x18;
            hdr = block->data;
            i = 0;
            hdr->count = 0;
            hdr->offsets[0] = offset;
            CUSet__Transpose(first, (char *)hdr + offset, 0);
            hdr->count = 1;
            offset += size0;
            vdst = (char *)hdr + offset;
            hdr->offsets[1] = offset;
            CUSet__Transpose(vlist, vdst, 0);
            hdr->count = 2;
            offset += size1;
            dst = (char *)hdr + offset;
            hdr->offsets[2] = offset;
            CUSet__Transpose(flist, dst, 1);
            face = flist->items;
            copiedTriangles = (void *)dst;
            copiedTriangles->kind = 0x20;
            copiedTriangles->count = ntris;
            base = (char *)hdr;
            tris = copiedTriangles->items;
            nverts = vlist->count;
            copiedVertices = (void *)vdst;
            verts = copiedVertices->items;
            for (i = 0; i < nverts; i++) {
                UnpackScaledVec4(&verts[i]);
            }
            for (j = 0; j < ntris; j++) {
                tri = &tris[j];
                CGameRegion__Unpack(tri, &face[j], verts, nverts, tris, ntris);
                index = WORLD_GROUP(world);
                group = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(WORLD_TREE(world), 0), index), 0);
                CISet__GetBlock(group, 0);
                CISet__GetBlockSize(group, 1);
                bits = CISet__GetBlock(group, 1);
                if (testBit(j, bits)) {
                    tri->flags |= 0x400;
                }
                if (!(tri->mask & gDefaultVisBits)) {
                    tri->mask = gDefaultVisBits;
                }
                if (Cinema_PathPlaying() != 0) {
                    tri->mask = gDefaultVisBits;
                }
                if (tri->flags & 0x2000) {
                    tri->mask = gDefaultVisBits;
                }
            }
            for (j = 0; j < ntris; j++) {
                tri = &tris[j];
                for (k = 0; k < 3; k++) {
                    if (tri->adj[k] != 0 && (tri->adj[k]->flags & 0x2000)) {
                        { s32 *surface = &tri->mask; *surface = gDefaultVisBits; }
                    }
                }
            }
            offset += tableSize;
            hdr->count = 3;
            hdr->offsets[3] = offset;
            CUSet__Transpose(chunk, base + offset, 0);
            hdr->count = 4;
            offset += size3;
            hdr->offsets[4] = offset;
        }
        CCache__DeallocAndReplaceCompressedData(0, handle, block);
    }
#undef world
#undef handle
}

#include "structs/game_object.h"

typedef struct Handle {
    void *data;
} Handle;
extern sptr CScene__GetRegion(CScene *world, s32);
s32 CScene__FindNearestWarpDest_2(void *world, void *position, s32 type, SearchPoint *out);
sptr CScene__GetIntelligence(CScene *world, s32 id);
/* called with the record's u16 id and its result read as a pointer, as the ROM's code has it */
#define CScene__GetIntelligence_u16 ((s32 *(*)(CScene *, u16)) CScene__GetIntelligence)

extern void CGameObjectInstance__ConstructFromRecord(CGameObjectInstance *obj, CObjectRecord *rec, s32 mode, s32 index);
extern sptr CScene__FindRegionAt(CScene *world, CVector3 *pos);
void Angle_Wrap(f32 *value);
extern void CPlayer__SetupActors_2(CPlayer *player, CObjectRecord *rec, s32 mode, s32 index);
extern s32 GameObject_IsFollower(CGameObjectInstance *obj);
extern s32 GameObject_IsLeader(CGameObjectInstance *obj);
extern f32 AI_DistSqToPointAtHeight(CGameObjectInstance *obj, CVector3 pos);
/* Builds the level's actor block from its records: places every player on the spawn points found for
   the level (one record per player start), then links each remaining actor to the nearest actor with
   the same definition. The spawn point is indexed afresh on each use and the position copied with
   memcpy (as in Rage Wars): both set loop.c's insn count and giv benefits, which decide the ROM's
   hoisting of the spawns base and of the angle constants. */
#define handle ((Handle **) handleArg)
typedef struct Placed {
    /* 0x00 */ s32 data[4];
    /* 0x10 */ s16 kind;
    /* 0x12 */ s16 unk12;
} Placed; /* 0x14 */
s32 CScene__FindWarpDests_2(void *world, s32 kind, s32 key, Placed *out, s32 max);

/* the definition takes the callers' type (one prototype for the merged object); its handle view reads it */
void CScene__DecompressAnimatedInstances_2(CScene *world, void *handleArg) {
    CWarpPoint spawns[4];
    CGameObjectInstance *objects;
    sptr mode;
    Handle *block;
    s32 nspawns;
    CObjectRecordBlock *node;
    CObjectRecord *records;
    CInstanceBlock *hdr;
    s32 n;
    s32 i;
    s32 j;
    s32 playerIndex;
    s32 spawnIndex;
    CObjectRecord *rec;
    CObjectRecord *recWalk;
    CGameObjectInstance *objWalk;
    CPlayer *player;
    CGameObjectInstance *obj;
    CGameObjectInstance *other;
    f32 angle;
    f32 best;
    f32 dist;
    s16 value;

    if (ProPackDecompress(handle, 1) != 0) {
        mode = CScene__GetRegion(world, 0);
        node = (*handle)->data;
        n = node->count;
        block = CCache__AllocateMemoryForDecompression(0, handle, n * sizeof(CGameObjectInstance) + 8);
        if (block != NULL) {
            CUSet__Transpose(node, block->data, 1);
            hdr = block->data;
            hdr->size = sizeof(CGameObjectInstance);
            hdr->count = n;
            records = node->records;
            objects = hdr->objects;
            if (world->m_bWarpSpawn == 0) {
                world->m_LevelID = CScene__FindNearestWarpDest_2(world, &node->records[0].m_vPos, world->m_nLevel, &world->m_WarpSpawn);
                if (world->m_LevelID != 0) {
                    world->m_bWarpSpawn = 1;
                }
            }
            spawnIndex = 0;
            nspawns = 0;
            if (world->m_bWarpSpawn != 0) {
                nspawns = CScene__FindWarpDests_2(world, world->m_nLevel, world->m_LevelID, spawns, 4);
            }
            world->m_pActors = (CGameObjectInstance *)objects;
            world->m_nActors = n;
            i = 0;
            if (i < n) {
                objWalk = objects;
                recWalk = records;
                while (i < n) {
                    rec = recWalk;
                    obj = objWalk;
                    i++;
                    obj->header.m_pIntelligence = CScene__GetIntelligence_u16(world, rec->m_nIntelligence);
                    objWalk = obj + 1;
                    recWalk = rec + 1;
                }
            }
            world->m_nFirstActor = 0;
            for (i = 0; i < n; i++) {
                rec = &records[i];
                obj = &objects[i];
                CGameObjectInstance__ConstructFromRecord(obj, rec, mode, i);
                if (*CScene__GetIntelligence_u16(world, rec->m_nIntelligence) != 0xB && i != 0) {
                    continue;
                }
                world->m_nFirstActor++;
                memcpy(&gEngineApp_m_Game_m_CameraPool.m_MainCamera.m_ViewParams.m_vPos, &rec->m_vPos, sizeof(CVector3));
                gEngineApp_m_Game_m_CameraPool.m_MainCamera.m_ViewParams.m_vPos.y += 204.79999f;
                gEngineApp_m_Game_m_CameraPool.m_MainCamera.m_ViewParams.m_pCurrentRegion = CScene__FindRegionAt(world, &rec->m_vPos);
                for (playerIndex = 0; playerIndex < gEngineApp_m_Game.m_PlayerList.m_nPlayers; playerIndex++) {
                    player = &gEngineApp_m_Game.m_PlayerList.m_Players[playerIndex];
                    if (nspawns != 0) {
                        rec->m_nRegion = spawns[spawnIndex].m_nID;
                        Angle_Wrap(&spawns[spawnIndex].m_RotY);
                        angle = spawns[spawnIndex].m_RotY;
                        if (!(angle > 3.1415927f)) {
                            if (!(angle < -3.1415927f)) {
                                if (!(angle > 3.1415927f)) {
                                    value = angle * 10430.06f;
                                } else {
                                    value = 0x7FFF;
                                }
                            } else {
                                value = -0x7FFF;
                            }
                        } else {
                            value = 0x7FFF;
                        }
                        rec->m_Angle = value;
                        memcpy(&rec->m_vPos, &spawns[spawnIndex].m_vPos, sizeof(CVector3));
                        spawnIndex++;
                        if (spawnIndex >= nspawns) {
                            spawnIndex = 0;
                        }
                    }
                    CPlayer__SetupActors_2(player, rec, mode, i);
                }
            }
            for (i = world->m_nFirstActor; i < n; i++) {
                obj = &objects[i];
                best = 3.4028235e38f;
                obj->m_AI.m_pParent = NULL;
                if (GameObject_IsFollower(obj) == 0) {
                    continue;
                }
                for (j = world->m_nFirstActor; j < n; j++) {
                    other = &objects[j];
                    if (obj->header.m_pIntelligence->m_Type == other->header.m_pIntelligence->m_Type && GameObject_IsLeader(other) != 0 && obj != other) {
                        if (obj->m_AI.m_pParent != NULL) {
                            dist = AI_DistSqToPointAtHeight(obj, other->header.m_vPos);
                            if (dist < best) {
                                obj->m_AI.m_pParent = other;
                                best = dist;
                            }
                        } else {
                            obj->m_AI.m_pParent = other;
                            best = AI_DistSqToPointAtHeight(obj, other->header.m_vPos);
                        }
                    }
                }
            }
        }
        CCache__DeallocAndReplaceCompressedData(0, handle, block);
        world->m_Kind1Cursor = 0;
    }
}

#undef handle

void CScene__DecompressGridSection(struct CScene *, s32 ***);
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

void CStaticObject__RequestLitVertices(void *object, void *camera);
extern void *CStaticObject__LoadGeometry(ObjA *a);
extern void *CLevelActor__LoadGeometry(ObjB *b);
extern void Model_RequestTextures(void *handle);

/* Streams in every grid cell of the level that is not loaded yet and spawns its objects. The loops run over
 * a rectangle of cells (firstRow..lastRow, firstCol..lastCol), here the whole grid: the column loop starting
 * from the variable firstCol, not the constant 0, is what keeps its entry test as the ROM's slt (Rage Wars'
 * decomp matches the same function with a lone `first` local it flags as a fake match). */
void CScene__StreamAllSections_2(CScene *level) {
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

#include "globals/strings.h"

typedef struct Node {
    /* 0x00 */ s32 index;
    /* 0x04 */ void *prev;
    /* 0x08 */ void *next;
} Node;
s32 BinaryRange(void *array, s32 count, u32 key, s32 *first, s32 *last);

/* Copies up to max placed objects of the given kind (-1: any) whose key matches into out; returns how many. */
s32 CScene__FindWarpDests_2(void *world, s32 kind, s32 key, Placed *out, s32 max) {
    ListRec list;
    Node nodes[64];
    sptr resource;          /* the index (a pointer) */
    s32 first;
    s32 last;
    s32 found;
    s32 *keys;
    s32 *objectTable;
    Placed *objects;
    Node *node;
    s32 i;
    s32 match;
    s32 count;
    void **handle;
    void *table;
    s32 pool;

    found = 0;
    pool = gEngineApp.m_CartridgeIndex;
    CCache__LoadPersistantIndex(found, &resource, pool, sCSceneCacheDesc_CartridgeIndex);
    handle = CCache__LoadPersistantSubBlock_v(found, resource, 8, pool, world, 0, "warp dests");
    table = *handle;
    count = (keys = CISet__GetBlock(table, 0))[1];
    keys += 2;
    objectTable = CISet__GetBlock(table, 1);
    objects = (Placed *)(objectTable + 2);
    if (BinaryRange(keys, count, key, &first, &last) != 0) {
        CList__Construct(&list, T2_OFFSETOF(Node, prev), T2_OFFSETOF(Node, next));
        node = nodes;
        for (i = first; i <= last; i++) {
            match = 1;
            if (kind != -1) {
                match = objects[i].kind == kind;
            }
            if (match) {
                node->index = i;
                CList__AddHead(&list, node);
                node++;
            }
        }
        for (max--; max != -1; max--) {
            if (list.count == 0) {
                break;
            }
            node = CList__GetRandom(&list);
            out[found++] = objects[node->index];
            CList__Remove(&list, node);
        }
    }
    CCache__Deallocate(0, handle);
    return found;
}
void CScene__LoadBitSections2_2(CScene *menu);
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 done;
    /* 0x08 */ u8 pad8[0x28 - 0x8];
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[4];
    /* 0x30 */ s32 unk30;
} Arg0_0042C418;
void CScene__LoadBitSections_2(Arg0_0042C418 *arg0);

void CScene__InitPersistentData_2(CScene *menu) {
    CScene__LoadBitSections_2(menu);
    CScene__LoadBitSections2_2(menu);
    menu->m_PreviewState = 0;
    menu->m_PreviewTimer = 34;
    menu->m_PreviewOptions = gEngineApp_m_Settings.unlockedCheats;
}

void CLoader__LoadSync(void *reader, sptr address, sptr length, void *dest);
s32 SavedPlayers_IsActive();                                /* extern */

void CScene__LoadBitSections_2(Arg0_0042C418 *arg0) {
    if (SavedPlayers_IsActive() == 0) {
        CLoader__LoadSync(&gLoader, arg0->unk28, arg0->unk30, (sptr) &gLoadSavePersistentData);
        arg0->done = 1;
    }
}

extern s32 SavedPlayers_IsActive(void);

void CScene__LoadBitSections2_2(CScene *menu) {
    if (SavedPlayers_IsActive() == 0) {
        CLoader__LoadSync(&gLoader, menu->unk_2C, menu->unk_34, gGamePersistentData);
    }
}
void CScene__RequestWarp_2(CScene *arg0, s32 arg1, s32 arg2);
/* the N64 symbol by another C name: the merged object defines it DEFERRED (inline), and a call by its own
 * name after that definition would expand it (the CGameObjectInstance__LoadGeometry_call pattern) */
#ifdef TARGET_PORT
#define CScene__RequestWarp_call CScene__RequestWarp_2
#else
void CScene__RequestWarp_call(struct CScene *, s32, s32) __asm__("CScene__RequestWarp_2");
#endif

void CScene__WarpToLevel_2(CScene *arg0, s32 arg1);

void CScene__WarpToLevel_2(CScene *arg0, s32 arg1) {
    if (arg1 != arg0->m_nLevel) {
        arg0->m_bWarpSpawn = 0;
        CScene__RequestWarp_call(arg0, ~arg1, 0);
    }
}

void CScene__ConstructWarp_2(CScene *arg0) {
    arg0->m_WarpFade = 2;
    arg0->m_WarpState = 0;
    arg0->m_WarpTime = 0;
    arg0->m_bWarpSpawn = 0;
    arg0->m_LevelID = 0;
    arg0->m_WarpID = 0;
    arg0->unk_18FD8 = 0;
    arg0->m_PendingWarpID = 0;
}

extern f32 CVector3__Distance(void *a, void *b);

/* Finds the point of the given type nearest to position; copies it to out and returns its value (0 when none). */
s32 CScene__FindNearestWarpDest_2(void *world, void *position, s32 type, SearchPoint *out) {
    s32 i;
    s32 best;
    SearchValues *values;
    SearchPoints *points;
    s32 *vals;
    SearchPoint *pts;
    s32 count;
    f32 distance;
    f32 minimum;
    sptr resource;          /* the index (a pointer) */
    s32 pool;
    void **handle;
    void *table;

    pool = gEngineApp.m_CartridgeIndex;
    CCache__LoadPersistantIndex(0, &resource, pool, sCSceneCacheDesc_CartridgeIndex);
    i = 0;
    best = 0;
    handle = CCache__LoadPersistantSubBlock_v(0, resource, 8, pool, world, 0, sCSceneCacheDesc_WarpDests);
    table = *handle;
    values = CISet__GetBlock(table, 0);
    count = values->count;
    vals = values->values;
    points = CISet__GetBlock(table, 1);
    minimum = 3.4028235e38f;
    pts = points->points;
    for (; i < count; i++) {
        if (pts[i].type == type) {
            distance = CVector3__Distance(&pts[i], position);
            if (distance < minimum) {
                minimum = distance;
                best = i;
            }
        }
    }
    if (best != 0) {
        *out = pts[best];
    }
    CCache__Deallocate(0, handle);
    if (best != 0) {
        return vals[best];
    }
    return 0;
}

s32 Sound__Play2D(s32);

void CScene__RequestWarp_2(CScene *arg0, s32 arg1, s32 arg2) {
    arg0->m_WarpState = 1;
    arg0->m_WarpFade = arg2;
    arg0->m_WarpID = arg1;
    if ((arg2 == 2) && (arg1 != 0x3E7)) {
        Sound__Play2D(0xBE0);
    }
}

void CScene__SetupLighting_2(CScene *menu) {
    CScene__LoadLightSettings_2(menu);
    gFogEnabled = (menu->m_pLevelHeader->m_Flags >> 2) & 1;
}

void CScene__RestoreFiredEvent(void *, void *);
struct State_func_8044D5C4_de;
struct State_func_8044D5C4_de {
    char p[0x1168];
    s32 unk1168;
    s32 unk116C;
    char q[8];
    s32 unk1178;
    s32 unk117C;
};
typedef struct State_func_8044D5C4_de State_func_8044D5C4_de;
void Cinema_Enable();
void Cinema_Disable();
#ifdef TARGET_PORT
/* The port (round 13 b2): the record tables by member of CScene (the N64 view reads them at
 * 0x1168-0x117C, other members of the host CScene, and keeps the table pointers in s32). */
void CScene__RestoreFiredEvents_2(CScene *w) {
    s32 i, n;
    Cinema_Disable();
    n = w->m_nRecords;
    for (i = 0; i < n; i++) CScene__RestoreFiredEvent(w, &w->m_pRecords[i]);
    n = w->m_nAltRecords;
    for (i = 0; i < n; i++) CScene__RestoreFiredEvent(w, &w->m_pAltRecords[i]);
    Cinema_Enable();
}
#else
#define arg0 ((State_func_8044D5C4_de *) scene)
/* the definition takes the callers' type (one prototype for the merged object); the Rage Wars view reads it */
void CScene__RestoreFiredEvents_2(CScene *scene) {
 s32 i,n;
 Cinema_Disable();
 n=arg0->unk1168;
 for(i=0;i<n;i++) CScene__RestoreFiredEvent(arg0,arg0->unk1178+i*20);
 n=arg0->unk116C;
 for(i=0;i<n;i++) CScene__RestoreFiredEvent(arg0,arg0->unk117C+i*20);
 Cinema_Enable();
}
#undef arg0
#endif

struct Item_func_8044D668_de;
typedef struct Item_func_8044D668_de Item_func_8044D668_de;
struct Item_func_8044D668_de {
    char pad0[0x11];
    u8 kind;
    char pad12[2];
};
struct Level;
struct Level {
    char pad0[0x1168];  /* record tables 0x58 earlier than in Rage Wars */
    s32 counts[2];
    char pad1170[8];
    Item_func_8044D668_de *items[2];
};
typedef struct Level Level;
extern void Cinema_Disable(void);
extern void Cinema_Enable(void);
#ifdef TARGET_PORT
/* The port (round 13 b2): the record tables by member of CScene (the N64 view reads them at
 * 0x1168-0x117C, other members of the host CScene, and keeps the table pointers in s32). */
void CScene__ReapplyFiredEvents_2(CScene *w) {
    s32 i, k, n;
    Item_func_8044D668_de *item;
    Cinema_Disable();
    for (k = 0; k < 2; k++) {
        n = k == 0 ? w->m_nRecords : w->m_nAltRecords;
        for (i = 0; i < n; i++) {
            item = (Item_func_8044D668_de *) &(k == 0 ? w->m_pRecords : w->m_pAltRecords)[i];
            if (item->kind == 1 || item->kind == 4 || item->kind == 2 || item->kind == 10) {
                CScene__RestoreFiredEvent(w, item);
            }
        }
    }
    Cinema_Enable();
}
#else
#define level ((Level *) scene)
/* the definition takes the callers' type (one prototype for the merged object); the Rage Wars view reads it */
void CScene__ReapplyFiredEvents_2(CScene *scene) {
    s32 i;
    s32 n;
    Item_func_8044D668_de *item;
    Cinema_Disable();
    n = level->counts[0];
    for (i = 0; i < n; i++) {
        item = &level->items[0][i];
        if (item->kind == 1 || item->kind == 4 || item->kind == 2 || item->kind == 10) {
            CScene__RestoreFiredEvent(level, item);
        }
    }
    n = level->counts[1];
    for (i = 0; i < n; i++) {
        item = &level->items[1][i];
        if (item->kind == 1 || item->kind == 4 || item->kind == 2 || item->kind == 10) {
            CScene__RestoreFiredEvent(level, item);
        }
    }
    Cinema_Enable();
}
#undef level
#endif
#ifdef TARGET_PORT
/* The port: the scene's delayed events typed (structs/world.h): fire them all and free them. */
#include "common.h"
#include "structs/world.h"

void LevelEvent_Fire(void *event);
void CScene__FlushDelayedEvents_2(CScene *scene) {
    CDelayedEvent *d = scene->m_DelayedEvents.m_pHead;
    CDelayedEvent *next;

    while (d != NULL) {
        next = d->m_pNext;
        LevelEvent_Fire((sptr)d->m_pEvent); /* full width (soak S12, --high-memory) */
        CList__Remove(&scene->m_DelayedEvents, d);
        CList__AddHead(&scene->m_FreeDelayedEvents, d);
        d = next;
    }
}
#else
struct Entry_func_8044D794_de;
typedef struct Entry_func_8044D794_de Entry_func_8044D794_de;
struct Owner_func_8044D794_de;
typedef struct Owner_func_8044D794_de Owner_func_8044D794_de;
struct Entry_func_8044D794_de;
struct Entry_func_8044D794_de {
    char pad0[4];
    struct Entry_func_8044D794_de *next;
    s32 handle;
};
struct Entry_func_8044D794_de;
struct Owner_func_8044D794_de;
struct Owner_func_8044D794_de {
    char pad0[0x6C];
    void *unk6C;
    char pad70[0xC];
    void *unk7C;
    char pad80[0x1100];
    struct Entry_func_8044D794_de *active;
    char pad1184[0x10]; /* active at 0x1180, spare at 0x1194 */
    struct Entry_func_8044D794_de *spare;
};
void LevelEvent_Fire(void *event);
#define arg0 ((Owner_func_8044D794_de *) scene)
/* the definition takes the port body's types (one prototype for the merged object); the view reads them */
void CScene__FlushDelayedEvents_2(CScene *scene) {
    Entry_func_8044D794_de *entry;
    Entry_func_8044D794_de *next;
    entry = arg0->active;
    while (entry != 0) {
        next = entry->next;
        LevelEvent_Fire(entry->handle);
        CList__Remove(&arg0->active, entry);
        CList__AddHead(&arg0->spare, entry);
        entry = next;
    }
}
#undef arg0
#endif

typedef struct LevelState {
    /* 0x00 */ u8 pad0[0x98];
    /* 0x98 */ void *handle98;
    /* 0x9C */ void *handle9C;
    /* 0xA0 */ void *handleA0;
    /* 0xA4 */ void *handleA4;
    /* 0xA8 */ void *handleA8;
    /* 0xAC */ void *handleAC;
    /* 0xB0 */ void *handleB0;
    /* 0xB4 */ void *handleB4;
    /* 0xB8 */ u8 padB8[0x10];
    /* 0xC8 */ void *handleC8;
    /* 0xCC */ void *handleCC;
} LevelState;

/* Tears the level down: releases its resources' handles. */
void CScene__UnloadLevel_2(LevelState *level) {
    void *p;

    p = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    if (p != NULL) {
        CPlayer__StopLoopingSounds_2(p);
    }
    LoopSounds_StopAll(LoopSounds_Get());
    Sound__DetachAll();
    Sound__StopFlags(0x1000);
    CCache__Deallocate(0, level->handle98);
    CCache__Deallocate(0, level->handle9C);
    CCache__Deallocate(0, level->handleA0);
    CCache__Deallocate(0, level->handleA4);
    CCache__Deallocate(0, level->handleA8);
    CCache__Deallocate(0, level->handleB0);
    CCache__Deallocate(0, level->handleB4);
    CCache__Deallocate(0, level->handleCC);
    CCache__Deallocate(0, level->handleAC);
    CCache__Deallocate(0, level->handleC8);
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8044D668_de.c, func_8044D804_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

s32 CScene__GetObject(struct CScene *, s32);
/* the N64 symbol by another C name: the merged object defines it DEFERRED (inline), and a call by its own
 * name after that definition would expand it (the CGameObjectInstance__LoadGeometry_call pattern) */
#ifdef TARGET_PORT
#define CScene__GetObject_call CScene__GetObject
#else
s32 CScene__GetObject_call(struct CScene *, s32) __asm__("CScene__GetObject");
#endif
#ifdef TARGET_PORT
void T2Port_ParticlesBuild(void *scene, void *data); /* port/game/t2particles.c */
#endif

void CScene__DecompressParticles_2(sptr arg0, void ***arg1) {
    void *obj;
    GroupCode8044E2B8 *group;
    Piece *piece;
    sptr base;
    s32 count;
    s32 i;
    if (ProPackDecompress(arg1, 0) != 0) {
        obj = **arg1;
#ifdef TARGET_PORT
        /* the host copies the templates with full-width pointers (port/game/t2particles.c);
         * block 2 keeps its offsets */
        T2Port_ParticlesBuild((void *) (sptr) arg0, obj);
        return;
#endif
        group = CISet__GetBlock(obj, 2);
        count = group->count;
        piece = group->pieces;
        i = 0;
        base = (sptr) CISet__GetBlock(obj, i);
        if (count > 0) {
            do {
                if (piece[i].tag == -1) {
                    piece[i].tag = 0;
                } else {
                    piece[i].tag = CScene__GetObject_call(arg0, piece[i].tag);
                }
                piece[i].unk18 = base + piece[i].unk18;
                piece[i].unk1C = base + piece[i].unk1C;
                piece[i].unk20 = base + piece[i].unk20;
                piece[i].unk24 = base + piece[i].unk24;
                piece[i].unk28 = base + piece[i].unk28;
                piece[i].unk2C = base + piece[i].unk2C;
                piece[i].unk30 = base + piece[i].unk30;
                piece[i].unk34 = base + piece[i].unk34;
                piece[i].unk38 = base + piece[i].unk38;
                i++;
            } while (i < count);
        }
    }
}

/* The second VM module's copy of CScene__DecompressIntelligence, from the same source file as
 * func_002261A8.c: the engine's CScene__FindNearestPathPoint and the VM's CScene__DecompressIntelligence.
 * Its code reads the first module's pool (versions/shared_rodata.us.txt):
 * CScene__FindNearestPathPoint's -1.0f at 0x800A6010, then this function's 1.0f and its jump
 * table at 0x800A6018, so the file's .rodata has to start with that -1.0f
 * for the table to land 8-aligned where the ROM has it. The engine half is
 * compiled into .text_engine, which no output section names: the linker
 * script's /DISCARD/ drops it. docs/VMASM.md. */

void CVector3__Subtract(CVector3 *out, CVector3 *a, u8 *b);

/* the section on the definition (the merged object drops a prototype of a function it defines) */
__attribute__((section(".text_engine"))) static s32 CScene__FindNearestPathPoint(CScene *lv, s32 unused, CVector3 *pt) {
    CVector3 d;
    f32 bestIdx = -1.0f;
    f32 bestD = 0.0f;
    s32 i = 0;
    s32 *t;
    s32 n;
    u8 *p;
    t = CISet__GetBlock(((void *(*)(s32 *)) CISet__GetBlock)(lv->unk_68), 1); /* the inner call passes one (a1 goes through) */
    n = t[1];
    p = (u8 *)(t + 2);
    if (n > 0) {
        do {
            f32 dd;
            CVector3__Subtract(&d, pt, p);
            dd = d.x * d.x + d.y * d.y + d.z * d.z;
            if (bestIdx < 0.0f || dd < bestD) {
                bestIdx = (f32)i;
                bestD = dd;
            }
            i++;
            p += 0x1C;
        } while (i < n);
    }
    return (s32)bestIdx;
}

typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad4[0x24];
    /* 0x28 */ f32 x;
    /* 0x2C */ f32 y;
    /* 0x30 */ f32 z30;
    /* 0x34 */ f32 z34;
    /* 0x38 */ u8 pad38[0x14];
    /* 0x4C */ f32 x2;
    /* 0x50 */ f32 y2;
    /* 0x54 */ f32 z2;
} Element;
#ifdef TARGET_PORT
int T2Port_LegacyIntel9(void); /* port/game/t2debug.c */
#endif

void CScene__DecompressIntelligence_2(s32 unused, void ***resource) {
    f32 scale = 1.0f;
    s32 *table;
    s32 count;
    s32 i;
    Element *e;

    if (ProPackDecompress(resource, 0)) {
        table = (s32 *) **resource;
        count = *table;
        for (i = 0; i < count; i++) {
            e = CISet__GetBlock(table, i);
            switch (e->type) {
                case 1:
                    e->x *= scale;
                    e->z30 *= scale;
                    e->z34 *= scale;
                    e->x2 *= scale;
                    e->y2 *= scale;
                    e->z2 *= scale;
                    break;
                case 11:
                    e->x *= scale;
                    e->y *= scale;
                    break;
                case 0:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                    break;
            }
#ifdef TARGET_PORT
            /* The port (round 17 f3, tools/port/n64cmp.py): the archive's swap plan has no struct for
             * the type-9 (interactive anim) body (tools/assets/swap_spec.json, "no struct yet"), so its
             * three words (the type flags, the start animation id, 0) stayed big-endian: on a
             * little-endian host InteractiveAnim_Advance took 0xFD530000 for animation 0x53FD and the
             * flag tests (m_dwTypeFlags & 1, ...) read the wrong bits. Swapped here, once per load.
             * T2_LEGACY_INTEL9=1 (DEBUG-ONLY) keeps the old bytes. */
            if (e->type == 9 && !T2Port_LegacyIntel9()) {
                u32 *w = (u32 *) ((u8 *) e + 0x14);
                w[0] = __builtin_bswap32(w[0]);
                w[1] = __builtin_bswap32(w[1]);
                w[2] = __builtin_bswap32(w[2]);
            }
#endif
        }
    }
}

typedef struct Entry {
    u8 data[0x20];
} Entry;

/* Resolves the references of every item of every group of the loaded resource and keeps it in the
 * level (CScene.unk_68; the references point into block 2 of m_pRegionData). Typed on CScene: the
 * m2c view put them at their N64 offsets 0x58/0x68, which on the host overwrote m_pCollision.
 * The item's ref stays an s32 (the ROM record); on the host it stays the region index
 * (CScene__GetPathRegion below). */
void CScene__DecompressPaths_2(CScene *level, s32 ***handle) {
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

extern s32 CScene__FindObjectType(struct CScene *, s32);
/* the N64 symbol by another C name: the merged object defines it DEFERRED (inline), and a call by its own
 * name after that definition would expand it (the CGameObjectInstance__LoadGeometry_call pattern) */
#ifdef TARGET_PORT
#define CScene__FindObjectType_call CScene__FindObjectType
#else
s32 CScene__FindObjectType_call(void *level, s32 key) __asm__("CScene__FindObjectType");
#endif

extern s32 CScene__GetObject(struct CScene *, s32);

/* Looks key up in the level and loads its record into out (out->valid = 0 when there is none). */
void CScene__ConstructMap_2(void *level, CSceneMapRecord *out, s32 key) {
    s32 id;
    s32 **h1;
    s32 **h2;
    s32 size;
    s32 res;

    id = CScene__FindObjectType_call(level, key);
    if (id == -1) {
        out->valid = 0;
        return;
    }
    res = CScene__GetObject_call(level, id);
    h1 = CCache__Request(0, res, 0x18, 0, 0, 0, sCSceneCacheDesc_ObjectIndex, 1);
    if (h1 != NULL) {
        h2 = CCache__Request(0, CISet__GetROMAddressAndSize(*h1, res, 1, &size), size, 0, 0, Model_DecompressCallback, sCSceneCacheDesc_Geometry, 1);
        if (h2 != NULL) {
            *out = *(CSceneMapRecord *)CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*h2, 2), 0), 0), 0);
            CCache__Deallocate(0, h2);
        }
        CCache__Deallocate(0, h1);
    }
}
