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

#endif
