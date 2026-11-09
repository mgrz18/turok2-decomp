#include "common.h"
#include "gfx.h"
#include "structs/frame.h"
#include "globals/render.h"
#include "protos/iset.h"

/* One two-word display list command at the list head. */
#define GFX_CMD(a, b) { Gfx *_g = gDList++; _g->w0 = (a); _g->w1 = (b); }


s32 Render_SetupMaterial(s32, s32);
void CCache__LockForDisplayList(s32, s32 **);
void Render_SetRenderMode(s32);

void func_0020BBE4(s32 arg0, s32 renderMode, s32 **model, s32 matrix, s32 projected,
                   s32 lights, s32 *segment, s32 group, s32 arg8) {
    s32 *obj;
    s32 *list;
    s32 *entry;
    s32 *dl;
    s32 count;
    s32 i;
    s32 *seg;

    if ((u32) (gDisplayListSize - ((u32) ((u8 *) gDList - (u8 *) gEngineApp_m_pCurrentFrameData->m_pDisplayList) >> 3)) < 3000) {
        return;
    }
    if (Render_SetupMaterial(arg0, arg8) == 0) {
        return;
    }
    if (projected) {
        GFX_CMD(0xDB060004, matrix);
    } else {
        GFX_CMD(0xDA380003, matrix);
    }
    if (lights) {
        GFX_CMD(0xDB020000, 0x18);
        GFX_CMD(0xDC08060A, lights + 8);
        GFX_CMD(0xDC08090A, lights);
    }
    if (renderMode != -1) {
        Render_SetRenderMode(renderMode);
    }
    CCache__LockForDisplayList(0, model);
    obj = *model;
    seg = segment;
    if (seg == NULL) {
        seg = CISet__GetBlock(obj, 0);
    }
    GFX_CMD(0xDB060008, (u32) seg);
    list = CISet__GetBlock(CISet__GetBlock(obj, 2), group);
    count = *list;
    for (i = 0; i < count; i++) {
        entry = CISet__GetBlock(list, i);
        CISet__GetBlock(entry, 0);
        dl = CISet__GetBlock(entry, 1);
        GFX_CMD(0xDE000000, (u32) dl);
    }
}
