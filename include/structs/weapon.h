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
typedef struct AmmoInfo {
    /* 0x0 */ s16 type;             /* index into the player's ammo counts */
    /* 0x2 */ s16 max;              /* the most the player can carry */
    /* 0x4 */ s16 projectile;
    /* 0x6 */ u16 cost;             /* ammo used per shot */
} AmmoInfo; /* size 0x8 */

/* A weapon's definition (D_800AF618[weapon]). */
typedef struct WeaponInfo {
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

/* The weapon wheel (the "barrel", CPlayer m_Barrel at 0xCD0). Two layouts:
 * a strip of every usable weapon that scrolls by m_Spin (CBarrel__RunButtons,
 * filled by CBarrel__Open / CBarrel__Fill), and, with control style 1, the
 * eight-slot stick wheel (CBarrel__RunStick, laid out by CBarrel__LayoutEight from a
 * table per button group, another one under water). CBarrel__Update updates
 * it, CBarrel__DrawRing / CBarrel__DrawEight build the icons and CBarrel__DrawIcons draws
 * them. LibTEngine names the struct CBarrel (m_nItems, m_nLastItem) but its
 * layout does not fit; the field names here come from the code. */

/* One weapon on the wheel. */
typedef struct {
    /* 0x00 */ s32 m_Weapon;        /* -1: an empty slot */
    /* 0x04 */ WeaponInfo *m_pInfo;
    /* 0x08 */ s32 m_bAvailable;    /* owned and has ammo */
    /* 0x0C */ f32 m_Angle;         /* radians round the wheel */
    /* 0x10 */ f32 m_Lift;
    /* 0x14 */ f32 m_Scale;
} CBarrelItem; /* size 0x18 */

/* One icon to draw (CBarrel__DrawRing / CBarrel__DrawEight build them on the stack). */
typedef struct {
    /* 0x00 */ s32 m_Item;          /* index into CBarrel.m_Items */
    /* 0x04 */ CVector3 m_vPos;
    /* 0x10 */ s32 m_Icon;          /* WeaponInfo.icon */
    /* 0x14 */ f32 m_Alpha;
    /* 0x18 */ f32 m_Size;
} CBarrelIcon; /* size 0x1C */

typedef struct {
    /* 0x000 */ s32 m_State;        /* 0 closed, 1 opening, 3 closing */
    /* 0x004 */ f32 m_Turn;         /* degrees of a step still to turn */
    /* 0x008 */ f32 m_Open;         /* 0..1 */
    /* 0x00C */ f32 m_Spin;         /* the strip's angle */
    /* 0x010 */ s32 m_Step;         /* the direction it turns, -1 or 1 */
    /* 0x014 */ f32 m_Glow;         /* phase of the cursor's pulse */
    /* 0x018 */ s32 m_nCount;
    /* 0x01C */ CBarrelItem m_Items[36];
    /* 0x37C */ s32 m_nCursor;      /* -1: none */
    /* 0x380 */ s32 m_bStickWheel;  /* the eight-slot wheel, not the strip */
    /* 0x384 */ s32 m_Group;        /* which button opened the wheel */
    /* 0x388 */ s32 m_bBothHeld;
    /* 0x38C */ s32 m_bUnderwater;  /* laid out from the underwater table */
    /* 0x390 */ s32 m_Hover;
} CBarrel; /* size 0x394 */

extern WeaponInfo *D_800AF618[]; /* the weapon table */

#endif
