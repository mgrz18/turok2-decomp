#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"

extern CListedInstance *gScene_m_InstancePool_m_ActiveList;  /* active actors */

extern f32 gFrameIncrement;          /* per-frame scale */
extern f32 Random_Float(f32 scale);                                      /* random 0..scale */
extern void Actor_SpawnParticle(void *actor, s32 event, CVector3 pos, s32 arg);
extern void DamageEvent_Set(DamageEvent *event, void *source, s32 damage, f32 radius, s32 flags, void *shield);
extern void EnemyAI_OnDamage(CListedInstance *actor, void *ai, DamageEvent *event);

typedef struct {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ f32 timer;
} HurtTimer;

/* After a delay, one time in four, hurt the first live player actor. */
void TotemDefense_TryHitEnemyAfterDelay(HurtTimer *h) {
    DamageEvent event;
    CListedInstance *actor;

    if (h->timer > 0.0f) {
        h->timer -= gFrameIncrement;
        return;
    }
    if (Random_Float(100.0f) > 25.0f) {
        return;
    }
    for (actor = gScene_m_InstancePool_m_ActiveList; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            Actor_SpawnParticle(actor, 0x17C, actor->m_Instance.header.m_vPos, 0);
            DamageEvent_Set(&event, NULL, 0xFF00, 51.199997f, 0x100000, NULL);
            EnemyAI_OnDamage(actor, &actor->m_Instance.m_AI.m_dwFlags, &event);
            return;
        }
    }
}
