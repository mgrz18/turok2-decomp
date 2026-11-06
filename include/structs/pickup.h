#ifndef STRUCTS_PICKUP_H
#define STRUCTS_PICKUP_H

/* A pickup's effect descriptor, what the pickup handlers get
 * (Pickup_GiveWeapon weapons, Pickup_GiveAmmo / func_0023F21C / func_0023F300
 * ammo, Pickup_GiveHealth health, Pickup_GiveLifeForce life force, Pickup_GiveItem the
 * items by m_Id): the feedback (the screen flash, then the optional
 * message, sound and callback) and what the pickup gives, which each kind
 * reads its own way (m_Params). CPlayer__AddAmmo gives ammo (type, amount).
 * Rage Wars: src/MENU/802ABDBC_11.CPP (the pickup feedback).
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"

typedef struct {
    /* 0x0 */ s16 m_Weapon;                 /* index into gWeaponInfoTable */
    /* 0x2 */ s16 m_AmmoType;               /* -1: no ammo with it */
    /* 0x4 */ s16 m_AmmoAmount;
} CPickupWeapon; /* size 0x6 */

typedef struct {
    /* 0x0 */ s16 m_Type;                   /* -1: none */
    /* 0x2 */ s16 m_Amount;
} CPickupAmmo; /* size 0x4 */

typedef struct {
    /* 0x0 */ s32 m_Amount;
    /* 0x4 */ s32 m_Max;
} CPickupHealth; /* size 0x8 */

typedef union {
    CPickupWeapon m_Weapon;
    CPickupAmmo m_Ammo;
    CPickupHealth m_Health;
    s16 m_Amount;                           /* one amount (ammo type 2, life force) */
} CPickupParams;

typedef struct CPickupEntry {
    /* 0x00 */ void *m_pMessage;            /* shown on the player's camera */
    /* 0x04 */ s16 m_Id;                    /* which item (Pickup_GiveItem) */
    /* 0x06 */ s16 m_Sound;
    /* 0x08 */ s16 m_Callback;              /* Sound__PlaySpeech */
    /* 0x0A */ u8 _padA[2];
    /* 0x0C */ CPickupParams m_Params;
} CPickupEntry; /* size 0x14 */

/* The pickup tables' rows (globals/pickup.h), keyed by the pickup's id: a
 * descriptor's head, what the row gives, then the handler that takes it
 * (CPlayer__CollectPickup; the row is the handler's descriptor). */
typedef struct CPickupRow18 {               /* weapons (m_Weapon), health (m_Health) */
    /* 0x00 */ void *m_pMessage;
    /* 0x04 */ s16 m_Id;
    /* 0x06 */ s16 m_Sound;
    /* 0x08 */ s16 m_Callback;
    /* 0x0A */ u8 _padA[2];
    /* 0x0C */ CPickupParams m_Params;
    /* 0x14 */ s32 (*m_pHandler)(void *player, struct CPickupRow18 *row);
} CPickupRow18; /* size 0x18 */

typedef struct CPickupRow14 {               /* ammo (m_Ammo), life force (m_Amount) */
    /* 0x00 */ void *m_pMessage;
    /* 0x04 */ s16 m_Id;
    /* 0x06 */ s16 m_Sound;
    /* 0x08 */ s16 m_Callback;
    /* 0x0A */ u8 _padA[2];
    /* 0x0C */ union {
        CPickupAmmo m_Ammo;
        s16 m_Amount;
    } m_Params;
    /* 0x10 */ s32 (*m_pHandler)(void *player, struct CPickupRow14 *row);
} CPickupRow14; /* size 0x14 */

typedef struct CPickupRow10 {               /* items, by m_Id */
    /* 0x00 */ void *m_pMessage;
    /* 0x04 */ s16 m_Id;
    /* 0x06 */ s16 m_Sound;
    /* 0x08 */ s16 m_Callback;
    /* 0x0A */ u8 _padA[2];
    /* 0x0C */ s32 (*m_pHandler)(void *player, struct CPickupRow10 *row, void *pickup);
} CPickupRow10; /* size 0x10 */

/* {mask, pickup id}: AI_DropPickupsOnce drops the pickup for each mask bit set; 0-ended. */
typedef struct CDropPickup {
    /* 0x0 */ u32 m_Mask;
    /* 0x4 */ s32 m_Id;
} CDropPickup; /* size 0x8 */

#endif
