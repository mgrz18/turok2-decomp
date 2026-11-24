/* Local types that vm1/TotemDefense and its vm2 copy both define (one layout): one definition
 * for both copies (tools/struct_census.py). */
#ifndef STRUCTS_TWIN_TOTEMDEFENSE_H
#define STRUCTS_TWIN_TOTEMDEFENSE_H

#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"

typedef struct {
    /* 0x00 */ u8 pad00[0x44];
    /* 0x44 */ s16 limit;
} TargetDef;
typedef struct {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ f32 timer;
} HurtTimer;

#endif
