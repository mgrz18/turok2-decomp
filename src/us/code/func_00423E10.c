#include "common.h"
#include "structs/frame.h"
#include "globals/libultra.h"

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/FRONTEND/8040B45C_21.CPP, func_8040BBB0_de, with Turok 2's layouts. */
/* Video setup: picks the low or high resolution set (high when an expansion pak is present, decided once), then
 * the mode for the TV type, copies its flags to the display globals and starts the display. */

typedef struct {
    s32 a, b, c, d, e;
} VideoFlags;

extern s32 gEngineApp_m_Game_m_CameraPool_m_bActiveChanged;
extern s32 gScreenWidth; /* screen width */
extern s32 gScreenHeight; /* screen height */
extern s32 gResolution; /* resolution set, -1 until decided */
extern s32 D_800C2044;
extern s32 gRenderAntiAlias;
extern s32 gViAntiAlias;
extern s32 gRenderZModeInter;
extern s32 gRenderImageRead;
extern VideoFlags gVideoModeFlags[];
extern s32 GetMemSize(void); /* osGetMemSize */
extern void Video_SetMode(s32 width, s32 height, s32 a2, s32 a3, s32 a4, s32 a5);
extern void osViSetSpecialFeatures(s32 flags);

static __inline__ void SetConfig(s32 a, s32 b, s32 c, s32 d, s32 e) {
    D_800C2044 = a;
    gRenderAntiAlias = b;
    gViAntiAlias = c;
    gRenderZModeInter = d;
    gRenderImageRead = e;
}

static __inline__ void SetMode(s32 *mode) {
    gScreenWidth = mode[0];
    gScreenHeight = mode[1];
    Video_SetMode(mode[0], mode[1], mode[2], mode[3], mode[4], mode[5]);
}

void Video_ApplyResolution(void) {
    VideoMode *mode = NULL;
    s32 flag;

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
            mode = &gVideoModes[0][gResolution];
            break;
        case 0:
        case 2:
            mode = &gVideoModesPAL[gResolution];
            break;
    }
    if (mode != NULL) {
        VideoFlags flags = gVideoModeFlags[gResolution];

        SetConfig(flags.a, flags.b, flags.c, flags.d, flags.e);
        SetMode((s32 *)mode);
        flag = 0x80;
        if (D_800C2044 != 0) {
            flag = 0x40;
        }
        osViSetSpecialFeatures(flag);
        osViSetSpecialFeatures(0x20);
        osViSetSpecialFeatures(2);
    }
}
