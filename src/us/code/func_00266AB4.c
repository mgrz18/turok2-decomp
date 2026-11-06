#include "common.h"
#include "structs/light.h"
#include "structs/model.h"

extern s32 D_800B6D1C;
s32 *CISet__GetBlock(s32 base, s32 index);
void CGameObjectInstance__DrawShadow(void *actor, void *state, CDrawArgs *req);
void Render_DrawModel(s32 hModel, s32 model, s32 one, CLights1 *slot, s32 zero, s32 index, s32 second);

void InteractiveAnim_DrawAllModels(u8 *actor, u8 *state, CDrawArgs *req) {
    s32 i;
    s32 count;

    count = *CISet__GetBlock(*(s32 *) req->m_hModel, 2);
    for (i = 0; i < count; i++) {
        req->m_nModel = i;
        req->m_nShadowModel = i;
        CGameObjectInstance__DrawShadow(actor, state, req);
        Render_DrawModel(req->m_hModel, *(s32 *) (actor + 0x98), 1, &((CLights1 *) (actor + 0x110))[D_800B6D1C], 0, i,
                      *(s8 *) (actor + 0x151));
    }
}
