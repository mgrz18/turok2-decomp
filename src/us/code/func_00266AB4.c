#include "common.h"
#include "structs/light.h"
#include "structs/model.h"
#include "globals/engine.h"
#include "protos/game.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif

s32 *CISet__GetBlock(sptr base, s32 index);
void CGameObjectInstance__DrawShadow(void *actor, void *state, CDrawArgs *req);

void InteractiveAnim_DrawAllModels(u8 *actor, u8 *state, CDrawArgs *req) {
    s32 i;
    s32 count;

#ifdef TARGET_PORT
    count = *CISet__GetBlock(*(sptr *) req->m_hModel, 2); /* the handle holds a pointer */
#else
    count = *CISet__GetBlock(*(s32 *) req->m_hModel, 2);
#endif
    for (i = 0; i < count; i++) {
        req->m_nModel = i;
        req->m_nShadowModel = i;
        CGameObjectInstance__DrawShadow(actor, state, req);
#ifdef TARGET_PORT
        /* the instance's members: m_pmtDrawMtxs, m_Lights, m_AI.m_nTexture */
        Render_DrawModel(req->m_hModel, (sptr) ((CGameObjectInstance *) actor)->m_pmtDrawMtxs, 1,
                         &((CGameObjectInstance *) actor)->m_Lights[gEvenOdd], 0, i,
                         ((CGameObjectInstance *) actor)->m_AI.m_nTexture);
#else
        Render_DrawModel(req->m_hModel, *(s32 *) (actor + 0x98), 1, &((CLights1 *) (actor + 0x110))[gEvenOdd], 0, i,
                      *(s8 *) (actor + 0x151));
#endif
    }
}
