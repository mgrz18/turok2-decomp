/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/CODE_8028C490.CPP, func_8028CAE0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"

/* Turok 2 LevelActor: 0x288 bytes (Rage Wars 0x2E8). */
typedef struct LevelActor {
    char pad0[0x4];
    f32 x;                     /* 0x04 (Rage Wars 0x08) */
    f32 y;                     /* 0x08 */
    f32 z;                     /* 0x0C */
    char pad10[0x4];
    s32 *kind;                 /* 0x14 def (Rage Wars 0x18) */
    char pad18[0xB8 - 0x18];
    u16 team;                  /* 0xB8 type id (Rage Wars 0xE4) */
    char padBA[0x144 - 0xBA];
    s32 active;                /* 0x144 health (Rage Wars 0x174) */
    char pad148[0x288 - 0x148];
} LevelActor;

/* Nearest other actor matching team/kind (-1 = any), optionally alive only. */
LevelActor *func_00225778(CWorld *world, LevelActor *self, s32 team, s32 kind, s32 activeOnly) {
    s32 i;
    LevelActor *obj;
    LevelActor *best;
    f32 min;
    f32 dx, dy, dz, distSq;
    s32 count;
    i = 0;
    min = 3.4028235e38f;
    count = world->m_nActors;
    best = 0;
    for (; i < count; i++) {
        obj = (LevelActor *)world->m_pActors + i;
        if (obj == self) {
            continue;
        }
        if (team != -1 && obj->team != team) {
            continue;
        }
        if (kind != -1 && *obj->kind != kind) {
            continue;
        }
        if (activeOnly && obj->active == 0) {
            continue;
        }
        dx = self->x - obj->x;
        dy = self->y - obj->y;
        dz = self->z - obj->z;
        distSq = dx * dx + dy * dy + dz * dz;
        if (distSq < min) {
            min = distSq;
            best = obj;
        }
    }
    return best;
}
