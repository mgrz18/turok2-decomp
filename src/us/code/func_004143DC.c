/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0044044C.c, func_0044044C, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ u32 flags;
    /* 0x0C */ u8 padC[0x14 - 0xC];
    /* 0x14 */ s32 image;
} MenuItem;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 width;
    /* 0x08 */ s32 height;
    /* 0x0C */ f32 scaleX;
    /* 0x10 */ f32 scaleY;
    /* 0x14 */ s32 x;
    /* 0x18 */ u8 pad18[0x1C - 0x18];
    /* 0x1C */ s32 y;
} MenuBox;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ f32 fade;
    /* 0x34 */ f32 alpha;
    /* 0x38 */ u8 pad38[0x3C - 0x38];
    /* 0x3C */ s32 highlighted;
} MenuWindow;

/* The game state block around the multiplayer flag. */
typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 multiplayer; /* D_8011AAF1 */
    /* 0x002 */ u8 pad2[0x198 - 0x2];
    /* 0x198 */ s32 mode;       /* D_8011AC88 */
    /* 0x19C */ u8 pad19C[0x1B4 - 0x19C];
    /* 0x1B4 */ s32 paused;     /* D_8011ACA4 */
} GameState;

extern GameState D_8011AAF0;
extern s32 D_800C1BD0; /* current draw mode */

extern s32 func_00266B80(void);
extern void func_0027AF4C(void);
extern void func_002778A8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j);
extern void func_00279028(s32 alpha);
extern void func_0027B320(s32 image, s32 b, s16 x, s16 y, f32 scaleX, f32 scaleY, s32 g);

/* Draws a menu image: a filled box when it is the focused item, the image otherwise. */
void func_004143DC(MenuItem *item, MenuBox *box, s32 unused, MenuWindow *window) {
    s32 alpha;
    f32 fx;
    f32 fy;
    f32 t1;
    f32 t2;

    if ((D_8011AAF0.mode == 13 && (item->flags & 0x40000000)) ||
        (D_8011AAF0.multiplayer != 0 && (s32) item->flags < 0 && func_00266B80() == 0x400000)) {
        window->highlighted = 1;
        if (D_800C1BD0 != 3) {
            D_800C1BD0 = 3;
        }
        func_0027AF4C();
        t1 = (f32) box->width * box->scaleX;
        t2 = (f32) box->height * box->scaleY;
        fy = (f32) box->y;
        fx = (f32) box->x;
        fx = fx + t1;
        fy = fy + t2;
        if (D_8011AAF0.paused != 0) {
            if (D_8011AAF0.multiplayer != 0) {
                if ((s32) item->flags < 0) {
                    alpha = 0xFF;
                } else {
                    alpha = 0xC0;
                }
            } else {
                alpha = 0x80;
            }
        } else {
            alpha = 0x80;
        }
        func_002778A8(box->x, box->y, (s32) fx, (s32) fy, 1, 0, 0, 0, 0, alpha);
    } else {
        if (D_800C1BD0 != 3) {
            D_800C1BD0 = 3;
            func_00279028((s32) (window->fade * (window->alpha * 255.0f)));
        }
        func_0027B320(item->image, 0, (s16) box->x, (s16) box->y, box->scaleX, box->scaleY, 1);
    }
}
