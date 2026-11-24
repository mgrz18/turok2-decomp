/* Local types that vm1/Primagen and its vm2 copy both define (one layout): one definition
 * for both copies (tools/struct_census.py). */
#ifndef STRUCTS_TWIN_PRIMAGEN_H
#define STRUCTS_TWIN_PRIMAGEN_H

#include "common.h"
#include "structs/game_object.h"
#include "structs/game.h"
#include "structs/boss.h"
#include "structs/vector.h"
#include "structs/light.h"
#include "structs/model.h"

typedef struct {
    /* 0x00 */ u8 flag;
    /* 0x01 */ u8 pad01[0x8 - 0x1];
    /* 0x08 */ s32 count;
    /* 0x0C */ f32 delay;
    /* 0x10 */ u8 pad10[0x14 - 0x10];
    /* 0x14 */ s32 type;
    /* 0x18 */ u8 pad18[0x1C - 0x18];
} Wave0040457C;
typedef struct {
    /* 0x0 */ f32 waitMin;       /* 0x8012F618 */
    /* 0x4 */ f32 waitMax;       /* 0x8012F61C */
} BossWait;
typedef struct {
    u8 data[0x64];
} Slot00407DC0;

#endif
