#ifndef STRUCTS_WEAPON_NODE_H
#define STRUCTS_WEAPON_NODE_H

/* The N64 view of a weapon model node the WeaponNode_Rotate_* callbacks get (code/WeaponMode). */

#include "common.h"

typedef struct WeaponNodeAnim {
    /* 0x000 */ u8 pad0[0x24C];
    /* 0x24C */ f32 angle;
} WeaponNodeAnim;

typedef struct WeaponNodeBone {
    /* 0x00 */ WeaponNodeAnim *anim;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ s32 type;
} WeaponNodeBone;

#endif
