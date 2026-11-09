#include "common.h"
#include "structs/light.h"
#include "structs/model.h"
#include "globals/engine.h"
#include "protos/game.h"

s32 *CISet__GetBlock(s32 base, s32 index);
void CGameObjectInstance__DrawShadow(void *actor, void *state, CDrawArgs *req);

void InteractiveAnim_DrawAllModels(u8 *actor, u8 *state, CDrawArgs *req) {
    s32 i;
    s32 count;

    count = *CISet__GetBlock(*(s32 *) req->m_hModel, 2);
    for (i = 0; i < count; i++) {
        req->m_nModel = i;
        req->m_nShadowModel = i;
        CGameObjectInstance__DrawShadow(actor, state, req);
        Render_DrawModel(req->m_hModel, *(s32 *) (actor + 0x98), 1, &((CLights1 *) (actor + 0x110))[gEvenOdd], 0, i,
                      *(s8 *) (actor + 0x151));
    }
}
