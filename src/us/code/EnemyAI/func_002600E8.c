#include "common.h"
#include "structs/vector.h"
#include "structs/intelligence.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/enemy_ai.h"

#include "structs/game_object.h"

void TotemDefense_AddKill(void);

/* On CGameObjectInstance since round 29 (the N64 and port views, Actor, read only the position,
 * the definition and m_dwFlags). */
void EnemyAI_NukedFrozenRemove(CGameObjectInstance *self, CAI *ai) {
    CIntelligence *def;

    Actor_SpawnParticle(&self->header, 0x17, self->header.m_vPos, 0);
    TotemDefense_AddKill();
    def = self->header.m_pIntelligence;
    if (!(self->m_dwFlags & 0x80000)) {
        if (!(def->m_dwTypeFlags & 0x20)) {
            CScene__SetInstanceSavedBit(&gScene, self, 1);
        }
        GameObject_FireTriggers(self, 1, (sptr) self);
    }
    self->m_dwFlags &= ~0x100;
    self->m_dwFlags &= ~0x2000;
    AI_StopSound(self, ai);
    if (self->m_dwFlags & 0x80000) {
        CListedInstance__Free(self);
    }
}
