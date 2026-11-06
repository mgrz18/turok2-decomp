#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"

/* Picks a random point on the ground around the origin: a distance in the phase's range
 * [unk688, unk68C] and a random angle in [-pi, pi]. */

extern f32 Random_Blend(f32 min, f32 max);
extern f32 func_002982F0(f32 angle); /* sin */
extern f32 func_002974C0(f32 angle); /* cos */

void MotherBoss_RandomArenaPoint(CVector3 *out) {
    MotherBossState *state = &gMotherBoss;
    f32 dist;
    f32 angle;
    f32 z;

    dist = Random_Blend((f32)state->minRadius, (f32)state->maxRadius);
    angle = Random_Blend(-3.1415927f, 3.1415927f);
    out->x = dist * func_002982F0(angle);
    z = dist * func_002974C0(angle);
    out->y = 0.0f;
    out->z = z;
}
