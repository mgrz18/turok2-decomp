#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/collision.h"

/* When the instance the last collision test hit (D_800F1CE0) is one of the AI's two targets during anim state 0xE/0xF, knocks it back
 * away from the point (0, 0, 512) (and upward) and deals it 0x500 damage. */

extern CCollisionResults D_800F1CE0;  /* the last collision test (m_pInstance: what it hit) */
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a - b */
extern void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a + b */
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
extern void CVector3__Normalize(CVector3 *v); /* normalise */
extern void DamageEvent_Set(DamageEvent *event, void *source, s32 damage, f32 radius, s32 flags, void *shield);
extern void CAI__CallHurt(CGameObjectInstance *target, CAI *ai, DamageEvent *event);

void MotherBoss_Collision(CGameObjectInstance *obj, CAI *ai) {
    DamageEvent event;
    CVector3 dir;
    CGameObjectInstance *player = (CGameObjectInstance *) D_800F1CE0.m_pInstance;

    if (player != NULL && player->header.m_Type == 1 && (player->m_dwFlags & 0x300000) && ai->m_Health != 0 &&
        (ai->m_Mode == 0xE || ai->m_Mode == 0xF) && (player == ai->m_pSightTarget || player == ai->m_pPathTarget)) {
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 512.0f;
        CVector3__Subtract(&dir, &obj->header.m_vPos, &dir);
        CVector3__Normalize(&dir);
        CVector3__Scale(&dir, &dir, 1536.0f);
        dir.y = 409.59998f;
        CVector3__Add(&player->m_vVelocity, &player->m_vVelocity, &dir);
        DamageEvent_Set(&event, obj, 0x500, 102.399994f, 0x100000, NULL);
        CAI__CallHurt(player, &player->m_AI, &event);
    }
}
