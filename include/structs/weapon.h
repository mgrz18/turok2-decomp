#ifndef STRUCTS_WEAPON_H
#define STRUCTS_WEAPON_H

/* The weapons.
 *
 * D_800AF618 is the weapon table: one WeaponInfo per weapon, indexed by the
 * weapon number (CPlayer m_Data.m_CurrentWeapon, m_RequestedWeapon, the
 * m_WeaponList slots). Rage Wars has the same record (rw_abu
 * include/COMMON/ITEMRECAT20V1.H, struct WeaponInfo: model at 0x4,
 * alternate at 0xC, the three ammo classes at 0x20); its later fields moved,
 * so the names past 0x20 come from what Turok 2 does with them. */

#include "common.h"
#include "structs/vector.h"

/* One ammo class of a weapon (WeaponInfo.ammo[]). */
typedef struct {
    /* 0x0 */ s16 type;             /* index into the player's ammo counts */
    /* 0x2 */ s16 max;              /* the most the player can carry */
    /* 0x4 */ s16 projectile;
    /* 0x6 */ u16 cost;             /* ammo used per shot */
} AmmoInfo; /* size 0x8 */

/* A weapon's definition (D_800AF618[weapon]). */
typedef struct {
    /* 0x00 */ u8 *name;            /* the message shown when it is picked */
    /* 0x04 */ u16 model;           /* the weapon actor's object type */
    /* 0x06 */ u16 icon;            /* in the weapon wheel */
    /* 0x08 */ u16 zoomModel;       /* the scope model shown when zoomed, 0 = none */
    /* 0x0A */ u16 pickupId;        /* the pickup dropped for it */
    /* 0x0C */ s16 alternate;       /* the weapon it upgrades to / pairs with */
    /* 0x0E */ s16 aimHold;
    /* 0x10 */ s16 lockHold;
    /* 0x12 */ u8 pad12[0x14 - 0x12];
    /* 0x14 */ u32 flags;           /* 1: usable on land, 2: under water, 4: automatic */
    /* 0x18 */ f32 fireDelay;       /* scaled into the weapon AI's refire timer */
    /* 0x1C */ f32 aimCone;
    /* 0x20 */ AmmoInfo *ammo[3];   /* [1] is the lower ammo */
    /* 0x2C */ CVector3 viewOffset; /* the first-person model offset */
    /* 0x38 */ f32 scopeLeft;
    /* 0x3C */ f32 scopeTop;
    /* 0x40 */ f32 scopeRight;
    /* 0x44 */ f32 scopeBottom;
    /* 0x48 */ struct CAIModeInfo *modeTable; /* the weapon actor's m_AI.m_pModeTable */
    /* 0x4C */ void *effects;       /* the weapon actor's m_AI.m_pfEffects */
    /* 0x50 */ void (*update)();    /* called every frame on the weapon actor */
} WeaponInfo;

extern WeaponInfo *D_800AF618[]; /* the weapon table */

#endif
