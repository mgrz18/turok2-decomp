/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0043FFAC.c, func_0043FFAC, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x00 */ s32 idx;
    /* 0x04 */ s16 kind;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ u32 flags;
} MenuItem;

/* Text style of an item class. */
typedef struct {
    /* 0x00 */ s32 mode;
    /* 0x04 */ f32 f4;
    /* 0x08 */ f32 f8;
    /* 0x0C */ f32 scaleX;
    /* 0x10 */ f32 scaleY;
    /* 0x14 */ u8 shadow;
    /* 0x15 */ u8 r0, g0, b0, r1, g1, b1;
    /* 0x1B */ u8 pad1B;
} MenuStyle;

typedef struct {
    /* 0x00 */ s32 selected;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ s32 x;
    /* 0x18 */ u8 pad18[0x1C - 0x18];
    /* 0x1C */ s32 y;
} MenuBox;

typedef struct {
    /* 0x00 */ u16 id;
    /* 0x02 */ u8 pad2[0x30 - 0x2];
    /* 0x30 */ f32 fade;
    /* 0x34 */ f32 alpha;
    /* 0x38 */ s32 visible;
    /* 0x3C */ s32 highlighted;
} MenuWindow;

/* The game state block around the multiplayer flag. */
typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 multiplayer; /* D_8011AAF1 */
    /* 0x002 */ u8 pad2[0x198 - 0x2];
    /* 0x198 */ s32 mode;       /* D_8011AC88 */
} GameState;

extern GameState D_8011AAF0;
extern MenuStyle D_800C1BD4[];
extern s32 D_800C1BD0; /* current draw mode */
extern s32 D_801101B0[];
extern s32 D_800C2030; /* screen width */
extern s32 D_800C2034; /* screen height */

extern s32 func_00266B80(void);
extern void func_00415CE8(MenuWindow *w, MenuItem *it, s32 c);
extern void func_00277F6C(void);
extern void func_0027836C(void);
extern void func_0027845C(void);
extern void COnScreen__SetFontColor(s32 r0, s32 g0, s32 b0, s32 r1, s32 g1, s32 b1);
extern void COnScreen__SetFontScale(f32 x, f32 y);
extern void func_00278CDC(char *text, s32 x, s32 y, s32 alpha, s32 centred, s32 shadow, f32 scaleX, f32 scaleY);

static __inline__ MenuStyle *MenuItem_Style(MenuItem *it) {
    s32 i;

    i = 0;
    switch (it->flags & 0x3FE0) {
        case 0x20:
            i = 0;
            break;
        case 0x40:
            i = 1;
            break;
        case 0x80:
            i = 2;
            break;
        case 0x100:
            i = 3;
            break;
        case 0x200:
            i = 4;
            break;
        case 0x400:
            i = 5;
            break;
        case 0x800:
            i = 6;
            break;
        case 0x1000:
            i = 7;
            break;
        case 0x2000:
            i = 8;
            break;
        default:
            i = 0;
            break;
    }
    return &D_800C1BD4[i];
}

/* Draws a menu item's text in its class's style, brighter when it is the selected one. */
void func_00413F74(MenuItem *it, MenuBox *b, void *c, MenuWindow *w, char *text) {
    u8 en;
    MenuStyle *E;
    f32 k;
    s32 r, g, bl, a, t, t2;

    en = 1;
    if ((D_8011AAF0.mode == 13 && (it->flags & 0x40000000)) ||
        (D_8011AAF0.multiplayer != 0 && (s32) it->flags < 0 && func_00266B80() == 0x400000)) {
        w->visible = 0;
    }
    if (it->flags & 0x08000000) {
        if (w->highlighted != 0) {
            w->visible = 0;
        }
        func_00415CE8(w, it, 1);
        func_00415CE8(w, it, 0);
        en = 0;
    }
    if (w->visible == 0) {
        en = 0;
    }
    E = MenuItem_Style(it);
    if (D_800C1BD0 != E->mode) {
        switch (E->mode) {
            case 0:
                func_00277F6C();
                break;
            case 1:
                func_00277F6C();
                func_0027836C();
                break;
            case 2:
                func_00277F6C();
                func_0027845C();
                break;
        }
        D_800C1BD0 = E->mode;
    }
    if (b->selected == (s16) w->id) {
        k = 1.25f;
    } else {
        k = !(it->flags & 0x01000000) ? 0.25f : 0.9f;
    }
    t = (s32) ((f32) E->r0 * k);
    r = 0xFF;
    if (t < 0x100) {
        r = t;
    }
    t = (s32) ((f32) E->g0 * k);
    g = 0xFF;
    if (t < 0x100) {
        g = t;
    }
    t = (s32) ((f32) E->b0 * k);
    bl = 0xFF;
    if (t < 0x100) {
        bl = t;
    }
    t = (s32) ((f32) E->r1 * k);
    a = 0xFF;
    if (t < 0x100) {
        a = t;
    }
    COnScreen__SetFontColor(r, g, bl, a, (t = (s32) ((f32) E->g1 * k)) < 0x100 ? t : 0xFF,
                            (t2 = (s32) ((f32) E->b1 * k)) < 0x100 ? t2 : 0xFF);
    D_801101B0[0] = E->shadow;
    D_801101B0[1] = E->shadow;
    COnScreen__SetFontScale(E->scaleX * (f32) D_800C2030 * 0.0035211267f, E->scaleY * (f32) D_800C2034 * 0.0045045046f);
    if (en != 0) {
        en = E->shadow;
    }
    func_00278CDC(text, b->x, b->y, (s32) (w->fade * (w->alpha * 255.0f)), 0, en, 1.0f, 1.0f);
}
