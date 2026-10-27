#include "common.h"

typedef struct Viewport {
    /* 0x0 */ f32 scaleX;
    /* 0x4 */ f32 scaleY;
    /* 0x8 */ f32 offsetX;
    /* 0xC */ f32 offsetY;
} Viewport;

typedef struct View {
    /* 0x000 */ u8 pad0[0x288];
    /* 0x288 */ Viewport vp;
    /* 0x298 */ u8 pad298[0x328 - 0x298];
    /* 0x328 */ f32 normal[3];      /* view plane: points in front have dot <= dist */
    /* 0x334 */ f32 dist;
} View;

extern s32 D_800C2030;      /* screen width */
extern s32 D_800C2034;      /* screen height */
extern u8 D_8011AAF1;

void func_002810EC(View *, f32 *, f32 *, f32 *);
void func_0027B320(s32, s32, s16, s16, f32, f32, s32);

/* Draws a sprite at a world position projected to the screen (or centred
 * for the HUD sprites 0x2BC, 0x2DA and 0x2E4). */
void func_0024A678(View *view, f32 *pos, s32 sprite, f32 frame, f32 scale, f32 size) {
    f32 x;
    f32 y;
    f32 half;

    if (sprite == 0x2DA || sprite == 0x2E4 || sprite == 0x2BC) {
        x = view->vp.offsetX + view->vp.scaleX * 0.5f;
        y = view->vp.offsetY + view->vp.scaleY * 0.5f;
    } else {
        if (!(view->normal[0] * pos[0] + view->normal[1] * pos[1] + view->normal[2] * pos[2] <= view->dist)) {
            return;
        }
        func_002810EC(view, pos, &x, &y);
        if (x < -64.0f || (f32) (D_800C2030 + 64) < x || y < -64.0f || (f32) (D_800C2034 + 64) < y) {
            return;
        }
        if (D_8011AAF1 != 0) {
            x = view->vp.offsetX + x / D_800C2030 * view->vp.scaleX;
            y = view->vp.offsetY + y / D_800C2034 * view->vp.scaleY;
        }
    }
    half = scale * size - 1.0f;
    func_0027B320(sprite, frame, x - half, y - half, size, size, 1);
}
