/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/CODE_8028C490.CPP, func_8028CAE0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
#include "common.h"
#include "structs/world.h"

/* Turok 2 LevelActor: 0x288 bytes (Rage Wars 0x2E8). */

/* Nearest other actor matching team/kind (-1 = any), optionally alive only. */
CGameObjectInstance *CScene__FindNearestActor(CScene *world, CGameObjectInstance *self, s32 team, s32 kind, s32 activeOnly) {
    s32 i;
    CGameObjectInstance *obj;
    CGameObjectInstance *best;
    f32 min;
    f32 dx, dy, dz, distSq;
    s32 count;
    i = 0;
    min = 3.4028235e38f;
    count = world->m_nActors;
    best = 0;
    for (; i < count; i++) {
        obj = (CGameObjectInstance *)world->m_pActors + i;
        if (obj == self) {
            continue;
        }
        if (team != -1 && obj->m_nTypeFlag != team) {
            continue;
        }
        if (kind != -1 && obj->header.m_pIntelligence->m_Type != kind) {
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
