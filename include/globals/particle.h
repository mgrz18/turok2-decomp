#ifndef GLOBALS_PARTICLE_H
#define GLOBALS_PARTICLE_H

/* Particles: the impact event handlers, the fired projectile's scales, trails and swooshes. */

#include "common.h"
#include "structs/vector.h"
#include "structs/particle_event.h"

extern CImpactEventHandler gImpactEventHandlers[34];   /* by event kind */
extern f32 gProjectileSpeedScale;   /* set by CPlayer__FireWeapon from gWeaponChargeScale */
extern f32 gProjectilePower;        /* the weapon's weapon_particle_damage_scaler */
extern void *sTrailTextures[4];     /* the trail texture records, by Trail tex0/tex1 */
extern s32 sSwooshSpawned;          /* cleared each frame by SwooshSystem_Update */

/* The particle drawer's matrices */
extern CMtxF sParticleParentMtx;
extern CMtxF sParticleRotXMtx;

/* The trail texture records sTrailTextures points at: morph points of 5 f32 (round 25, data_g8) */
extern f32 sTrailTexture0Points[10];
extern f32 sTrailTexture1Points[10];
extern f32 sTrailTexture2Points[30];
extern f32 sTrailTexture3Points[30];

/* Round 30 s2: the constants CParticle__UpdateReturning and CParticle__GetDistanceScale read by
 * name (defined in place by CParticle.c with asm labels on the N64), and Sprite_FromImage's two. */
extern f32 sReturningHomeFrame;     /* 15: flies home after this frame (engine .data) */
extern f32 sReturningCatchRadius;   /* 512: the catch radius (engine .data) */
extern f32 sDistanceScaleNear;      /* 16 */
extern f32 sDistanceScaleInvRange;  /* 1/2032 */
extern f32 sDistanceScaleRampMax;   /* 1 */
extern f32 sDistanceScaleInvRange2; /* 1/2032 */
extern f32 sDistanceScaleUnit;      /* 0.001 */
extern f32 sDistanceScaleDefault;   /* 1 */
extern f32 sDistanceScaleFovRef;    /* 47.5 */
extern f32 sDistanceScaleFovBlend;  /* 0.25 */
extern f32 sDistanceScaleInvFovRef; /* 1/47.5 */
extern f32 sSpriteTexelScale;       /* 32 (Trail.c) */
extern f32 sSpriteOne;              /* 1 */

#endif
