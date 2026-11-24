#include "common.h"
#include "protos/enemy_ai.h"
#include "structs/game_object.h"

/* On CGameObjectInstance since round 29 (the N64 and port views, Actor, read only m_dwFlags). */
void EnemyAI_StopSoundAndFree(CGameObjectInstance *self, CAI *ai) {
    self->m_dwFlags &= ~0x100;
    self->m_dwFlags &= ~0x2000;
    AI_StopSound(self, ai);
    if (self->m_dwFlags & 0x80000) {
        CListedInstance__Free(self);
    }
}
