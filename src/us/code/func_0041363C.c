/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0043F69C.c, func_0043F69C, with Turok 2's layouts. */
#include "common.h"

/* 0x28-byte menu item, also copied by value to lay out its parts. */
typedef struct MenuItem {
    /* 0x00 */ s32 idx;
    /* 0x04 */ s16 kind;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ u32 flags;
    /* 0x0C */ s16 x;
    /* 0x0E */ s16 y;
    /* 0x10 */ u8 b10, b11, b12, b13;
    /* 0x14 */ s32 data;
    /* 0x18 */ void *def;
    /* 0x1C */ s32 value;
    /* 0x20 */ void *p20;
    /* 0x24 */ void *owner;
} MenuItem;

/* Layout of an item. */
typedef struct {
    /* 0x00 */ s32 f0;
    /* 0x04 */ s32 width;
    /* 0x08 */ s32 height;
    /* 0x0C */ f32 scaleX;
    /* 0x10 */ f32 scaleY;
    /* 0x14 */ s32 x0, x1, y0, y1;
} MenuInfo;

/* Text style of an item class. */
typedef struct {
    /* 0x00 */ s32 mode;
    /* 0x04 */ s32 charWidth;
    /* 0x08 */ s32 lineHeight;
    /* 0x0C */ u8 padC[0x1C - 0xC];
} MenuStyle;

typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ void *ctx;
} MenuPanel;

typedef struct {
    /* 0x000 */ u8 pad0[0x288];
    /* 0x288 */ f32 width;
    /* 0x28C */ f32 height;
} MenuCamera;

typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 width;
    /* 0x28 */ s32 height;
} MenuImage;

/* The game state block around the multiplayer flag. */
typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 multiplayer; /* D_8011AAF1 */
    /* 0x002 */ u8 pad2[0x198 - 0x2];
    /* 0x198 */ s32 mode;       /* D_8011AC88 */
} GameState;

extern GameState D_8011AAF0;
extern MenuStyle D_800C1BD4[];
extern s32 D_800C2030; /* screen width */
extern s32 D_800C2034; /* screen height */

extern s32 func_00266B80(void);
extern MenuCamera *func_00416B28(void *ctx);
extern u8 *func_00413230(MenuItem *item);
extern f32 func_004163C0(u8 *text, f32 a, f32 b);
extern void func_0027AEA8(s32 image, s32 b, s32 *width, s32 *height);

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

/* Measures a menu item and places it on screen. */
void func_0041363C(MenuItem *it, MenuInfo *o) {
    MenuCamera *cam;
    MenuStyle *E;
    MenuItem cp;
    MenuInfo a, b, c;
    u8 *p;
    MenuImage *sub;
    s32 n, n2;
    f32 fs, v, fa, fb, vt, vt2;

    cam = func_00416B28(((MenuPanel *) it->owner)->ctx);
    o->scaleY = 1.0f;
    o->scaleX = 1.0f;
    switch (it->kind) {
        case 0:
        case 5:
            E = MenuItem_Style(it);
            if (E->mode == 0) {
                fs = E->charWidth;
                if (it->kind == 5) {
                    p = func_00413230(it);
                } else {
                    p = (u8 *) it->data;
                }
                v = func_004163C0(p, fs, fs);
                o->width = v;
            } else {
                n = 0;
                p = (it->kind == 5 ? func_00413230(it) : (u8 *) it->data);
                for (; p != NULL && *p != 0 && *p != 10; p++) {
                    n++;
                }
                o->width = n * E->charWidth;
            }
            if (it->flags & 0x08000000) {
                o->height = (f32) D_800C2034 * 0.10810811f;
            } else if (E->mode == 2 && D_800C2034 >= 0xDF) {
                o->height = ((f32) E->lineHeight + 0.5f) * (f32) D_800C2034 * 0.0045045046f;
            } else {
                o->height = (E->lineHeight * D_800C2034) / 222;
            }
            o->width = (o->width * D_800C2030) / 284;
            break;
        case 4:
            E = MenuItem_Style(it);
            o->width = E->charWidth;
            o->height = (E->lineHeight * D_800C2034) / 222;
            break;
        case 1:
            if ((D_8011AAF0.mode == 13 && (it->flags & 0x40000000)) ||
                (D_8011AAF0.multiplayer != 0 && (s32) it->flags < 0 && func_00266B80() == 0x400000)) {
                o->width = 0x11C;
                o->height = 0xDE;
            } else {
                func_0027AEA8(it->data, 0, &o->width, &o->height);
            }
            if ((it->flags & 0x10000000) && (f32) o->width != 0.0f && (f32) o->height != 0.0f) {
                fa = cam->width / (f32) o->width;
                fb = cam->height / (f32) o->height;
            } else {
                fa = (f32) D_800C2030 * 0.0035211267f;
                fb = (f32) D_800C2034 * 0.0045045046f;
            }
            o->scaleX = fa;
            o->scaleY = fb;
            break;
        case 2:
            a = *o;
            cp = *it;
            cp.kind = 0;
            func_0041363C(&cp, &a);
            b = *o;
            cp = *it;
            cp.kind = 1;
            cp.data = 0;
            func_0041363C(&cp, &b);
            c = *o;
            cp = *it;
            cp.kind = 1;
            cp.data = 0;
            func_0041363C(&cp, &c);
            n2 = b.width + c.width;
            if (n2 < a.width) {
                n2 = a.width;
            }
            o->width = n2;
            o->height = a.height + b.height + ((c.height - b.height) / 2);
            break;
        case 3:
            sub = (MenuImage *) it->data;
            o->width = ((f32) sub->width * cam->width) / (f32) D_800C2030;
            o->height = ((f32) sub->height * cam->height) / (f32) D_800C2034;
            break;
        default:
            o->width = 1;
            o->height = 1;
            break;
    }
    vt = (f32) ((it->x * D_800C2030) / 284);
    if (!(it->flags & 0x8000)) {
        if (it->flags & 0x4000) {
            o->x0 = (f32) o->x1 + vt;
        } else {
            o->x0 = vt;
        }
    }
    o->x1 = (f32) o->x0 + (f32) o->width * o->scaleX;
    vt2 = (f32) ((it->y * D_800C2034) / 222);
    if (!(it->flags & 0x20000)) {
        if (it->flags & 0x10000) {
            o->y0 = (f32) o->y1 + vt2;
        } else {
            o->y0 = vt2;
        }
    }
    o->y1 = (f32) o->y0 + (f32) o->height * o->scaleY;
}
