#include "common.h"
#include "structs/weapon.h"

#ifdef TARGET_PORT
#include "structs/player.h"
/* Host body: SlotOwner is the CPlayer (N64 0x3C8 m_WeaponActor.m_AI.m_dwFlags, 0x972
 * m_Data.m_WeaponList). */
s32 CPlayer__CanSelectWeapon(CPlayer *self, s32 slot) {
    WeaponInfo *def = gWeaponInfoTable[slot];

    if (self->m_Data.m_WeaponList[slot] == 0) {
        return 0;
    }
    if (self->m_WeaponActor.m_AI.m_dwFlags & 0x80) {
        if (!(def->flags & 2)) {
            return 0;
        }
    } else if (!(def->flags & 1)) {
        return 0;
    }
    if (self->m_Data.m_WeaponList[def->alternate] != 0 && slot < def->alternate) {
        return 0;
    }
    return 1;
}
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x3C8];
    /* 0x3C8 */ s32 flags;
    /* 0x3CC */ u8 pad3CC[0x972 - 0x3CC];
    /* 0x972 */ s8 slots[1];
} SlotOwner;

s32 CPlayer__CanSelectWeapon(SlotOwner *self, s32 slot) {
    WeaponInfo *def = gWeaponInfoTable[slot];

    if (self->slots[slot] == 0) {
        return 0;
    }
    if (self->flags & 0x80) {
        if (!(def->flags & 2)) {
            return 0;
        }
    } else if (!(def->flags & 1)) {
        return 0;
    }
    if (self->slots[def->alternate] != 0 && slot < def->alternate) {
        return 0;
    }
    return 1;
}
#endif
