#ifndef GLOBALS_STRINGS_H
#define GLOBALS_STRINGS_H

/* The engine's strings read by address: CCache__Request descriptions, libultra/libaudio
 * assert(EX) literals and file names, the crash screen's texts, pickup names and messages,
 * kill and cheat messages (KSEG0 .rodata/.data).
 * Named in round 21 (versions/names/data_j2.us.txt: the words of the string, the
 * functions that read it). Declared `char []`; a file that needs another view keeps a
 * local declaration with a `local type` comment (docs/GLOBALS.md). */

extern char sRenderCacheDesc_SimpObjectIndex[];          /* "simp object index" */
extern char sRenderCacheDesc_Geometry[];                 /* "geometry" */
extern char sCStaticObjectCacheDesc_LitVertices[];       /* "lit vertices" */
extern char sCStaticObjectCacheDesc_StatObjectIndex[];   /* "stat object index" */
extern char sCStaticObjectCacheDesc_StatGeometry[];      /* "stat geometry" */
extern char sCGameObjectInstanceCacheDesc_AnimObjectInfo[]; /* "anim object info" */
extern char sCSceneCacheDesc_ObjectIndex[];              /* "object index" */
extern char sCSceneCacheDesc_Geometry[];                 /* "geometry" */
extern char sCSceneCacheDesc_CartridgeIndex[];           /* "cartridge index" */
extern char sCSceneCacheDesc_LevelIndex[];               /* "level index" */
extern char sCSceneCacheDesc_GridSection[];              /* "grid section" */
extern char sCSceneCacheDesc_WarpDests[];                /* "warp dests" */
extern char sCSceneCacheDesc_LevelName[];                /* "level name" */
extern char sCInstancePoolCacheDesc_AnimObjectIndex[];   /* "anim object index" */
extern char sCInstancePoolCacheDesc_AnimObjectInfo[];    /* "anim object info" */
extern char sCPlayerPoolCacheDesc_Players[];             /* "Players" */
extern char sActorCacheDesc_PlayerAnimObjectInfo[];      /* "player anim object info" */
extern char sCCameraPoolCacheDesc_Cameras[];             /* "Cameras" */
extern char sCCameraPoolCacheDesc_CameraTexts[];         /* "Camera Texts" */
extern char sCrashText_ColonSpace[];                     /* ": " */
extern char sCrashText_Spaces3[];                        /* "   " */
extern char sCSeqPlayerAssertExpr[];                     /* "EX" */
extern char sSeqPlayerAssertExpr[];                      /* "EX" */
extern char sSeqPlayerAssertFile[];                      /* "audio/seqplayer.c" */
extern char sEnvAssertExpr[];                            /* "EX" */
extern char sEnvAssertFile[];                            /* "audio/env.c" */
extern char sSaveAssertExpr[];                           /* "EX" */
extern char sSaveAssertFile[];                           /* "audio/save.c" */
extern char sSiRawDmaAssertExpr[];                       /* "EX" */
extern char sSiRawDmaAssertFile[];                       /* "io/sirawdma.c" */
extern char sSpRawDmaAssertExpr[];                       /* "EX" */
extern char sSpRawDmaAssertFile[];                       /* "io/sprawdma.c" */
extern char sPiRawReadAssertExpr[];                      /* "EX" */
extern char sPiRawReadAssertFile[];                      /* "io/pirawread.c" */
extern char sSiRawReadAssertExpr[];                      /* "EX" */
extern char sSiRawReadAssertFile[];                      /* "io/sirawread.c" */
extern char sPickupNameText_SatchelCharge[];             /* "satchel charge" */
extern char sPickupNameText_PowerCell[];                 /* "power cell" */
extern char sPickupNameText_Key[];                       /* "key" */
extern char sPickupNameText_CaveDoorKey[];               /* "cave door key" */
extern char sPickupNameText_IonCapacitor[];              /* "ion capacitor" */
extern char sPickupNameText_RedLaserCell[];              /* "red laser cell" */
extern char sPickupNameText_BlueLaserCell[];             /* "blue laser cell" */
extern char sPickupNameText_GateKey[];                   /* "gate key" */
extern char sPickupNameText_GraveyardKey[];              /* "graveyard key" */
extern char sPickupNameText_Level1Key[];                 /* "level 1 key" */
extern char sPickupNameText_Level2Key[];                 /* "level 2 key" */
extern char sPickupNameText_Level3Key[];                 /* "level 3 key" */
extern char sPickupNameText_Level4Key[];                 /* "level 4 key" */
extern char sPickupNameText_Level5Key[];                 /* "level 5 key" */
extern char sPickupNameText_Level6Key[];                 /* "level 6 key" */
extern char sPickupNameText_PrimagenKey1[];              /* "primagen key 1" */
extern char sPickupNameText_PrimagenKey2[];              /* "primagen key 2" */
extern char sPickupNameText_PrimagenKey3[];              /* "primagen key 3" */
extern char sPickupNameText_PrimagenKey4[];              /* "primagen key 4" */
extern char sPickupNameText_PrimagenKey5[];              /* "primagen key 5" */
extern char sPickupNameText_PrimagenKey6[];              /* "primagen key 6" */
extern char sPickupMessageText_10PistolBulletsClip[];    /* "10 pistol bullets (clip)" */
extern char sKillMessageText_HeadShot[];                 /* "head shot!" */
extern char sKillMessageText_TaggedBy[];                 /* "tagged by " */
extern char sKillMessageText_YouTagged[];                /* "you tagged " */
extern char sKillMessageText_KilledBy[];                 /* "killed by " */
extern char sKillMessageText_YouKilled[];                /* "you killed " */
extern char sKillMessageText_YouKilledYourself[];        /* "you killed yourself" */
extern char sCheatMessageText_NewCheatEnabled[];         /* "new cheat enabled" */
extern char sTotemDefenseText_MissionComplete[];         /* "mission complete." */
extern char sTotemDefenseText_MissionFailed[];           /* "mission failed." */
extern char sTotemDefenseText_DefendTheTotem[];          /* "defend the totem" */
extern char sPlayerHurtText_LowHealth[];                 /* "low health" */
extern char sCCacheDesc_Header[];                        /* "header" */
extern char sTextureCacheDesc_Texture[];                 /* "texture" */
extern char sCrashText_Newline[];                        /* "\n" */
extern char sCCacheDesc_Header[];                        /* "header" */
extern char sTextureCacheDesc_Texture[];                 /* "texture" */
extern char sCrashText_Newline[];                        /* "\n" */
extern char sPakCompanyCode[];                           /* "51": Pak_WriteNote company code */

#endif
