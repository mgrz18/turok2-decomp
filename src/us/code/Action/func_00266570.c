#include "common.h"
#include "structs/light.h"
#include "structs/model.h"
#include "globals/engine.h"

void CGameObjectInstance__DrawShadow(void *actor, void *state, CDrawArgs *req);
void Render_QueueModelInstance(sptr anim, sptr model, s32 one, CLights1 *slot, sptr zero, s32 first, s32 second);

#ifdef TARGET_PORT
#include "structs/game_object.h"
/* Host body: the instance's members (N64 0x14 m_pIntelligence, 0x98 m_pmtDrawMtxs, 0x110
 * m_Lights, 0x14C m_AI.m_VisBits; the AI's 0x34 m_Mode, 0x11 m_nTexture). The
 * intelligence body is asset data: its byte offsets stay. */
void Action_Draw(u8 *actorp, u8 *statep, CDrawArgs *req) {
    CGameObjectInstance *actor = (CGameObjectInstance *) actorp;
    CAI *state = (CAI *) statep;
    u8 *info = (u8 *) actor->header.m_pIntelligence + 0x14;
    s32 first;
    s32 second;

    if (state->m_Mode == 0) {
        first = *(s8 *) (info + 0xE);
        second = *(s8 *) (info + 0xF);
    } else {
        first = *(s8 *) (info + 0x1A);
        second = *(s8 *) (info + 0x1B);
    }
    req->m_nModel = first;
    req->m_nShadowModel = first;
    state->m_nTexture = second;
    actor->m_AI.m_VisBits = 1 << req->m_nModel;
    if (req->m_bShadow != 0) {
        CGameObjectInstance__DrawShadow(actor, state, req);
    }
    if (req->m_bDraw != 0) {
        Render_QueueModelInstance(req->m_hModel, (sptr) actor->m_pmtDrawMtxs, 1, &actor->m_Lights[gEvenOdd], 0,
                      first, second);
    }
}
#else
void Action_Draw(u8 *actor, u8 *state, CDrawArgs *req) {
    u8 *info = *(u8 **) (actor + 0x14) + 0x14;
    s32 first;
    s32 second;

    if (*(s8 *) (state + 0x34) == 0) {
        first = *(s8 *) (info + 0xE);
        second = *(s8 *) (info + 0xF);
    } else {
        first = *(s8 *) (info + 0x1A);
        second = *(s8 *) (info + 0x1B);
    }
    req->m_nModel = first;
    req->m_nShadowModel = first;
    *(s8 *) (state + 0x11) = second;
    *(s32 *) (actor + 0x14C) = 1 << req->m_nModel;
    if (req->m_bShadow != 0) {
        CGameObjectInstance__DrawShadow(actor, state, req);
    }
    if (req->m_bDraw != 0) {
        Render_QueueModelInstance(req->m_hModel, *(s32 *) (actor + 0x98), 1, &((CLights1 *) (actor + 0x110))[gEvenOdd], 0,
                      first, second);
    }
}
#endif
