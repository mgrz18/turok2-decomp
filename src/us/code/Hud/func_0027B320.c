#include "common.h"
#include "structs/frame.h"
#include "globals/scene.h"
#include "gfx.h"
#include "globals/render.h"
#include "globals/engine.h"
#include "protos/cache.h"
#include "protos/scene.h"

void Hud_DrawImageFrame(void *table, s32 frame, s32 x, s32 y, f32 scaleX, f32 scaleY, s32 scaled);

s32 Hud_DrawSprite(s32 arg0, s32 arg1, s16 arg2, s16 arg3, f32 arg4, f32 arg5, s32 arg6) {
    sptr temp_v0;

    if (((u32) (gDisplayListSize - ((u32) ((sptr) gDList - (sptr) gEngineApp_m_pCurrentFrameData->m_pDisplayList) / GFX_SIZE)) < 0x3E8U) || (gPersistentDataLoaded == 0) || (temp_v0 = CScene__LoadGraphicSet(&gScene, arg0, 0, 1), (temp_v0 == 0))) {
        return 0;
    }
    Hud_DrawImageFrame((void *) temp_v0, arg1, (s16) arg2, (s16) arg3, arg4, arg5, arg6); /* The ROM's caller narrows the coordinates to s16 at the call. */
    CCache__Unlock(0, temp_v0);
    return 1;
}
