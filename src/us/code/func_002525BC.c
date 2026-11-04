#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

/* Tag game state. */
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 limit;
    /* 0x1C */ s32 over;
    /* 0x20 */ u8 pad20[0x34 - 0x20];
    /* 0x34 */ s32 enabled;
    /* 0x38 */ s32 pick;
    /* 0x3C */ s32 pickStart;
    /* 0x40 */ s32 phase;
} TagState;

extern TagState D_8011ACB0;
extern u8 D_8011AAEA;
extern s32 D_800C2030;          /* screen width */
extern s32 D_800C2034;          /* screen height */

extern s32 func_002113A4(void);
extern void func_0025247C(CPlayer *p);
extern void func_00252840(CPlayerList *list);
extern void func_002534E0(CPlayerList *list);
extern s32 func_00254A5C(CPlayerList *list);
extern CPlayer *func_00254B04(CPlayerList *list, u32 index);
extern void func_00254B6C(CPlayerList *list);
extern s32 func_00254B9C(CPlayerList *list);
extern void func_00279028(s32 color);
extern void func_0027B320(s32, s32, s16, s16, f32, f32, s32);

/* Tag: pick who is "it" at the start of a round, flashing over the viewports
 * in turn until the pick lands. */
void func_002525BC(CPlayerList *list) {
    s32 count;
    CPlayer *p;
    CCamera *cam;
    f32 w, h, cx, cy, sx, sy;
    TagState *s = &D_8011ACB0;

    if (!s->enabled) {
        return;
    }
    if (func_00254A5C(list) && s->limit > 0 && !s->over) {
        func_002534E0(list);
        return;
    }
    count = func_00254B9C(list);
    s = &D_8011ACB0;
    switch (s->phase) {
        case 0:
        default:
            func_00254B6C(list);
            func_00252840(list);
            s->phase = 1;
            break;
        case 1:
            func_00252840(list);
            if (count > 0) {
                s32 r = func_002113A4() % count;
                s->phase = 2;
                s->pickStart = r;
                s->pick = r;
            }
            break;
        case 2:
            p = func_00254B04(list, s->pick % count);
            cam = p->m_pCamera;
            if (cam != NULL && *(s32 *)((u8 *)cam + 0x54C) == 0   /* inside m_MenuPool: menu open */) {
                func_00279028(D_8011AAEA);
                w = cam->m_Viewport.m_Width;
                h = cam->m_Viewport.m_Height;
                cx = cam->m_Viewport.m_ScreenPosX + w * 0.5f;
                cy = cam->m_Viewport.m_ScreenPosY + h * 0.5f;
                sx = w / D_800C2030;
                sy = h / D_800C2034;
                func_0027B320(0x1FB, 0, cx - sx * 16.0f, cy - sy * 16.0f, sx, sy, 1);
            }
            s = &D_8011ACB0;
            if (++s->pick >= (5 - count) * 20 - s->pickStart) {
                func_00254B6C(list);
                s->phase = 3;
                func_0025247C(p);
            }
            break;
        case 3:
            break;
    }
}
