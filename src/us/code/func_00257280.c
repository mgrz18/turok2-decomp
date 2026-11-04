#include "common.h"
#include "structs/weapon.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x3C8];
    /* 0x3C8 */ s32 flags;
    /* 0x3CC */ u8 pad3CC[0x972 - 0x3CC];
    /* 0x972 */ s8 slots[1];
} SlotOwner;

s32 func_00257280(SlotOwner *self, s32 slot) {
    WeaponInfo *def = D_800AF618[slot];

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
