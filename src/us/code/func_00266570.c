#include "common.h"
#include "structs/light.h"
#include "structs/model.h"

extern s32 D_800B6D1C;
void CGameObjectInstance__DrawShadow(void *actor, void *state, CDrawArgs *req);
void Render_QueueModelInstance(s32 anim, s32 model, s32 one, CLights1 *slot, s32 zero, s32 first, s32 second);

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
        Render_QueueModelInstance(req->m_hModel, *(s32 *) (actor + 0x98), 1, &((CLights1 *) (actor + 0x110))[D_800B6D1C], 0,
                      first, second);
    }
}
