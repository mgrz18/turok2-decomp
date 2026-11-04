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
#include "structs/vector.h"

/* The definition m_pIntelligence points at. */
#include "structs/intelligence.h"

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
    /* 0x08 */ s16 m_nFrames;
    /* 0x0A */ s8 m_bCycleCompleted;
    /* 0x0B */ s8 m_bAnimActive;
    /* 0x0C */ u32 m_nAnimStartFrame;
    /* 0x10 */ struct CMemEntry *pmeAnimData;
} CAnimationState; /* size 0x14 */

typedef struct {
    /* 0x00 */ s8 m_Mode;
    /* 0x01 */ u8 m_nCurrentPathIndex;
    /* 0x02 */ s16 m_nCurrentPointIndex;
} CPathTrack; /* size 0x4 */

/* One row of an AI's mode table (CAI::m_pModeTable, e.g. D_800B0A60: a
 * 0x20-byte header row, then one row per mode; m_pModeCurrent points at the
 * current one). The AI step calls m_pfCallback while the mode runs. */
typedef struct CAIModeInfo {
    /* 0x00 */ s32 m_Mode;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 (*m_pfCallback)();
    /* 0x0C */ void *m_pData;   /* D_800F1C20 in the enemy table */
    /* 0x10 */ s32 unk_10;   /* a function in the mode rows */
    /* 0x14 */ f32 m_AnimSpeed;   /* copied to CAI::m_AnimModeSpeed */
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ u32 unk_1C;
} CAIModeInfo; /* size 0x20 */

/* The views of CAI::m_VarBuffer: CEnemyAIVars m_Enemy, m_Turret, m_Platform,
 * m_Generator (ai_vars.h); the bosses and the weapon actor (ai_vars_boss.h). */
#include "structs/ai_vars.h"
#include "structs/ai_vars_boss.h"

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
    /* 0x10C */ union {
        /* Each AI type keeps its own state here (timers, counters, a
         * second target...), read through the view of the type it needs. */
        /* 0x10C */ u32 m_VarBuffer[12];
        /* 0x10C */ s32 m_VarS32[12];
        /* 0x10C */ f32 m_VarF32[12];
        /* 0x10C */ void *m_VarPtr[12];
        /* 0x10C */ struct CGameObjectInstance *m_VarObj[12];
        /* 0x10C */ CVector3 m_VarVec3[4];
        /* 0x10C */ s16 m_VarS16[24];
        /* 0x10C */ u16 m_VarU16[24];
        /* 0x10C */ s8 m_VarS8[48];
        /* 0x10C */ u8 m_VarU8[48];
        /* 0x10C */ CBlindOneAIVars m_BlindOne;
        /* 0x10C */ CMotherAIVars m_Mother;
        /* 0x10C */ CWeaponAIVars m_Weapon;
        /* 0x10C */ CEnemyAIVars m_Enemy;
        /* 0x10C */ CTurretAIVars m_Turret;
        /* 0x10C */ CPlatformAIVars m_Platform;
        /* 0x10C */ CGeneratorAIVars m_Generator;
    };
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
    /* 0x0B8 */ u16 m_nTypeFlag;
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

/* A dynamic object (an actor spawned at run time, an effect...): the
 * instance followed by its links in the world's object lists (CList, links
 * at m_LastOffset 0x288 / m_NextOffset 0x28C: the active list D_8010FEB4,
 * the free list D_8010FEA0) and the counter it decrements when it goes. */
typedef struct CListedInstance {
    /* 0x000 */ CGameObjectInstance m_Instance;
    /* 0x288 */ struct CListedInstance *m_pPrev;
    /* 0x28C */ struct CListedInstance *m_pNext;
    /* 0x290 */ s32 *m_pOwnerCount;
} CListedInstance; /* size 0x294 */

/* What an AI aims at this frame: func_002449E0 fills one from CAI's
 * m_pSightTarget, func_00244338 one from m_pPathTarget (or the home point, a
 * location, the actor itself when there is none; m_Kind says which). The
 * position, the offset from the actor and its length, in 3D and flattened. */
typedef struct {
    /* 0x00 */ s32 m_Kind;
    /* 0x04 */ struct CGameObjectInstance *m_pTarget;
    /* 0x08 */ f32 m_Angle;   /* the turn to face it */
    /* 0x0C */ CVector3 m_vPos;
    /* 0x18 */ CVector3 m_vDelta;
    /* 0x24 */ f32 m_Dist;
    /* 0x28 */ CVector3 m_vFlatPos;
    /* 0x34 */ CVector3 m_vFlatDelta;
    /* 0x40 */ f32 m_FlatDist;
} CAITarget; /* size 0x44 */

/* The pair the AI callbacks hand each other (func_00246108 fills both). */
typedef struct {
    /* 0x00 */ CAITarget m_Sight;
    /* 0x44 */ CAITarget m_Path;
} CAITargets; /* size 0x88 */

#endif
