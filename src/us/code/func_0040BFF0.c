#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/collision.h"

/* When the instance the last collision test hit (D_800F1CE0) is one of the AI's two targets during anim state 0xE/0xF, knocks it back
 * away from the point (0, 0, 512) (and upward) and deals it 0x500 damage. */

extern CCollisionResults D_800F1CE0;  /* the last collision test (m_pInstance: what it hit) */
extern void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a - b */
extern void func_0020EEF8(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a + b */
extern void func_0020EF60(CVector3 *out, CVector3 *in, f32 scale);
extern void func_0020F040(CVector3 *v); /* normalise */
extern void func_00245828(DamageEvent *event, void *source, s32 damage, f32 radius, s32 flags, void *shield);
extern void func_002465C8(CGameObjectInstance *target, CAI *ai, DamageEvent *event);

void func_0040BFF0(CGameObjectInstance *obj, CAI *ai) {
    DamageEvent event;
    CVector3 dir;
    CGameObjectInstance *player = (CGameObjectInstance *) D_800F1CE0.m_pInstance;

    if (player != NULL && player->header.m_Type == 1 && (player->m_dwFlags & 0x300000) && ai->m_Health != 0 &&
        (ai->m_Mode == 0xE || ai->m_Mode == 0xF) && (player == ai->m_pSightTarget || player == ai->m_pPathTarget)) {
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 512.0f;
        func_0020EF2C(&dir, &obj->header.m_vPos, &dir);
        func_0020F040(&dir);
        func_0020EF60(&dir, &dir, 1536.0f);
        dir.y = 409.59998f;
        func_0020EEF8(&player->m_vVelocity, &player->m_vVelocity, &dir);
        func_00245828(&event, obj, 0x500, 102.399994f, 0x100000, NULL);
        func_002465C8(player, &player->m_AI, &event);
    }
}
