#ifndef STRUCTS_PARTICLE_H
#define STRUCTS_PARTICLE_H

/* The particle system: gScene_m_ParticleSystem, a pool of 128 particles (CParticle,
 * 0x1D8 each) with its free list, its draw list and three active lists (a
 * particle's template picks the list), and the ROM templates the particles
 * are spawned from (CROMParticleEffect, with the impact events of
 * CROMParticleImpact). CParticleSystem__Construct builds the system, CParticleSystem__Spawn spawns
 * particles, CParticleSystem__Update steps them, CParticleSystem__Free frees one,
 * CParticleSystem__Draw draws the draw list.
 *
 * Names from Rage Wars' debug records (CParticle_t, CParticleSystem_t,
 * CROMParticleEffect_t, CROMParticleImpact_t, CDLight_t: see
 * references/rw_abu/src/particle/CODE_80281A70.CPP), where Rage Wars has
 * them; Turok 2's particle is 0x20 bytes shorter at the front (its instance
 * header is the 0x18-byte CInstanceHeader) and 4 more from 0x11C on. The
 * other names come from what the code does.
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "structs/game_object.h"
#include "structs/list.h"

/* A dynamic light (the set at D_8010FF34: DLight_Spawn takes one,
 * CDLightSet__Free gives it back). A particle with a light moves it along. */
typedef struct CDLight {
    /* 0x00 */ struct CDLight *m_pPrev;
    /* 0x04 */ struct CDLight *m_pNext;
    /* 0x08 */ void *m_pROMLight;
    /* 0x0C */ f32 m_ColorFade;
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ s16 m_bEnabled;
} CDLight; /* size 0x18 */

/* The scene's dynamic lights (CScene.m_DLightSet, the N64 alias D_8010FF34): eight CDLights
 * on a used and a free list (func_0042D120 builds it; CDLightSet__Alloc/__Free move one
 * between them; DLights_Load walks the used list). */
typedef struct CDLightSet {
    /* 0x00 */ CList m_UsedList;
    /* 0x14 */ CList m_FreeList;
    /* 0x28 */ CDLight m_Lights[8];
} CDLightSet; /* size 0xE8 */

/* The value an impact event passes to the effect it starts (ImpactEvent_Send):
 * two halfwords, or the pair as one word. */
#ifdef TARGET_PORT
/* The host: the value is two host-order words, word 0 = the N64's first big-endian word
 * ((upper << 16) | lower), as the archive gives the animation events' values (s32[2]): the
 * halves by name, the words for the handlers that read them as ints (docs/port/RENDER.md 6). */
typedef struct CEventValue {
    s16 lower;
    s16 upper;
    s32 extra;
} CEventValue;
#else
typedef struct CEventValue {
    /* 0x0 */ s16 upper;
    /* 0x2 */ s16 lower;
    /* 0x4 */ s32 extra;
} CEventValue; /* size 0x8 */
#endif

/* What a particle does when something happens to it (it hits an object, the
 * floor, a wall, it expires...), one entry per event index: the particle,
 * the effect (0xFFFF: none) with its value, and the sound to start. */
typedef struct CROMParticleImpact {
    /* 0x00 */ CEventValue m_ImpactEventValue[14];
    /* 0x70 */ u16 m_ImpactParticleType[14];
    /* 0x8C */ u16 m_ImpactEventType[14];
    /* 0xA8 */ u16 m_ImpactSoundType[14];
} CROMParticleImpact; /* size 0xC4 */

/* Two half floats (HalfToFloat): a base and a random range (RandomInPackedRange takes the
 * pair by value). Every random value of a template is one of these. */
typedef struct CPackedRange {
    /* 0x0 */ u16 m_Base;
    /* 0x2 */ u16 m_Range;
} CPackedRange; /* size 0x4 */

/* A template's starting size and growth (CParticle.m_vSize/m_vGrowth), and how the size follows
 * its distance from the camera (CParticle__GetDistanceScale, CParticle__BuildMatrix): the scale
 * near and far. */
typedef struct CROMParticleDistance {
    /* 0x00 */ CPackedRange m_Size[6];
    /* 0x18 */ f32 m_ShadowScale;      /* the blob shadow's size (CParticle__DrawShadow) */
    /* 0x1C */ f32 m_NearScale;
    /* 0x20 */ f32 m_FarScale;
} CROMParticleDistance; /* size 0x24 */

/* A template's wobble (CParticle.m_WobblePhase/Amplitude) and the frames its opacity ramps up
 * over (CParticle__BuildMatrix). */
typedef struct CROMParticleOscillation {
    /* 0x00 */ CPackedRange m_Wobble[6];
    /* 0x18 */ s8 m_Ramp;
    /* 0x19 */ u8 unk_19[3];
} CROMParticleOscillation; /* size 0x1C */

/* A template's two colours, each shifted in hue, saturation and value by a random amount up to
 * the ranges when a particle spawns. */
typedef struct CROMParticleColors {
    /* 0x00 */ u8 m_Color0[3];
    /* 0x03 */ u8 m_Color1[3];
    /* 0x06 */ u8 unk_06[6];
    /* 0x0C */ s8 m_HueRange;
    /* 0x0D */ s8 m_SatRange;
    /* 0x0E */ s8 m_ValRange;
    /* 0x0F */ u8 unk_0F;
} CROMParticleColors; /* size 0x10 */

/* How a template spawns: life, chance, count, which active list... */
typedef struct CROMParticleParams {
    /* 0x00 */ s16 m_LifeBase;
    /* 0x02 */ s16 m_LifeRange;
    /* 0x04 */ s8 m_Chance;           /* percent */
    /* 0x05 */ s8 m_Group;
    /* 0x06 */ s8 m_CountBase;
    /* 0x07 */ s8 m_CountRange;
    /* 0x08 */ s8 m_nList;            /* CParticleSystem.m_ActiveList index */
    /* 0x09 */ u8 unk_09[0x0B - 0x09];
    /* 0x0B */ s8 unk_0B;
    /* 0x0C */ s8 m_nTrailCount;     /* copies spawned along its path each frame (CParticle__SpawnAlongPath) */
    /* 0x0D */ s8 m_Spread;
    /* 0x0E */ s8 m_Mode;
    /* 0x0F */ u8 m_Event;
} CROMParticleParams; /* size 0x10 */

/* How a template's particles start and speed up: the gravity (CParticle.m_Gravity) and the
 * speed along m_vDirection (random pairs), four half floats copied to CParticle 0x19C..0x1A8,
 * the acceleration along m_vDirection and the speed it stops at (CParticle__Accelerate). All
 * half floats (HalfToFloat). */
typedef struct CROMParticleMotion {
    /* 0x00 */ CPackedRange m_Gravity;
    /* 0x04 */ CPackedRange m_Speed;
    /* 0x08 */ u16 m_Render[4];
    /* 0x10 */ u16 m_Accel;
    /* 0x12 */ u16 m_MaxSpeed;
} CROMParticleMotion; /* size 0x14 */

/* A particle template (block 2 of the particle data; CParticle.m_pEffect). On the N64 the nine
 * pointers are offsets into block 0 that CScene__DecompressParticles relocates in place; the
 * host builds its own copy of the templates with full-width pointers instead
 * (port/game/t2particles.c, docs/port/RENDER.md section 6). */
typedef struct CROMParticleEffect {
    /* 0x00 */ u32 m_dwFlags;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s8 unk_06;
    /* 0x07 */ s8 m_DLightType;      /* the light DLight_Spawn gives the particle */
    /* 0x08 */ s8 m_AnimMode;
    /* 0x09 */ u8 unk_09;
    /* 0x0A */ s8 m_bClampToGround;
    /* 0x0B */ u8 unk_0B;
    /* 0x0C */ s16 m_nSound;
    /* 0x0E */ u8 unk_0E[2];
    /* 0x10 */ u8 m_PlayerMask;
    /* 0x11 */ u8 unk_11[3];
    /* 0x14 */ void *m_pModel;          /* drawn as this model, not as a sprite */
    /* 0x18 */ CROMParticleImpact *m_pImpact;
    /* 0x1C */ CPackedRange *m_pOffset;        /* [3]: x, y, z */
    /* 0x20 */ CPackedRange *m_pRotation;      /* [6]: CParticle.m_vRot / m_vRotSpeed */
    /* 0x24 */ CROMParticleDistance *m_pDistanceRange;
    /* 0x28 */ CPackedRange *m_pDirection;     /* [3]: x, y, z */
    /* 0x2C */ CROMParticleOscillation *m_pOscillation;
    /* 0x30 */ CROMParticleMotion *m_pMotion;
    /* 0x34 */ CROMParticleColors *m_pColors;
    /* 0x38 */ CROMParticleParams *m_pParams;
} CROMParticleEffect; /* size 0x3C */

typedef struct CParticle {
    /* 0x000 */ CInstanceHeader header;   /* m_Type 2 */
    /* 0x018 */ CVector3 m_vVelocity;
    /* 0x024 */ u8 unk_024[0x34 - 0x24];
    /* 0x034 */ CVector3 m_vAttachPos;    /* in the frame of the bone (or owner) it sticks to */
    /* 0x040 */ u32 m_dwFlags;
    /* 0x044 */ u8 unk_044[0x48 - 0x44];
    /* 0x048 */ u8 m_mtxDraw[2][0x40];    /* Mtx, one per frame buffer (gEvenOdd) */
    /* 0x0C8 */ CLights1 m_Lights[2];     /* one per frame buffer */
    /* 0x0F8 */ CROMParticleEffect *m_pEffect;
    /* 0x0FC */ u32 unk_0FC;
    /* 0x100 */ u8 unk_100[0x104 - 0x100];
    /* 0x104 */ void *m_pOwnerID;
    /* 0x108 */ void *m_pSource;          /* what spawned it */
    /* 0x10C */ struct CGameObjectInstance *m_pOwner;
    /* 0x110 */ s32 *m_pOwnerCount;       /* decremented when it is freed */
    /* 0x114 */ struct CGameObjectInstance *m_pTarget;   /* what it hit and sticks to */
    /* 0x118 */ CDLight *m_pDLight;
    /* 0x11C */ f32 m_cFrame;             /* its age; it expires at m_nFrames */
    /* 0x120 */ f32 m_AnimFrame;
    /* 0x124 */ f32 m_AnimSpeed;
    /* 0x128 */ s16 m_nFrames;
    /* 0x12A */ s8 m_BuiltFrame;
    /* 0x12B */ s8 m_RandomFrame;
    /* 0x12C */ CVector3 m_vSize;
    /* 0x138 */ CVector3 m_vGrowth;
    /* 0x144 */ CVector3 m_vLastPos;
    /* 0x150 */ CVector3 m_vDirection;
    /* 0x15C */ CVector3 m_vRot;
    /* 0x168 */ CVector3 m_vRotSpeed;
    /* 0x174 */ f32 m_Alpha;
    /* 0x178 */ f32 m_WobblePhase[2];
    /* 0x180 */ u8 unk_180[0x188 - 0x180];
    /* 0x188 */ f32 m_WobbleAmplitude[2];
    /* 0x190 */ u32 m_dwHitFlags;         /* the collision test's flags (Collision_MoveInstance) */
    /* 0x194 */ s8 m_ObjectImpact;        /* 1: it dies when it hits an object */
    /* 0x195 */ s8 m_bDieOnImpact;
    /* 0x196 */ s8 m_bDieOnBounce;
    /* 0x197 */ u8 unk_197;
    /* 0x198 */ f32 m_Gravity;
    /* 0x19C */ f32 unk_19C;
    /* 0x1A0 */ f32 m_RotationScale;      /* applied to m_vRotSpeed on an impact */
    /* 0x1A4 */ u8 unk_1A4[0x1AC - 0x1A4];
    /* 0x1AC */ s8 m_Alignment;           /* the space it lives in (CParticle__GetParentTransform) */
    /* 0x1AD */ union {
        /* 0x1AD */ s8 m_nBone;           /* the bone it sticks to */
        /* 0x1AD */ s8 m_Shake;           /* a decaying shake otherwise */
    };
    /* 0x1AE */ u8 m_PrimColor[3];
    /* 0x1B1 */ u8 m_EnvColor[3];
    /* 0x1B4 */ u8 m_Opacity;
    /* 0x1B5 */ u8 m_ActiveSwooshes;      /* its trail is in the swoosh set gEngineApp_m_SwooshSystem */
    /* 0x1B6 */ u8 unk_1B6[0x1B8 - 0x1B6];
    /* 0x1B8 */ struct SndSource *m_pLoopSound;  /* its looping sound (CParticle__StartLoopSound) */
    /* 0x1BC */ f32 m_Scale;
    /* 0x1C0 */ CList *m_pActiveList;
    /* 0x1C4 */ struct CParticle *m_pLast;      /* free list / active list links */
    /* 0x1C8 */ struct CParticle *m_pNext;
    /* 0x1CC */ struct CParticle *m_pDrawLast;  /* draw list links */
    /* 0x1D0 */ struct CParticle *m_pDrawNext;
    /* 0x1D4 */ u8 unk_1D4[0x1D8 - 0x1D4];
} CParticle; /* size 0x1D8 */

typedef struct CParticleSystem {
    /* 0x0000 */ CParticle m_ParticleVars[128];
    /* 0xEC00 */ CList m_FreeList;
    /* 0xEC14 */ CList m_DrawList;
    /* 0xEC28 */ CList m_ActiveList[3];
    /* 0xEC64 */ void *m_pParticleData;
} CParticleSystem; /* size 0xEC68 */

#endif
