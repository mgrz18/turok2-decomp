/* Sets the video mode for the TV type and memory size: screen size, VI features. The mode record is read as an s32 array (an in-struct read changes GCC's alias ordering). */
#include "common.h"
#include "structs/frame.h"
#include "globals/camera.h"
#include "globals/render.h"
#include "globals/libultra.h"

/* One screen setting for a TV type and memory size (0x1C bytes). */
typedef struct {
    /* 0x00 */ s32 antialias;
    /* 0x04 */ s32 v[4];
} ScreenExtra; /* 0x14 */
extern ScreenExtra gVideoModeFlags[];
s32 GetMemSize(void); /* osGetMemSize */
void osViSetSpecialFeatures(u32 features); /* osViSetSpecialFeatures */
void Video_SetMode_2(unsigned width,int a1,int a2,int a3,int a4,int a5);

void Video_ApplyResolution_2(void) {
    s32 *mode = NULL;
    ScreenExtra extra;

    gEngineApp_m_Game_m_CameraPool.m_bActiveChanged = 2;
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
        gVideoDitherFilter = extra.antialias;
        gRenderAntiAlias = extra.v[0];
        gViAntiAlias = extra.v[1];
        gRenderZModeInter = extra.v[2];
        gRenderImageRead = extra.v[3];
        gScreenWidth = mode[0];
        gScreenHeight = mode[1];
        Video_SetMode_2(mode[0], mode[1], mode[2], mode[3], mode[4], mode[5]);
        osViSetSpecialFeatures(gVideoDitherFilter ? 0x40 : 0x80);
        osViSetSpecialFeatures(0x20);
        osViSetSpecialFeatures(2);
    }
}

#include "protos/ultra.h"
#include "structs/twin_pakmenu.h"

/* Resizes the selected VI mode around its original horizontal and field centres, keeping the original lower
 * screen limits, then applies it. */

/* A packed pair register (hStart, vStart, yScale): the high half and the low half. */





extern ViMode sResizedViMode;
extern void *func_0029E460(void *dst, const void *src, s32 count);

void Video_ResizeViMode_2(s32 width, s32 height, s32 x, s32 y, s32 borderX, s32 borderY) {
    ViMode *mode = &sResizedViMode;
    Span v[2];
    Span old[2];
    s16 center[2];
    Span h, oldH;
    Span yScale;
    u32 xScale;
    s32 oldRight;
    s32 cx;

    func_0029E460(mode, &osViModeTable[sViModeIndex], 0x50);
    old[0] = v[0] = mode->fldRegs[0].vStart;
    old[1] = v[1] = mode->fldRegs[1].vStart;
    center[0] = v[0].h.start + (v[0].h.end - v[0].h.start) / 2;
    center[1] = v[1].h.start + (v[1].h.end - v[1].h.start) / 2;
    h = mode->comRegs.hStart; /* new left/right edges */
    center[0] -= y;
    center[1] -= y;
    oldH = h; /* original edges */
    oldRight = oldH.h.end;
    cx = (oldRight - oldH.h.start) / 2 + oldH.h.start;
    cx -= x;
    xScale = cx;
    h.h.start = xScale - width / 2 + borderX / 2;
    h.h.end = xScale + width / 2 - borderX * 2;
    v[0].h.start = center[0] - height / 2 + borderY / 2;
    v[0].h.end = center[0] + height / 2 - borderY * 2;
    v[1].h.start = center[1] - height / 2 + borderY / 2;
    v[1].h.end = center[1] + height / 2 - borderY * 2;
    yScale = mode->fldRegs[0].yScale;
    yScale.h.end = (height << 11) / (v[0].h.end - v[0].h.start);
    v[0].h.end -= 3;
    v[1].h.end -= 3;
    xScale = (width << 10) / (h.h.end - h.h.start);
    if (old[0].h.end < v[0].h.end) {
        v[0].h.end = old[0].h.end;
    }
    if (old[1].h.end < v[1].h.end) {
        v[1].h.end = old[1].h.end;
    }
    if (oldRight < h.h.end) {
        h.h.end = oldH.h.end;
    }
    mode->fldRegs[0].yScale = yScale;
    mode->fldRegs[1].yScale = yScale;
    mode->fldRegs[0].origin = 0;
    mode->fldRegs[1].origin = width * 2;
    mode->comRegs.width = width;
    mode->comRegs.xScale = xScale;
    mode->comRegs.hStart = h;
    mode->fldRegs[0].vStart = v[0];
    mode->fldRegs[1].vStart = v[1];
    osViSetMode(mode);
}

#define MAX(a, b) ((a) >= (b) ? (a) : (b))

/* Steps to the next video mode of the TV region that fits in memory. */
void Video_NextResolution_2(void) {
    s32 region = 0;
    s32 *mode;
    s32 limit;

    switch (osTvType) {
        case 0:
        case 2:
            region = 1;
            break;
        case 1:
            break;
    }
    mode = (s32 *)&gVideoModes[region][gResolution];
    if (GetMemSize() != 0x400000) {
        limit = 0x2A300;
    } else {
        limit = 0xF660;
    }
    gResolution = (gResolution + 1) % gVideoModeCounts[region];
    while (mode[0] * mode[1] > limit) {
        gResolution = (gResolution + 1) % gVideoModeCounts[region];
    }
    gScreenWidth = MAX(gVideoModes[region][gResolution].width, gVideoModes[region][gAppliedResolution].width);
    gScreenHeight = MAX(gVideoModes[region][gResolution].height, gVideoModes[region][gAppliedResolution].height);
}

void Video_SetResolution_2(s32 arg0) {
    gResolution = arg0;
    Video_ApplyResolution_2();
}

#include "m2c_macros.h"

void Video_StartResolutionChange_2(void) {
    if (gVideoClearFrames == 0) {
        gVideoClearFrames = 0x14;
    }
}

void Video_UpdateResolution_2(void) {
    if (gResolution == -1) {
        if (GetMemSize() != 0x400000) {
            gResolution = 1;
        } else {
            gResolution = 0;
        }
    }
    if (gVideoClearFrames < 5 && gAppliedResolution != gResolution) {
        Video_ApplyResolution_2();
        gAppliedResolution = gResolution;
    }
}

#include "globals/menu_text.h"

char *Video_GetResolutionText_2(void) {
    switch (gResolution) {
        case 0:
        default:
            return sOptionsMenuText_ResolutionLoRez;
        case 1:
            return sOptionsMenuText_ResolutionHiRez;
        case 2:
            return sOptionsMenuText_ResolutionHiRezLb;
        case 3:
            return sOptionsMenuText_ResolutionLoRezLb;
    }
}

struct NodeRecAt294;
typedef struct NodeRecAt294 NodeRecAt294;
struct NodeRecAt294 {
    int unused;
    struct NodeRecAt294 *next;
    char pad[0x280];
    float width;
    float height;
    int x;
    int y;
};

void Video_SetMode_2(unsigned width,int a1,int a2,int a3,int a4,int a5) {
 int flags=0,region=0; NodeRecAt294 *node; float h;
 switch(osTvType) {case 0:region=1;break;case 1:break;case 2:region=2;break;}
 if(width>320) flags|=4;
 if(gViAntiAlias) flags|=2;
 if(width>320) flags|=1;
 sViModeIndex=gViModeChoices[region][flags].index;
 Video_ResizeViMode_2(width,a1,a2,a3,a4,a5);
 node=(NodeRecAt294 *)&gEngineApp_m_Game_m_CameraPool_m_MainCamera;
 if(node) {node->width=gScreenWidth;h=gScreenHeight;node->x=0;node->y=0;node->height=h;}
 node=*(NodeRecAt294 **)((char *)node-32);
 while(node) {node->x=0;node->y=0;node->width=gScreenWidth;node->height=gScreenHeight;node=node->next;}
}

#include "gfx.h"
#include "structs/menu.h"
#include "globals/menu.h"

s32 CPlayer__GetPickupCountHigh(u8 *player, s32 type);
void Hud_SetupTextureMode(s32 alpha);
void Hud_DrawSprite(s32 image, s32 frame, s16 x, s16 y, f32 scaleX, f32 scaleY, s32 arg6);

/* Draws an inventory item's icon, full colour when the player has it, greyed out otherwise. */
void InventoryMenu_DrawItem_2(MenuItem *item, MenuItemLayout *box, s32 unused, Menu *window) {
    s32 type = item->def->param;
    s32 index = item->def->index;
    u8 *player = window->player;
    s32 owned = 0;

    switch (type) {
        default:
            owned = index < CPlayer__GetPickupCountHigh(player, type);
            break;
        case 0x10FE:
            if (index >= 0) {
                if (index < 3) {
                    goto first_row;
                }
                if (index < 6) {
                    goto second_row;
                }
            }
            break;
        first_row:
            owned = index < player[0xA05];
            break;
        second_row:
            owned = index - 3 < player[0xA06];
            break;
        case 0x1194:
            owned = (player + index)[0x9FC] != 0;
            break;
    }
    if (gMenuCurrentFont != 0x14) {
        gMenuCurrentFont = 0x14;
        Hud_SetupTextureMode((s32) (window->fade * (window->alpha * 255.0f)));
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE7000000;
        g->w1 = 0;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3000A01;
        g->w1 = 0x100000;
    }
    if (owned) {
        Gfx *g = gDList++;
        g->w0 = 0xFB000000;
        g->w1 = ((s32) (window->fade * (window->alpha * 255.0f)) & 0xFF) | 0xFFFFFF00;
    } else {
        Gfx *g = gDList++;
        g->w0 = 0xFB000000;
        g->w1 = ((s32) (window->fade * (window->alpha * 255.0f)) & 0xFF) | 0x32323200;
    }
    Hud_DrawSprite(item->image, 0, (s16) box->left, (s16) box->top, box->scaleX, box->scaleY, 1);
}

void InventoryMenu_Init_2(void) {

}

s32 InventoryMenu_HasItem_2(MenuItem *item, Menu *event) {
    s32 type = item->def->param;
    s32 index = item->def->index;
    u8 *player = event->player;
    s32 result = 0;

    switch (type) {
        default:
            result = index < CPlayer__GetPickupCountHigh(player, type);
            break;
        case 0x10FE:
            if (index >= 0) {
                if (index < 3) {
                    goto first_row;
                }
                if (index < 6) {
                    goto second_row;
                }
            }
            break;
        first_row:
            result = index < player[0xA05];
            break;
        second_row:
            result = index - 3 < player[0xA06];
            break;
        case 0x1194:
            result = (player + index)[0x9FC] != 0;
            break;
    }
    return result;
}

s32 MenuItem_SelectSubmenu_2(sptr, sptr, sptr);
void Music_RestoreVolume();                            /* extern */

void InventoryMenu_SelectOk_2(s32 arg0, s32 arg1, s32 arg2) {
    Music_RestoreVolume();
    MenuItem_SelectSubmenu_2(arg0, arg1, arg2);
}
