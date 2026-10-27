#include "common.h"

typedef struct Quad {
    s32 w[4];
} Quad;

typedef struct Target {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 z;
    /* 0x10 */ s32 region;
    /* 0x14 */ u8 pad14[0x40 - 0x14];
    /* 0x40 */ Quad quad;
    /* 0x50 */ f32 angle;
} Target;

typedef struct ViewState {
    /* 0x000 */ u8 pad0[0x510];
    /* 0x510 */ s32 mode;
    /* 0x514 */ u8 pad514[8];
    /* 0x51C */ void *camera;
    /* 0x520 */ u8 pad520[0xA64 - 0x520];
    /* 0xA64 */ u32 buttons;
    /* 0xA68 */ u8 padA68[0xAB0 - 0xA68];
    /* 0xAB0 */ f32 unkAB0;
    /* 0xAB4 */ u8 padAB4[0xAC0 - 0xAB4];
    /* 0xAC0 */ f32 unkAC0;
    /* 0xAC4 */ u8 padAC4[4];
    /* 0xAC8 */ f32 eye[3];
    /* 0xAD4 */ f32 eyeOffset[3];
    /* 0xAE0 */ f32 lookOffset[3];
    /* 0xAEC */ u8 padAEC[0xB24 - 0xAEC];
    /* 0xB24 */ f32 unkB24;
    /* 0xB28 */ u8 padB28[0xB90 - 0xB28];
    /* 0xB90 */ f32 unkB90;
} ViewState;

typedef struct Orbit {
    /* 0x0 */ f32 angle;
    /* 0x4 */ f32 distance;
} Orbit;

#define MAX(a, b) ((a) < (b) ? (b) : (a))

extern f32 D_800AE308;
extern f32 D_800AE30C;

f32 func_002982F0(f32);             /* sin */
f32 func_002974C0(f32);             /* cos */
void func_00280C94(void *camera, s32 type, f32 a, f32 b, f32 x0, f32 y0, f32 x1, f32 y1, f32 z1,
                   f32 h, Quad quad, s32 region, f32 c, f32 d);

/* Points the camera at : in mode 1 from the view's offsets, in
 * mode 2 orbiting it, the buttons turning the orbit and moving it in or out. */
void func_0024CF58(ViewState *view, Target *target) {
    void *camera;
    f32 angle;
    f32 x;

    camera = view->camera;
    if (camera == NULL) {
        return;
    }
    if (view->mode == 1) {
        func_00280C94(camera, 0, target->angle, view->eye[0] + view->eyeOffset[0] - view->unkAB0,
                      view->eye[1] + view->eyeOffset[1], view->eye[2] + view->eyeOffset[2],
                      target->x + view->lookOffset[0], target->y, target->z + view->lookOffset[2],
                      MAX(view->lookOffset[1] + view->unkB24 - view->unkAC0, 40.96f), target->quad, target->region, view->unkB90, 3.0f);
    } else if (view->mode == 2) {
        if (view->buttons & 0x200) {
            D_800AE308 += 0.069813177f;
        } else if (view->buttons & 0x100) {
            D_800AE308 -= 0.069813177f;
        } else if (view->buttons & 0x800) {
            D_800AE30C -= 4.096f;
        } else if (view->buttons & 0x400) {
            D_800AE30C += 4.096f;
        }
        angle = target->angle + D_800AE308;
        x = target->x + D_800AE30C * func_002982F0(angle);
        func_00280C94(camera, 1, angle, 0.0f, 0.0f, 0.0f, x, target->y + 81.92f,
                      target->z + D_800AE30C * func_002974C0(angle), 0.0f, target->quad, target->region, 0.0f, 3.0f);
    }
}
