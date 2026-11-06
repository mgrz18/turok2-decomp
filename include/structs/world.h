#ifndef STRUCTS_WORLD_H
#define STRUCTS_WORLD_H

/* The world (gScene): the game's resource files, the loaded level and
 * its actor lists, the particle and actor pools, and the level-warp state.
 * One object of 0x19078 bytes and more (gEngineApp_m_SwooshSystem, world + 0x19240, is
 * passed on as an object of its own). Other symbols point inside it and must
 * keep being used where the code uses them: gScene_m_CinemasKey (+0x24), D_800F70A8
 * (+0x30), gScene_m_pCinemas (+0x60), gScene_m_pBitSections (+0x6C), gScene_m_Heap (+0xD0),
 * gScene_m_pActors (+0xE0), gScene_m_nDrawActors (+0x8EC).
 *
 * CScene__Construct opens the game's resource files (the *Key fields are file
 * ids, the m_p* fields their loaded tables: a count, then offsets, read with
 * CISet__GetBlock), CScene__LoadLevel loads a level into it (the m_h* fields are
 * the resource handles of the level's blocks), CScene__UpdateWarp runs the warp
 * fade. Rage Wars has the same object (Shared_World in its COMMON/WORLD.H)
 * with another layout: its actor lists sit 0x58 later and its level state
 * at 0x1B40C instead of 0x18FA8.
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "structs/game_object.h"
#include "structs/region.h"
#include "structs/table.h"
#include "structs/level_actor.h"
#include "structs/particle.h"

/* Where a warp puts the player: a spawn point of a level. */
typedef struct CWarpPoint {
    /* 0x00 */ CVector3 m_vPos;
    /* 0x0C */ f32 m_RotY;
    /* 0x10 */ s16 m_nLevel;
    /* 0x12 */ s16 m_nID;                         /* the spawn point's id */
} CWarpPoint; /* size 0x14 */

/* A streamed sector of the level: its data block and its extent on the ground plane. */
typedef struct CSceneSector {
    /* 0x00 */ void **m_hData;
    /* 0x04 */ void *m_pBounds;
} CSceneSector; /* size 0x8 */

/* A record of the level's object tables (two of them, by key). */
typedef struct CSceneRecord {
    /* 0x00 */ u8 unk_00[0xE];
    /* 0x0E */ u8 m_Flags;
    /* 0x0F */ u8 m_Key;
    /* 0x10 */ u8 unk_10[0x4];
} CSceneRecord; /* size 0x14 */

/* The head of the level file (block 0): its scale and its lighting. */
typedef struct CLevelHeader {
    /* 0x00 */ f32 m_Scale;
    /* 0x04 */ u8 unk_04[0x6];
    /* 0x0A */ u8 m_LightColor[3];
    /* 0x0D */ u8 m_AmbientColor[3];
    /* 0x10 */ u8 m_Flags;
    /* 0x11 */ s8 m_LightDir[3];
} CLevelHeader; /* size 0x14 */

typedef struct CGeomHeader {
    /* 0x00 */ s32 m_Data[7];
} CGeomHeader; /* size 0x1C */

/* Turok 3's CScene, the level (gScene). Called CWorld here until round N3;
 * CSceneSector and CSceneRecord were CWorldSector and CWorldRecord. */
typedef struct CScene {
    /* 0x00000 */ s32 m_Mode;                       /* 2, 4: multiplayer modes */
    /* 0x00004 */ s32 unk_04;
    /* 0x00008 */ s32 unk_08;
    /* 0x0000C */ s32 m_LevelsKey;
    /* 0x00010 */ s32 m_TextureSetsKey;
    /* 0x00014 */ s32 m_ObjectsKey;
    /* 0x00018 */ s32 m_BinariesKey;
    /* 0x0001C */ s32 m_GraphicSetsKey;
    /* 0x00020 */ s32 m_LevelInfoKey;
    /* 0x00024 */ s32 m_CinemasKey;                 /* gScene_m_CinemasKey */
    /* 0x00028 */ s32 unk_28;
    /* 0x0002C */ s32 unk_2C;
    /* 0x00030 */ s32 unk_30;                       /* D_800F70A8: the count of unk_28 */
    /* 0x00034 */ s32 unk_34;                       /* the count of unk_2C */
    /* 0x00038 */ s32 *m_pLevels;                   /* the level table: count first */
    /* 0x0003C */ s32 *m_pTextureSets;
    /* 0x00040 */ s32 *m_pObjects;
    /* 0x00044 */ s32 *m_pBinaries;
    /* 0x00048 */ s32 *m_pGraphicSets;
    /* 0x0004C */ s32 *m_pLevelFile;
    /* 0x00050 */ s32 *m_pLevelInfo;
    /* 0x00054 */ s32 *m_pCollision;
    /* 0x00058 */ s32 *m_pRegionData;               /* region infos, regions, floor grid (structs/region.h) */
    /* 0x0005C */ s32 *m_pSwooshes;
    /* 0x00060 */ s32 *m_pCinemas;                  /* gScene_m_pCinemas */
    /* 0x00064 */ s32 *m_pIntelligence;
    /* 0x00068 */ s32 *unk_68;
    /* 0x0006C */ void *m_pBitSections;             /* gScene_m_pBitSections: per-level bits (gLoadSavePersistentData) */
    /* 0x00070 */ void *m_pBitSections2;            /* (gGamePersistentData) */
    /* 0x00074 */ IdTable *m_pObjectTypes;
    /* 0x00078 */ s32 *m_pObjectTypes2;
    /* 0x0007C */ s32 *m_pGraphicSetTypes;
    /* 0x00080 */ s32 *m_pLights;
    /* 0x00084 */ s32 *m_pCurves;
    /* 0x00088 */ s32 *m_pRumbleTypes;
    /* 0x0008C */ s32 *m_pRumbles;
    /* 0x00090 */ s32 *m_pLinkGroupTypes;
    /* 0x00094 */ s32 *m_pParticleEffects;
    /* 0x00098 */ void **m_hLevelIndex;
    /* 0x0009C */ void **m_hRegionData;
    /* 0x000A0 */ void **m_hCollision;
    /* 0x000A4 */ void **m_hLevelInfo;
    /* 0x000A8 */ void **m_hLevelHeader;
    /* 0x000AC */ void **m_hSections;
    /* 0x000B0 */ void **m_hActors;
    /* 0x000B4 */ void **m_hLinks;
    /* 0x000B8 */ u8 unk_B8[0x10];
    /* 0x000C8 */ void **m_hHeap;
    /* 0x000CC */ void **m_hPaths;
    /* 0x000D0 */ u8 m_Heap[0x10];                  /* gScene_m_Heap */
    /* 0x000E0 */ struct CGameObjectInstance *m_pActors; /* gScene_m_pActors: the level's actors */
    /* 0x000E4 */ s32 m_nFirstActor;
    /* 0x000E8 */ s32 m_nActors;
    /* 0x000EC */ struct CGameObjectInstance *m_pDrawActors[0x200];
    /* 0x008EC */ s32 m_nDrawActors;                /* gScene_m_nDrawActors */
    /* 0x008F0 */ struct CGameObjectInstance *m_pMovers[0x80];
    /* 0x00AF0 */ s32 m_nMovers;
    /* 0x00AF4 */ struct CGameObjectInstance *m_pListed[0x40];
    /* 0x00BF4 */ s32 m_nListed;
    /* 0x00BF8 */ struct CGameObjectInstance *m_pRegistered[0x80];
    /* 0x00DF8 */ s32 m_nRegistered;
    /* 0x00DFC */ struct CGameObjectInstance *m_pKind1[0x40]; /* definition kind 1 */
    /* 0x00EFC */ s32 m_nKind1;
    /* 0x00F00 */ s32 m_Kind1Cursor;                /* round-robin position in m_pKind1 */
    /* 0x00F04 */ struct CGameObjectInstance *m_pFlagged[0x20]; /* flags & 2 */
    /* 0x00F84 */ s32 m_nFlagged;
    /* 0x00F88 */ struct CGameObjectInstance *m_pKind5[0x10]; /* definition kind 5 */
    /* 0x00FC8 */ s32 m_nKind5;
    /* 0x00FCC */ struct CGameObjectInstance *m_pList6[0x20];
    /* 0x0104C */ s32 m_nList6;
    /* 0x01050 */ struct CGameObjectInstance *m_pList7[4];
    /* 0x01060 */ s32 m_nList7;
    /* 0x01064 */ s32 m_nDeferred;
    /* 0x01068 */ void *m_pDeferred[0x40];          /* flag 4 actors */
    /* 0x01168 */ s32 m_nRecords;
    /* 0x0116C */ s32 m_nAltRecords;
    /* 0x01170 */ s32 *m_pRecordIndex;
    /* 0x01174 */ s32 *m_pAltRecordIndex;
    /* 0x01178 */ CSceneRecord *m_pRecords;
    /* 0x0117C */ CSceneRecord *m_pAltRecords;
    /* 0x01180 */ u8 unk_1180[0x14A8 - 0x1180];
    /* 0x014A8 */ s32 m_nSectors;
    /* 0x014AC */ CSceneSector m_Sectors[128];
    /* 0x018AC */ u8 unk_18AC[0x4];
    /* 0x018B0 */ CParticleSystem m_ParticleSystem;
    /* 0x10518 */ CLevelActorPool m_ActorPool;
    /* 0x13B24 */ u8 unk_13B24[0x4];
    /* 0x13B28 */ CInstancePool m_InstancePool;     /* gScene_m_InstancePool (structs/game_object.h) */
    /* 0x18E50 */ CLevelHeader *m_pLevelHeader;
    /* 0x18E54 */ u8 unk_18E54[0x8];
    /* 0x18E5C */ u8 unk_18E5C[0x40];
    /* 0x18E9C */ u8 *m_pSections;
    /* 0x18EA0 */ s32 m_nSections;
    /* 0x18EA4 */ f32 m_SkyAngle;
    /* 0x18EA8 */ CQuaternion m_qSky;               /* about Y */
    /* 0x18EB8 */ s32 unk_18EB8;
    /* 0x18EBC */ u8 unk_18EBC[0xE8];
    /* 0x18FA4 */ s32 m_bStreamAll;
    /* 0x18FA8 */ s32 m_nLevel;                     /* index in m_pLevels, -1 for none */
    /* 0x18FAC */ s32 m_WarpFade;                   /* index in the fade table gWarpFadeColors */
    /* 0x18FB0 */ s32 m_WarpState;                  /* 0 none, 1 fading out to the warp, 2 fading back in */
    /* 0x18FB4 */ f32 m_WarpTime;
    /* 0x18FB8 */ s32 m_bWarpSpawn;                 /* m_WarpSpawn was found (CScene__FindWarpDests) */
    /* 0x18FBC */ CWarpPoint m_WarpSpawn;
    /* 0x18FD0 */ s32 m_LevelID;                    /* the warp id of the loaded level */
    /* 0x18FD4 */ s32 m_WarpID;                     /* where the pending warp goes (~id: a level index) */
    /* 0x18FD8 */ s32 unk_18FD8;
    /* 0x18FDC */ s32 m_PendingWarpID; /* warp id kept for later: CPlayer__Update stores it, PakMessage_SelectOk passes it to CScene__RequestWarp */
    /* 0x18FE0 */ s32 m_PreviewState;
    /* 0x18FE4 */ s32 m_PreviewTimer;
    /* 0x18FE8 */ s32 m_PreviewOptions;             /* the option flags last announced */
    /* 0x18FEC */ CGeomHeader m_Geom[5];
} CScene; /* size 0x19078 */

#endif
