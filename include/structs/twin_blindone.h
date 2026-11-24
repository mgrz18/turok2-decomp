/* Local types that vm1/BlindOne and its vm2 copy both define (one layout): one definition
 * for both copies (tools/struct_census.py). */
#ifndef STRUCTS_TWIN_BLINDONE_H
#define STRUCTS_TWIN_BLINDONE_H

#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/light.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
} Matrix18;

#endif
