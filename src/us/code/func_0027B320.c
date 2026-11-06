#include "common.h"
#include "structs/frame.h"

s32 CCache__Unlock(s32, s32);                /* extern */
s32 CScene__LoadGraphicSet(s32 *, s32, s32, s32); /* extern */
s32 Hud_DrawImageFrame(s32, s32, s16, s16, f32, f32, s32); /* extern */
extern s32 D_800B6D54;
extern s32 D_800C2214;
extern s32 D_800D8E24;
extern s32 gScene;

s32 Hud_DrawSprite(s32 arg0, s32 arg1, s16 arg2, s16 arg3, f32 arg4, f32 arg5, s32 arg6) {
    s32 temp_v0;

    if (((u32) (D_800C2214 - ((u32) (D_800D8E24 - (s32) gEngineApp_m_pCurrentFrameData->m_pDisplayList) >> 3)) < 0x3E8U) || (D_800B6D54 == 0) || (temp_v0 = CScene__LoadGraphicSet(&gScene, arg0, 0, 1), (temp_v0 == 0))) {
        return 0;
    }
    Hud_DrawImageFrame(temp_v0, arg1, arg2, arg3, arg4, arg5, arg6);
    CCache__Unlock(0, temp_v0);
    return 1;
}
