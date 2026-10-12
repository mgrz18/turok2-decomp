#ifndef STRUCTS_PLAYER_H
#define STRUCTS_PLAYER_H

/* The player and the HUD. Layouts and names from LibTEngine's Turok 2 types
 * (Drahsid/LibTEngine, lib/libtengine/lib/types: CPlayer.h, COverlay.h,
 * COnScreen.h...), with these departures:
 *   - CBarrel (the weapon wheel) is kept opaque: LibTEngine's offsets jump
 *     from 0x00C to 0x100 and its 36-entry list does not fit the space, while
 *     its size, 0x394, is what CPlayer leaves for it;
 *   - LibTEngine skips 0xA2C in CPlayer; it is unk_0xA2C here;
 *   - CAmmo and CWeapon are 2 and 1 bytes there, as the gaps they fill say.
 * tools/check_structs.py checks every /\* 0xNN *\/ offset below with the game's
 * own compiler. Fields named unk_ are not yet known. */

#include "structs/game_object.h"

typedef struct { u8 m[0x40]; } Mtx;     /* libultra gbi.h, 4x4 fixed point */

/* A HUD element: health, ammo, lives, air... slide in, fade, show a value. */
typedef struct {
    /* 0x00 */ s32 m_bOnScreen;
    /* 0x04 */ s32 *m_pScript;
    /* 0x08 */ s32 m_Mode;
    /* 0x0C */ s32 m_Time;
    /* 0x10 */ f32 m_PositionX;
    /* 0x14 */ f32 m_PositionY;
    /* 0x18 */ f32 m_FinalX;
    /* 0x1C */ f32 m_FinalY;
    /* 0x20 */ f32 m_VelX;
    /* 0x24 */ f32 m_VelY;
    /* 0x28 */ f32 m_AccelX;
    /* 0x2C */ f32 m_AccelY;
    /* 0x30 */ f32 m_ZoomFactor;
    /* 0x34 */ f32 m_Alpha;
    /* 0x38 */ s32 m_Value;
} COverlay; /* size 0x3C */

/* On-screen text: boss bars and the fonts. */
typedef struct {
    /* 0x000 */ COverlay m_BossOverlay;
    /* 0x03C */ char *m_pBossName;
    /* 0x040 */ struct CGameObjectInstance *m_pBoss;
    /* 0x044 */ s32 m_BossPercent;
    /* 0x048 */ COverlay m_BossOverlay2;
    /* 0x084 */ char *m_pBossName2;
    /* 0x088 */ struct CGameObjectInstance *m_pBoss2;
    /* 0x08C */ s32 m_BossPercent2;
    /* 0x090 */ struct CMemEntry *m_pmeTinyFont;
    /* 0x094 */ struct CMemEntry *m_pmeSmallFont;
    /* 0x098 */ struct CMemEntry *m_pmeNormalFont;
    /* 0x09C */ struct CMemEntry *m_pmeKanjiFont;
    /* 0x0A0 */ s32 m_TopR;
    /* 0x0A4 */ s32 m_TopG;
    /* 0x0A8 */ s32 m_TopB;
    /* 0x0AC */ s32 m_BottomR;
    /* 0x0B0 */ s32 m_BottomG;
    /* 0x0B4 */ s32 m_BottomB;
    /* 0x0B8 */ f32 m_FontXScale;
    /* 0x0BC */ f32 m_FontYScale;
    /* 0x0C0 */ s32 m_ShadowXOff;
    /* 0x0C4 */ s32 m_ShadowYOff;
    /* 0x0C8 */ u8 m_CharacterSmallLookUpPos[256];
} COnScreen; /* 0x1C8 known; LibTEngine is unsure whether 0x2C8 */

/* The 3D weapon model drawn in the player's hand. */
typedef struct {
    /* 0x00 */ s32 m_ObjectType;
    /* 0x04 */ s32 m_DesiredObjectType;
    /* 0x08 */ void *m_rpObject;
    /* 0x0C */ Mtx m_mOrient[2];
    /* 0x8C */ CVector3 m_vScale;
    /* 0x98 */ CVector3 m_vRot;
    /* 0xA4 */ CVector3 m_vPos;
    /* 0xB0 */ s32 m_Mode;
    /* 0xB4 */ f32 m_ModeTime;
} CPlayerOverlay; /* size 0xB8 */

typedef struct {
    /* 0x00 */ f32 m_LookX;
    /* 0x04 */ f32 m_LookY;
    /* 0x08 */ f32 m_MoveX;
    /* 0x0C */ f32 m_MoveY;
    /* 0x10 */ struct CContState *m_pContState;
    /* 0x14 */ f32 m_FinalLookX;
    /* 0x18 */ f32 m_FinalLookY;
    /* 0x1C */ f32 m_FinalMoveX;
    /* 0x20 */ f32 m_FinalMoveY;
    /* 0x24 */ u32 m_dwButtons;
    /* 0x28 */ u32 m_dwButtonsSingle;
    /* 0x2C */ u32 m_dwButtonsSingleNoHold;
    /* 0x30 */ u32 m_dwButtonsDouble;
    /* 0x34 */ u32 m_dwButtonsRepeat;
} CPlayerContState; /* size 0x38 */

/* What a save game keeps of the player. Health is 24.8 fixed point. */
typedef struct {
    /* 0x000 */ s32 m_nDeathmatchCharacter;
    /* 0x004 */ s32 m_Health;
    /* 0x008 */ s16 m_LifeForce;
    /* 0x00A */ s16 m_Lives;
    /* 0x00C */ u8 m_SpecialPickupCount[1024];
    /* 0x40C */ s32 m_CheckPointWarpID;
    /* 0x410 */ s32 m_SavePointWarpID;
    /* 0x414 */ s16 m_AmmoList[31];
    /* 0x452 */ u8 m_WeaponList[35];
    /* 0x475 */ u8 _pad0;
    /* 0x476 */ s16 m_CurrentWeapon;
    /* 0x478 */ s16 m_LastWeapon[3];
    /* 0x47E */ u8 _pad1[2];
    /* 0x480 */ s32 m_AdonAmmoFlagHelped[6];
    /* 0x498 */ s32 m_AdonHealthFlagHelped[6];
    /* 0x4B0 */ s32 m_bLevel1TotemFlags;
    /* 0x4B4 */ s32 m_bLevel2TotemFlags;
    /* 0x4B8 */ s32 m_bLevel3TotemFlags;
    /* 0x4BC */ s32 m_bLevel4TotemFlags;
    /* 0x4C0 */ s32 m_bLevel5TotemFlags;
    /* 0x4C4 */ s32 m_bBlindBossComplete;
    /* 0x4C8 */ s32 m_bQueenBossComplete;
    /* 0x4CC */ s32 m_bMotherBossComplete;
    /* 0x4D0 */ s32 m_bPrimagenBossComplete;
    /* 0x4D4 */ u32 m_LevelIntroPlayedFlags;
    /* 0x4D8 */ u32 m_dwLevelFirstTimeFlags;
    /* 0x4DC */ u32 m_dwLevelMissionObjFlags;
    /* 0x4E0 */ u8 m_NukePartLevelFlags[6];
    /* 0x4E6 */ u8 m_KeySet6LevelFlags[6];
} CPlayerLoadSaveData; /* size 0x4EC */

typedef struct CPlayer {
    /* 0x0000 */ CGameObjectInstance m_CharacterActor;
    /* 0x0288 */ CGameObjectInstance m_WeaponActor;
    /* 0x0510 */ s32 m_bActive;
    /* 0x0514 */ s32 m_nController;
    /* 0x0518 */ struct CPlayerOptions *m_pOptions;
    /* 0x051C */ struct CCamera *m_pCamera;
    /* 0x0520 */ CPlayerLoadSaveData m_Data;
    /* 0x0A0C */ s16 m_Mode;
    /* 0x0A0E */ s16 m_ModeBefore;
    /* 0x0A10 */ s16 m_ModeLastFrame;
    /* 0x0A12 */ s16 _pad0;
    /* 0x0A14 */ f32 m_ModeTime;
    /* 0x0A18 */ u32 m_ModeLastFrameFlags;
    /* 0x0A1C */ u32 m_ModeBeforeFlags;
    /* 0x0A20 */ u32 m_ModeFlags;
    /* 0x0A24 */ s32 m_InSun;
    /* 0x0A28 */ f32 m_FrameIncrementScaled;
    /* 0x0A2C */ s32 unk_0xA2C;
    /* 0x0A30 */ f32 m_InvincibilityTime;
    /* 0x0A34 */ f32 m_PlaneWalkerTime;
    /* 0x0A38 */ f32 m_FlashlightTime;
    /* 0x0A3C */ struct CDLight *m_pDLFlashlight1;
    /* 0x0A40 */ struct CDLight *m_pDLFlashlight2;
    /* 0x0A44 */ struct CDLight *m_pDLDarkCityLight;
    /* 0x0A48 */ CPlayerContState m_ContState;
    /* 0x0A80 */ f32 m_Speed;
    /* 0x0A84 */ f32 m_SideSpeed;
    /* 0x0A88 */ f32 m_TotalSpeed;
    /* 0x0A8C */ f32 m_YVelLastFrame;
    /* 0x0A90 */ s32 m_bOnGround;
    /* 0x0A94 */ f32 m_BurstSpeed;
    /* 0x0A98 */ f32 m_BurstSideSpeed;
    /* 0x0A9C */ f32 m_BurstTimer;
    /* 0x0AA0 */ f32 m_HandSide;
    /* 0x0AA4 */ f32 m_TimeOffGround;
    /* 0x0AA8 */ CVector3 m_vDesiredPos;
    /* 0x0AB4 */ f32 m_FleeTime;
    /* 0x0AB8 */ f32 m_JumpAngle;
    /* 0x0ABC */ f32 m_StartJumpAngle;
    /* 0x0AC0 */ f32 m_DesiredJumpAngle;
    /* 0x0AC4 */ f32 m_JumpU;
    /* 0x0AC8 */ f32 m_DuckOffset;
    /* 0x0ACC */ s32 m_bDuckMode;
    /* 0x0AD0 */ CVector3 m_vHeadRotOffset;
    /* 0x0ADC */ CVector3 m_vHeadWobbleRotOffset;
    /* 0x0AE8 */ CVector3 m_vEyeOffset;
    /* 0x0AF4 */ CQuaternion m_qGround;
    /* 0x0B04 */ f32 m_BreathAmp;
    /* 0x0B08 */ f32 m_BreathSpeed;
    /* 0x0B0C */ CQuaternion PreviousCameraQuatern;
    /* 0x0B1C */ s16 m_RequestedWeapon;
    /* 0x0B1E */ s16 _pad1;
    /* 0x0B20 */ CVector3 m_vWeaponFirePos;
    /* 0x0B2C */ f32 m_ActualCameraVertEyeOffset;
    /* 0x0B30 */ f32 m_CameraMovementScaler;
    /* 0x0B34 */ s32 m_CrossHairMode;
    /* 0x0B38 */ f32 m_CrossHairTimer;
    /* 0x0B3C */ f32 m_CrossHairAlpha;
    /* 0x0B40 */ struct CGameObjectInstance *m_pCrossHairTarget;
    /* 0x0B44 */ s32 m_CrossHairIcon;
    /* 0x0B48 */ CVector3 m_vCrossHairPos;
    /* 0x0B54 */ CVector3 m_vCrossHairVel;
    /* 0x0B60 */ struct CGameObjectInstance *m_pSightTarget;
    /* 0x0B64 */ s32 m_SightIcon;
    /* 0x0B68 */ f32 m_SightIconFrame;
    /* 0x0B6C */ CVector3 m_vSightPos;
    /* 0x0B78 */ CVector3 m_vSightVel;
    /* 0x0B84 */ CQuaternion m_qAutoAim;
    /* 0x0B94 */ s32 m_SniperMode;
    /* 0x0B98 */ f32 m_ZoomBlend;
    /* 0x0B9C */ f32 m_TargetZoomBlend;
    /* 0x0BA0 */ f32 m_JumpPadCurrentTime;
    /* 0x0BA4 */ f32 m_JumpPadAngleDirection;
    /* 0x0BA8 */ struct CROMRegionSet *m_pJumpPadRegionSet;
    /* 0x0BAC */ CVector3 m_vJumpPadStart;
    /* 0x0BB8 */ struct CGameObjectInstance *m_pOnEnemy;
    /* 0x0BBC */ s32 m_OnEnemyObjectType;
    /* 0x0BC0 */ CVector3 m_vOnEnemyScale;
    /* 0x0BCC */ CVector3 m_vOnEnemyOffset;
    /* 0x0BD8 */ CVector3 m_vOnEnemyOriginalScale;
    /* 0x0BE4 */ f32 m_RideEnemyTime;
    /* 0x0BE8 */ f32 m_OnEnemyStompTime;
    /* 0x0BEC */ f32 m_Oxygen;
    /* 0x0BF0 */ f32 m_OutOfAirSfxTimer;
    /* 0x0BF4 */ s32 m_bOxygenOut;
    /* 0x0BF8 */ s32 m_DoDeathCin;
    /* 0x0BFC */ s32 m_nDeathmatchTeamSelected;
    /* 0x0C00 */ s32 m_bEnteringDeathmatch;
    /* 0x0C04 */ f32 m_WaitDeathmatchTimer;
    /* 0x0C08 */ s32 m_bLevelNotCompleteExit;
    /* 0x0C0C */ s32 m_bLevelStarting;
    /* 0x0C10 */ s32 m_CharAnim;
    /* 0x0C14 */ s32 m_LastCharAnim;
    /* 0x0C18 */ CPlayerOverlay m_Overlay;
    /* 0x0CD0 */ u8 m_Barrel[0x394];           /* CBarrel, the weapon wheel */
    /* 0x1064 */ COverlay m_AmmoOverlay;
    /* 0x10A0 */ COverlay m_HealthOverlay;
    /* 0x10DC */ COverlay m_LivesOverlay;
    /* 0x1118 */ COverlay m_TokensOverlay;
    /* 0x1154 */ COverlay m_WeaponOverlay;
    /* 0x1190 */ COverlay m_AirOverlay;
    /* 0x11CC */ COverlay m_BossOverlay;
    /* 0x1208 */ COverlay m_TotalKillsOverlay;
    /* 0x1244 */ COverlay m_TotalTagsOverlay;
    /* 0x1280 */ struct CGameRumble *m_pRumble;
    /* 0x1284 */ s32 m_bHeadMovementDisabled;
    /* 0x1288 */ struct CLoopingSoundData *m_pLoopingSound_WeaponBackground;
    /* 0x128C */ struct CLoopingSoundData *m_pLoopingSound_WeaponFiring;
    /* 0x1290 */ f32 m_SfxTimer;
    /* 0x1294 */ f32 weapon_particle_damage_scaler;
    /* 0x1298 */ f32 m_ShockedTimer;
    /* 0x129C */ s32 m_LastFrameAllowedWeaponType;
    /* 0x12A0 */ s32 m_CurrentAllowedWeaponType;
    /* 0x12A4 */ s32 m_UseLowerAmmo;
    /* 0x12A8 */ struct CInstanceHeader *m_pHitMeWithCBore;
    /* 0x12AC */ struct CInstanceHeader *m_pHitMeWithChargeDart;
    /* 0x12B0 */ s32 m_nNumArrowsStuckInMe;
    /* 0x12B4 */ s32 m_nDeathmatchString;
    /* 0x12B8 */ char m_DeathMatchString[4][25];
    /* 0x131C */ struct CPlayer *m_pLast;
    /* 0x1320 */ struct CPlayer *m_pNext;
} CPlayer; /* size 0x1324 */

#endif
