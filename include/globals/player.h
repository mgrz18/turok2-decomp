#ifndef GLOBALS_PLAYER_H
#define GLOBALS_PLAYER_H

/* The player's tables: state table and tunings, input remap, flashlights,
 * characters, HUD overlay scripts, riding, the weapon wheel (CBarrel), the
 * orbit camera and the weapon actor. */

#include "common.h"
#include "structs/vector.h"
#include "structs/game_object.h"
#include "structs/player_state.h"
#include "structs/ai_type.h"
#include "structs/collision.h"

extern ButtonMap sControlSide1ButtonMap[20];   /* CPlayerOptions m_ControlSide 1 */

/* Dynamic light descriptions (CDLightSet__Alloc), CPlayer__UpdateFlashlight. */
extern u8 sFlashlightLight1[8];
extern u8 sFlashlightLight2[8];
extern u8 sDarkCityLight[8];
extern u8 sDarkCityLight2[8];

extern s32 gCharacterModels[11];    /* multiplayer character model by intelligence m_Id */
extern s32 gTagModeModel;           /* the model of a tagged player */

/* COverlay scripts (words), started by CPlayer__LevelStart / CPlayer__DrawHud. */
extern s32 gHealthOverlayScript[5];
extern s32 gAmmoOverlayScript[4];
extern s32 gAmmoOverlayChangeScript[11];
extern s32 gLivesOverlayScript[4];
extern s32 gLivesOverlayChangeScript[12];
extern s32 gTokensOverlayScript[4];
extern s32 gTokensOverlayChangeScript[12];
extern s32 gTotalKillsOverlayScript[4];
extern s32 gTotalKillsOverlayChangeScript[12];
extern s32 gTotalTagsOverlayScript[4];
extern s32 gTotalTagsOverlayChangeScript[12];

extern CVector3 gRidingEyeOffset;   /* the eye offset riding object type 0x136 */
extern CVector3 gRidingScale;       /* copied to m_vOnEnemyScale */

/* CBarrel__LayoutEight slot layouts: group != 0, group 0, underwater; then multiplayer. */
extern s32 sBarrelLayoutGroup1[8];
extern s32 sBarrelLayoutGroup0[8];
extern s32 sBarrelLayoutUnderwater[8];
extern s32 sBarrelLayoutGroup1MP[8];
extern s32 sBarrelLayoutGroup0MP[8];
extern s32 sBarrelLayoutUnderwaterMP[8];

extern f32 gCharacterHealthRegen[13];   /* health regained per second, by character */

extern CMoveTuning gStandMoveTuning;
extern CMoveTuning gRidingForwardMoveTuning;
extern CMoveTuning gRidingBackwardMoveTuning;
extern CMoveTuning gDodgeMoveTuning;
extern CMoveTuning gCrouchMoveTuning;
extern CLookTuning gStandLookTuning;
extern CLookTuning gCrouchLookTuning;
extern CLookTuning gRidingLookTuning;
extern CMoveTuning gWadeMoveTuning;
extern CMoveTuning gSwimMoveTuning;
extern CLookTuning gSwimLookTuning;
extern CLookTuning gPlayerState12LookTuning;
extern CMoveTuning gPlayerState12MoveTuning;

extern PlayerStateInfo gPlayerStateTable[36];   /* by CPlayer m_Mode */

extern s16 gLivesSpeech[11];        /* speech by number of lives */
extern f32 gBarrelSlotScale;        /* the weapon wheel's slot scale */
extern f32 sOrbitCameraAngle;
extern f32 sOrbitCameraDistance;
extern s32 gDeathTint;              /* forces a tint over the view (CCamera__DrawTints) */
extern f32 gWeaponChargeScale;      /* 0.5-2, WeaponMode_WindUp_Update */

/* WeaponActor_BuildMatrix mirrors the weapon on an axis when set (debug). */
extern s32 sWeaponFlipX;
extern s32 sWeaponFlipY;
extern s32 sWeaponFlipZ;

extern CAIModeInfo sWeaponActorModeTable[48];   /* installed by WeaponActor_InitAI */
extern CIntelligenceTypeInfo gWeaponActorTypeInfo;  /* the type block of a player's second object */

extern s16 gAmmoIcons[];            /* HUD icon by ammo type, -1 none */

/* The blast shield's hit box, the cinema players and the death tint */
extern HitFrame sBlastShieldFrame;       /* the shield CGameObjectInstance__GetNearestBox finds */
extern struct CPlayer *sCinema50Player;
extern struct CPlayer *sDeathCinemaPlayer;
extern u8 gDeathTintColor[4];

#endif
