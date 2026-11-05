#include "common.h"
#include "structs/light.h"
#include "structs/model.h"

extern s32 D_800B6D1C;
s32 *func_002017D4(s32 base, s32 index);
void func_0021DC84(void *actor, void *state, CDrawArgs *req);
void func_0020A33C(s32 hModel, s32 model, s32 one, CLights1 *slot, s32 zero, s32 index, s32 second);

void func_00266AB4(u8 *actor, u8 *state, CDrawArgs *req) {
    s32 i;
    s32 count;

    count = *func_002017D4(*(s32 *) req->m_hModel, 2);
    for (i = 0; i < count; i++) {
        req->m_nModel = i;
        req->m_nShadowModel = i;
        func_0021DC84(actor, state, req);
        func_0020A33C(req->m_hModel, *(s32 *) (actor + 0x98), 1, &((CLights1 *) (actor + 0x110))[D_800B6D1C], 0, i,
                      *(s8 *) (actor + 0x151));
    }
}
