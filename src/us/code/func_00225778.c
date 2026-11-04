/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/CODE_8028C490.CPP, func_8028CAE0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"


/* Turok 2 LevelActor: 0x288 bytes (Rage Wars 0x2E8). */

typedef struct World {
    char pad0[0xE0];
    CGameObjectInstance *objects;       /* 0xE0 (Rage Wars 0x138) */
    char padE4[0x4];
    s32 count;                 /* 0xE8 (Rage Wars 0x140) */
} World;

/* Nearest other actor matching team/kind (-1 = any), optionally alive only. */
CGameObjectInstance *func_00225778(World *world, CGameObjectInstance *self, s32 team, s32 kind, s32 activeOnly) {
    s32 i;
    CGameObjectInstance *obj;
    CGameObjectInstance *best;
    f32 min;
    f32 dx, dy, dz, distSq;
    s32 count;
    i = 0;
    min = 3.4028235e38f;
    count = world->count;
    best = 0;
    for (; i < count; i++) {
        obj = &world->objects[i];
        if (obj == self) {
            continue;
        }
        if (team != -1 && obj->m_nTypeFlag != team) {
            continue;
        }
        if (kind != -1 && *((s32 *)obj->header.m_pIntelligence) != kind) {
            continue;
        }
        if (activeOnly && obj->m_AI.m_Health == 0) {
            continue;
        }
        dx = self->header.m_vPos.x - obj->header.m_vPos.x;
        dy = self->header.m_vPos.y - obj->header.m_vPos.y;
        dz = self->header.m_vPos.z - obj->header.m_vPos.z;
        distSq = dx * dx + dy * dy + dz * dz;
        if (distSq < min) {
            min = distSq;
            best = obj;
        }
    }
    return best;
}
