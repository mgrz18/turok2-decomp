/* De-faked in round 21 (j4): the N64 body used to test an uninitialised local
 * (`if (temp_s2)` with two identical arms, which clang -O2 on the host compiled to a
 * trap, round 7 v4) and had a separate host body; the plain loop matches. */
#include "common.h"
#include "structs/world.h"
#include "structs/game_object.h"

void AI_HearNoise(u8 *actor, CAI *ai, CVector3 *pos, s32 kind);

/* Every kind-1 actor (CScene::m_pKind1, count m_nKind1) hears a noise of the given kind at pos. */
void func_00225C08(CScene *scene, s32 kind, CVector3 *pos) {
    s32 n = scene->m_nKind1;
    s32 i;

    for (i = 0; i < n; i++) {
        CGameObjectInstance *actor = scene->m_pKind1[i];
        AI_HearNoise((u8 *) actor, &actor->m_AI, pos, kind);
    }
}
