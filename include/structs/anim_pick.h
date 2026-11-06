#ifndef STRUCTS_ANIM_PICK_H
#define STRUCTS_ANIM_PICK_H
#include "ultra_types.h"
/* One row of an AI_PickAnimFromTable table: {flag bits, anim id}, ended by flags == -1. */
typedef struct AIAnimPick {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s32 anim;
} AIAnimPick; /* size 0x8 */
#endif
