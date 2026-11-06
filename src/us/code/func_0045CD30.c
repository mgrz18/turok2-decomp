/* Sets the video mode for the TV type and memory size: screen size, VI features. The mode record is read as an s32 array (an in-struct read changes GCC's alias ordering). */
#include "common.h"
#include "structs/frame.h"
#include "globals/render.h"

/* One screen setting for a TV type and memory size (0x1C bytes). */
typedef struct {
    /* 0x00 */ s32 antialias;
    /* 0x04 */ s32 v[4];
} ScreenExtra; /* 0x14 */

extern s32 osTvType; /* osTvType */
extern s32 D_800C2044;
extern ScreenExtra gVideoModeFlags[];
extern s32 gRenderAntiAlias;
extern s32 gViAntiAlias;
extern s32 gRenderZModeInter;
extern s32 gRenderImageRead;
extern s32 gEngineApp_m_Game_m_CameraPool_m_bActiveChanged;

s32 GetMemSize(void); /* osGetMemSize */
void osViSetSpecialFeatures(u32 features); /* osViSetSpecialFeatures */
void Video_SetMode_2(s32 width, s32 height, s32 arg2, s32 arg3, s32 arg4, s32 arg5);

void Video_ApplyResolution_2(void) {
    s32 *mode = NULL;
    ScreenExtra extra;

    gEngineApp_m_Game_m_CameraPool_m_bActiveChanged = 2;
    if (gResolution == -1) {
        if (GetMemSize() != 0x400000) {
            gResolution = 1;
        } else {
            gResolution = 0;
        }
    }
    switch (osTvType) {
        case 1:
            mode = (s32 *)&gVideoModes[0][gResolution];
            break;
        case 0:
        case 2:
            mode = (s32 *)&gVideoModesPAL[gResolution];
            break;
    }
    if (mode != NULL) {
        extra = gVideoModeFlags[gResolution];
        D_800C2044 = extra.antialias;
        gRenderAntiAlias = extra.v[0];
        gViAntiAlias = extra.v[1];
        gRenderZModeInter = extra.v[2];
        gRenderImageRead = extra.v[3];
        gScreenWidth = mode[0];
        gScreenHeight = mode[1];
        Video_SetMode_2(mode[0], mode[1], mode[2], mode[3], mode[4], mode[5]);
        osViSetSpecialFeatures(D_800C2044 ? 0x40 : 0x80);
        osViSetSpecialFeatures(0x20);
        osViSetSpecialFeatures(2);
    }
}
