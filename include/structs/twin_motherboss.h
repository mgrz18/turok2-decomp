/* Local types that vm1/MotherBoss and its vm2 copy both define (one layout): one definition
 * for both copies (tools/struct_census.py). */
#ifndef STRUCTS_TWIN_MOTHERBOSS_H
#define STRUCTS_TWIN_MOTHERBOSS_H

#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/collision.h"
#include "structs/vector.h"
#include "structs/level_actor.h"
#include "structs/player.h"
#include "structs/pick.h"
#include "structs/game.h"
#include "structs/light.h"
#include "structs/model.h"

typedef struct {
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ s32 type;
} AnimEvent;

#endif
