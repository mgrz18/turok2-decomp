#ifndef STRUCTS_GAME_OBJECT_H
#define STRUCTS_GAME_OBJECT_H

/* The engine's game object (an "actor": enemies, pickups, the player's
 * body...), CGameObjectInstance in Acclaim's naming. Layout and field names
 * from LibTEngine's Turok 2 types (Drahsid/LibTEngine,
 * lib/libtengine/lib/types), checked against this ROM: tools/struct_scan.py
 * finds ~100 engine functions reading it through their first argument at
 * these offsets with these widths (floats at the position, a halfword at
 * m_nTypeFlag, words at m_dwFlags, m_AI.m_pParent and m_dwHotPointFlags).
 * Fields named unk_ are not yet known. */

#include "common.h"

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
} CVector3; /* size 0xC */

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0C */ f32 w;
} CQuaternion; /* size 0x10 */

typedef f32 CMtxF[4][4];

typedef struct {
    /* 0x00 */ u8 m_Type;
    /* 0x01 */ u8 _pad;
    /* 0x02 */ u16 m_nObjType;
    /* 0x04 */ CVector3 m_vPos;
    /* 0x10 */ struct CGameRegion *m_pCurrentRegion;
    /* 0x14 */ struct CIntelligence *m_pIntelligence;
} CInstanceHeader; /* size 0x18 */

typedef struct {
    /* 0x00 */ f32 m_AnimationFrame;
    /* 0x04 */ s16 m_nAnimIndex;
    /* 0x06 */ s16 m_nRequestedAnimIndex;
    /* 0x08 */ u16 m_nFrames;
    /* 0x0A */ u8 m_bCycleCompleted;
    /* 0x0B */ u8 m_bAnimActive;
    /* 0x0C */ u32 m_nAnimStartFrame;
    /* 0x10 */ struct CMemEntry *pmeAnimData;
} CAnimationState; /* size 0x14 */

typedef struct {
    /* 0x00 */ s8 m_Mode;
    /* 0x01 */ u8 m_nCurrentPathIndex;
    /* 0x02 */ s16 m_nCurrentPointIndex;
} CPathTrack; /* size 0x4 */

/* AI state embedded in every game object at 0x140. Health is 24.8 fixed point. */
typedef struct {
    /* 0x000 */ s32 m_dwFlags;
    /* 0x004 */ s32 m_Health;
    /* 0x008 */ s32 m_InitialHealth;
    /* 0x00C */ s32 m_VisBits;
    /* 0x010 */ u8 m_TranqHealth;
    /* 0x011 */ s8 m_nTexture;
    /* 0x012 */ s8 m_FlinchCountdown;
    /* 0x013 */ s8 m_FlinchTime;
    /* 0x014 */ s8 m_FlinchNode[4];
    /* 0x018 */ CVector3 m_vFlinchAxis;
    /* 0x024 */ f32 m_HeadTurn;
    /* 0x028 */ f32 m_HeadTilt;
    /* 0x02C */ struct CAIModeInfo *m_pModeTable;
    /* 0x030 */ struct CAIModeInfo *m_pModeCurrent;
    /* 0x034 */ s8 m_Mode;
    /* 0x035 */ s8 m_ModeLastFrame;
    /* 0x036 */ s8 m_ModeBefore;
    /* 0x037 */ s8 m_Action;
    /* 0x038 */ u32 m_ModeLastFrameFlags;
    /* 0x03C */ u32 m_ModeFlags;
    /* 0x040 */ f32 m_ModeTime;
    /* 0x044 */ f32 m_ActionTime;
    /* 0x048 */ CVector3 m_vPosLastFrame;
    /* 0x054 */ CVector3 m_vDesiredPos;
    /* 0x060 */ f32 m_RotYLastFrame;
    /* 0x064 */ f32 m_ConditionTimer;
    /* 0x068 */ struct CGameObjectInstance *m_pParent;
    /* 0x06C */ CVector3 m_vTargetOffset;
    /* 0x078 */ struct CGameObjectInstance *m_pTargetOverride;
    /* 0x07C */ f32 m_TargetOverrideTime;
    /* 0x080 */ struct CGameObjectInstance *m_pSightTarget;
    /* 0x084 */ struct CGameRegion *m_pPathAvoidRegion;
    /* 0x088 */ struct CGameObjectInstance *m_pPathTarget;
    /* 0x08C */ f32 m_PathRotY;
    /* 0x090 */ f32 m_PathHeight;
    /* 0x094 */ CPathTrack m_PathTrack;
    /* 0x098 */ f32 m_InitialRotY;
    /* 0x09C */ CVector3 m_vInitialPos;
    /* 0x0A8 */ struct CGameRegion *m_pInitialRegion;
    /* 0x0AC */ CVector3 m_vLeashPos;
    /* 0x0B8 */ struct CGameRegion *m_pLeashRegion;
    /* 0x0BC */ f32 m_AnimSpeed;
    /* 0x0C0 */ f32 m_AnimModeSpeed;
    /* 0x0C4 */ s16 m_AnimType;
    /* 0x0C6 */ s8 m_AnimIndex;
    /* 0x0C7 */ s8 m_AnimRepeatedCount;
    /* 0x0C8 */ u8 m_Aggression;
    /* 0x0C9 */ u8 m_FreezeTime;
    /* 0x0CA */ s8 m_PathFindCount;
    /* 0x0CB */ s8 m_LastHurtSFXTimer;
    /* 0x0CC */ f32 m_DeathRegionTimer;
    /* 0x0D0 */ f32 m_OnFireTimer;
    /* 0x0D4 */ CVector3 m_vBoreTarget;
    /* 0x0E0 */ CVector3 m_vBoreCollNormal;
    /* 0x0EC */ struct CLoopingSoundData *m_pAttachedLoopingSound;
    /* 0x0F0 */ void *m_pfStep;
    /* 0x0F4 */ void *m_pfDraw;
    /* 0x0F8 */ void *m_pfHurt;
    /* 0x0FC */ void *m_pfCollision;
    /* 0x100 */ void *m_pfHit;
    /* 0x104 */ void *m_pfCalculateOrientationMatrix;
    /* 0x108 */ void *m_pfEffects;
    /* 0x10C */ u32 m_VarBuffer[12];
} CAI; /* size 0x13C */

typedef struct CGameObjectInstance {
    /* 0x000 */ CInstanceHeader header;
    /* 0x018 */ CVector3 m_vVelocity;
    /* 0x024 */ CVector3 m_vWaterDisplacement;
    /* 0x030 */ struct CGameObjectInstance *m_pInstanceBelow;
    /* 0x034 */ CVector3 m_vScale;
    /* 0x040 */ CQuaternion m_qGround;
    /* 0x050 */ f32 m_RotY;
    /* 0x054 */ f32 m_CollisionYOffset;
    /* 0x058 */ CMtxF m_mfOrient;
    /* 0x098 */ struct Mtx *m_pmtDrawMtxs;
    /* 0x09C */ struct Mtx *m_pmtLastFrameDrawMtxs;
    /* 0x0A0 */ CMtxF *m_pmfShadow;
    /* 0x0A4 */ void *m_rpObject;
    /* 0x0A8 */ void *m_rpAnims;
    /* 0x0AC */ void *m_rpGeometryHeader;
    /* 0x0B0 */ u32 m_rpObjectHeader;
    /* 0x0B4 */ u32 m_rpGeometryHeaderSize;
    /* 0x0B8 */ s16 m_nTypeFlag;
    /* 0x0BA */ s16 m_nAnims;
    /* 0x0BC */ CVector3 m_vMin;
    /* 0x0C8 */ CVector3 m_vMax;
    /* 0x0D4 */ s32 m_dwFlags;
    /* 0x0D8 */ CAnimationState m_CurrentAnimation;
    /* 0x0EC */ CAnimationState m_BlendAnimation;
    /* 0x100 */ s16 m_BlendLength;
    /* 0x102 */ u16 _pad1;
    /* 0x104 */ f32 m_Blend;
    /* 0x108 */ f32 m_BlendPercent;
    /* 0x10C */ u8 m_BlendStart;
    /* 0x10D */ u8 m_BlendFinish;
    /* 0x10E */ u8 m_ShadowAlpha;
    /* 0x10F */ u8 m_ActiveSwooshes;
    /* 0x110 */ u8 m_Lights[2][0x18];   /* Lights1[2] (libultra gbi.h) */
    /* 0x140 */ CAI m_AI;
    /* 0x27C */ s32 m_dwHotPointFlags;
    /* 0x280 */ s32 m_dwUniqueID;
    /* 0x284 */ s32 _pad2;
} CGameObjectInstance; /* size 0x288 */

#endif
