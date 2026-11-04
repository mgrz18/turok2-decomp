#include "common.h"
#include "structs/collision.h"

/* Fills a damage event. */
void func_00245828(DamageEvent *event, void *source, s32 damage, f32 radius, s32 flags, HitFrame *frame) {
    event->source = source;
    event->damage = damage;
    event->radius = radius;
    event->flags = flags;
    event->frame = frame;
}
