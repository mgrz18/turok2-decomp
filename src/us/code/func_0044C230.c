/* The second VM module's copy of src/us/code/func_00413230.c, byte for byte,
 * at the same link address (named 0x44C230-0x450484, linked 0x413230-0x417484):
 * the same source, with the functions under their second-module names. Its
 * .rodata goes NOLOAD over the first module's (versions/shared_rodata.us.txt). */

/* One file, 0x413230-0x417484, .rodata 0x800C0AF0-0x800C0E64: the menu
 * code from func_0044C230 to func_00450418. Objects start 16-aligned in
 * .text and in .rodata, with junk words before: stray_00413228 opens it,
 * stray_00417484 closes it, and its pool opens after junk at 0x800C0AE8 and
 * ends with junk at 0x800C0E64, before the next object's at 0x800C0E70.
 * The pools run on from one function to the next (func_0044E6FC's double at
 * 0x800C0C28 is 8-aligned only counted from 0x800C0AF0), and the string
 * "menu" (0x800C0C54) is one constant for func_0044EFB8, func_0044F644,
 * func_0044F760, func_0044F894 and func_004502F8.
 *
 * Each part was matched alone first and keeps its own types; names two
 * parts both define carry the part's address as a suffix. The second
 * module holds this file byte for byte at the same link address
 * (func_0044C230.c). */
#include "common.h"
#include "rw/abu_types.h"

/* ---------------- func_0044C230 ---------------- */
/* WRONG CUT: splat's func_0041321C (0x41321C-0x4133A4) is the shared return of func_004131B0 (3 words), a padding nop,
 * one junk word (0x41322C) and this function, which really starts at 0x413230 (0x174 bytes). */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0043F290.c, func_0043F290, with Turok 2's layouts. */

struct MenuItem_413230;

typedef struct MenuBinding_413230 {
    /* 0x00 */ char *name;
    /* 0x04 */ s32 type; /* 0 f32, 1/2/5 s32, 3 s8, 4 s16, 6 u8, 7 u16, 8 u32 */
    /* 0x08 */ f32 min;
    /* 0x0C */ f32 max;
    /* 0x10 */ f32 step;
    /* 0x14 */ f32 scale;
    /* 0x18 */ void *(*storage)(struct MenuItem_413230 *item);
} MenuBinding_413230;

typedef struct MenuItem_413230 {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ MenuBinding_413230 *binding;
} MenuItem_413230;

extern s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */
extern char D_801308A0[];
extern char D_80130860[];

/* Reads the item's bound variable as a float, whatever its type. */
static __inline__ f32 MenuItem_GetValue(MenuItem_413230 *item) {
    MenuBinding_413230 *binding = item->binding;
    void *p = binding->storage(item);

    switch (binding->type) {
        default:
            return 0.0f;
        case 0:
            return *(f32 *) p;
        case 3:
            return (f32) *(s8 *) p;
        case 4:
            return (f32) *(s16 *) p;
        case 1:
        case 2:
        case 5:
            return (f32) *(s32 *) p;
        case 6:
            return (f32) *(u8 *) p;
        case 7:
            return (f32) *(u16 *) p;
        case 8:
            return (f32) *(u32 *) p;
    }
}

/* Formats the item's value (scaled) after its name. */
char *func_0044C230(MenuItem_413230 *item) {
    MenuBinding_413230 *binding = item->binding;
    f32 value = MenuItem_GetValue(item);

    value = value / binding->scale;
    func_0029E3E0(D_801308A0, "%3.2f", (f64) value);
    if (binding->name) {
        func_0029E3E0(D_80130860, "%s %s", binding->name, D_801308A0);
    } else {
        func_0029E3E0(D_80130860, "%s", D_801308A0);
    }
    return D_80130860;
}

/* ---------------- func_0044C3A4 ---------------- */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0043F404.c, func_0043F404, with Turok 2's layouts. */
/* A menu slider: step its bound variable (any numeric type) with left/right. */

struct MenuItem_4133A4;

typedef struct MenuBinding_4133A4 {
    /* 0x00 */ char *name;
    /* 0x04 */ s32 type;            /* 0 f32, 1/2/5 s32, 3 s8, 4 s16, 6 u8, 7 u16, 8 u32 */
    /* 0x08 */ f32 min;
    /* 0x0C */ f32 max;
    /* 0x10 */ f32 step;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ void *(*storage)(struct MenuItem_4133A4 *item);
} MenuBinding_4133A4;

typedef struct MenuItem_4133A4 {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ MenuBinding_4133A4 *binding;
} MenuItem_4133A4;

typedef struct Menu_4133A4 {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s32 controller;
} Menu_4133A4;

extern s32 func_00285628(s32 controller);   /* left pressed */
extern s32 func_00285670(s32 controller);   /* right pressed */

static __inline__ f32 bindingGet(MenuItem_4133A4 *item) {
    MenuBinding_4133A4 *b = item->binding;
    void *p = b->storage(item);

    switch (b->type) {
        default: return 0.0f;
        case 0: return *(f32 *)p;
        case 3: return *(s8 *)p;
        case 4: return *(s16 *)p;
        case 1: case 2: case 5: return *(s32 *)p;
        case 6: return *(u8 *)p;
        case 7: return *(u16 *)p;
        case 8: return *(u32 *)p;
    }
}

static __inline__ void bindingSet(MenuItem_4133A4 *item, f32 value) {
    MenuBinding_4133A4 *b = item->binding;
    void *p = b->storage(item);

    switch (b->type) {
        case 0: *(f32 *)p = value; return;
        case 3: *(s8 *)p = (s32)value; return;
        case 4: *(s16 *)p = (s32)value; return;
        case 1: case 2: case 5: *(s32 *)p = (s32)value; return;
        case 6: *(u8 *)p = (u32)value; return;
        case 7: *(u16 *)p = (u32)value; return;
        case 8: *(u32 *)p = (u32)value; return;
    }
}

s32 func_0044C3A4(MenuItem_4133A4 *item, Menu_4133A4 *menu) {
    MenuBinding_4133A4 *b = item->binding;
    f32 value;

    value = bindingGet(item);
    if (func_00285628(menu->controller)) {
        value -= b->step;
        if (value < b->min) {
            value = b->min;
        }
    } else if (func_00285670(menu->controller)) {
        value += b->step;
        if (b->max < value) {
            value = b->max;
        }
    }
    bindingSet(item, value);
    return 0;
}

/* ---------------- func_0044C63C ---------------- */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0043F69C.c, func_0043F69C, with Turok 2's layouts. */

/* 0x28-byte menu item, also copied by value to lay out its parts. */
typedef struct MenuItem_41363C {
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
} MenuItem_41363C;

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
} MenuStyle_41363C;

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
} GameState_41363C;

extern GameState_41363C D_8011AAF0;
extern MenuStyle_41363C D_800C1BD4[];
extern s32 D_800C2030; /* screen width */
extern s32 D_800C2034; /* screen height */

extern s32 func_00266B80(void);
extern MenuCamera *func_00416B28_41363C(void *ctx) __asm__("func_0044FB28");
extern u8 *func_00413230_41363C(MenuItem_41363C *item) __asm__("func_0044C230");
extern f32 func_004163C0_41363C(u8 *text, f32 a, f32 b) __asm__("func_0044F3C0");
extern void func_0027AEA8(s32 image, s32 b, s32 *width, s32 *height);

static __inline__ MenuStyle_41363C *MenuItem_Style_41363C(MenuItem_41363C *it) {
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
void func_0044C63C(MenuItem_41363C *it, MenuInfo *o) {
    MenuCamera *cam;
    MenuStyle_41363C *E;
    MenuItem_41363C cp;
    MenuInfo a, b, c;
    u8 *p;
    MenuImage *sub;
    s32 n, n2;
    f32 fs, v, fa, fb, vt, vt2;

    cam = func_00416B28_41363C(((MenuPanel *) it->owner)->ctx);
    o->scaleY = 1.0f;
    o->scaleX = 1.0f;
    switch (it->kind) {
        case 0:
        case 5:
            E = MenuItem_Style_41363C(it);
            if (E->mode == 0) {
                fs = E->charWidth;
                if (it->kind == 5) {
                    p = func_00413230_41363C(it);
                } else {
                    p = (u8 *) it->data;
                }
                v = func_004163C0_41363C(p, fs, fs);
                o->width = v;
            } else {
                n = 0;
                p = (it->kind == 5 ? func_00413230_41363C(it) : (u8 *) it->data);
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
            E = MenuItem_Style_41363C(it);
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
            func_0044C63C(&cp, &a);
            b = *o;
            cp = *it;
            cp.kind = 1;
            cp.data = 0;
            func_0044C63C(&cp, &b);
            c = *o;
            cp = *it;
            cp.kind = 1;
            cp.data = 0;
            func_0044C63C(&cp, &c);
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

/* ---------------- func_0044CF74 ---------------- */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0043FFAC.c, func_0043FFAC, with Turok 2's layouts. */

typedef struct {
    /* 0x00 */ s32 idx;
    /* 0x04 */ s16 kind;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ u32 flags;
} MenuItem_413F74;

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
} MenuStyle_413F74;

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
} GameState_413F74;

extern GameState_413F74 D_8011AAF0_413F74 __asm__("D_8011AAF0");
extern MenuStyle_413F74 D_800C1BD4_413F74[] __asm__("D_800C1BD4");
extern s32 D_800C1BD0; /* current draw mode */
extern s32 D_801101B0[];
extern s32 D_800C2030; /* screen width */
extern s32 D_800C2034; /* screen height */

extern s32 func_00266B80(void);
extern void func_00415CE8_413F74(MenuWindow *w, MenuItem_413F74 *it, s32 c) __asm__("func_0044ECE8");
extern void func_00277F6C(void);
extern void func_0027836C(void);
extern void func_0027845C(void);
extern void COnScreen__SetFontColor(s32 r0, s32 g0, s32 b0, s32 r1, s32 g1, s32 b1);
extern void COnScreen__SetFontScale(f32 x, f32 y);
extern void func_00278CDC(char *text, s32 x, s32 y, s32 alpha, s32 centred, s32 shadow, f32 scaleX, f32 scaleY);

static __inline__ MenuStyle_413F74 *MenuItem_Style_413F74(MenuItem_413F74 *it) {
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
    return &D_800C1BD4_413F74[i];
}

/* Draws a menu item's text in its class's style, brighter when it is the selected one. */
void func_0044CF74(MenuItem_413F74 *it, MenuBox *b, void *c, MenuWindow *w, char *text) {
    u8 en;
    MenuStyle_413F74 *E;
    f32 k;
    s32 r, g, bl, a, t, t2;

    en = 1;
    if ((D_8011AAF0_413F74.mode == 13 && (it->flags & 0x40000000)) ||
        (D_8011AAF0_413F74.multiplayer != 0 && (s32) it->flags < 0 && func_00266B80() == 0x400000)) {
        w->visible = 0;
    }
    if (it->flags & 0x08000000) {
        if (w->highlighted != 0) {
            w->visible = 0;
        }
        func_00415CE8_413F74(w, it, 1);
        func_00415CE8_413F74(w, it, 0);
        en = 0;
    }
    if (w->visible == 0) {
        en = 0;
    }
    E = MenuItem_Style_413F74(it);
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

/* ---------------- func_0044D3DC ---------------- */
/* func_0044D3DC to func_0044E6FC were matched first as one file of their
 * own (0x4143DC-0x415CE8, .rodata from 0x800C0BE8); their types carry the
 * address of the part that matched them. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0044044C.c, func_0044044C, with Turok 2's layouts. */

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ u32 flags;
    /* 0x0C */ u8 padC[0x14 - 0xC];
    /* 0x14 */ s32 image;
} MenuItem_4143DC;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 width;
    /* 0x08 */ s32 height;
    /* 0x0C */ f32 scaleX;
    /* 0x10 */ f32 scaleY;
    /* 0x14 */ s32 x;
    /* 0x18 */ u8 pad18[0x1C - 0x18];
    /* 0x1C */ s32 y;
} MenuBox_4143DC;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ f32 fade;
    /* 0x34 */ f32 alpha;
    /* 0x38 */ u8 pad38[0x3C - 0x38];
    /* 0x3C */ s32 highlighted;
} MenuWindow_4143DC;

/* The game state block around the multiplayer flag. */
typedef struct {
    /* 0x000 */ u8 pad0;
    /* 0x001 */ u8 multiplayer; /* D_8011AAF1 */
    /* 0x002 */ u8 pad2[0x198 - 0x2];
    /* 0x198 */ s32 mode;       /* D_8011AC88 */
    /* 0x19C */ u8 pad19C[0x1B4 - 0x19C];
    /* 0x1B4 */ s32 paused;     /* D_8011ACA4 */
} GameState_4143DC;

extern GameState_4143DC D_8011AAF0_4143DC __asm__("D_8011AAF0");
extern s32 D_800C1BD0_4143DC __asm__("D_800C1BD0"); /* current draw mode */

extern s32 func_00266B80(void);
extern void func_0027AF4C(); /* called with and without an argument */
extern void func_002778A8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j);
extern void func_00279028(s32 alpha);
extern void func_0027B320(s32 image, s32 b, s16 x, s16 y, f32 scaleX, f32 scaleY, s32 g);

/* Draws a menu image: a filled box when it is the focused item, the image otherwise. */
void func_0044D3DC(MenuItem_4143DC *item, MenuBox_4143DC *box, s32 unused, MenuWindow_4143DC *window) {
    s32 alpha;
    f32 fx;
    f32 fy;
    f32 t1;
    f32 t2;

    if ((D_8011AAF0_4143DC.mode == 13 && (item->flags & 0x40000000)) ||
        (D_8011AAF0_4143DC.multiplayer != 0 && (s32) item->flags < 0 && func_00266B80() == 0x400000)) {
        window->highlighted = 1;
        if (D_800C1BD0_4143DC != 3) {
            D_800C1BD0_4143DC = 3;
        }
        func_0027AF4C();
        t1 = (f32) box->width * box->scaleX;
        t2 = (f32) box->height * box->scaleY;
        fy = (f32) box->y;
        fx = (f32) box->x;
        fx = fx + t1;
        fy = fy + t2;
        if (D_8011AAF0_4143DC.paused != 0) {
            if (D_8011AAF0_4143DC.multiplayer != 0) {
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
        if (D_800C1BD0_4143DC != 3) {
            D_800C1BD0_4143DC = 3;
            func_00279028((s32) (window->fade * (window->alpha * 255.0f)));
        }
        func_0027B320(item->image, 0, (s16) box->x, (s16) box->y, box->scaleX, box->scaleY, 1);
    }
}


/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00440618.c, func_00440618, with Turok 2's layouts. */

/* 0x28-byte menu item, also copied by value to lay out its parts. */
typedef struct MenuItem_4145B0 {
    /* 0x00 */ s32 idx;
    /* 0x04 */ s16 kind;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ u32 flags;
    /* 0x0C */ u16 x;
    /* 0x0E */ u16 y;
    /* 0x10 */ u8 b10, b11, b12, b13;
    /* 0x14 */ s32 data;
    /* 0x18 */ void *def;
    /* 0x1C */ s32 value; /* slider position, 0-256 */
    /* 0x20 */ void *p20;
    /* 0x24 */ void *owner;
} MenuItem_4145B0;

/* Layout of an item (func_0041363C_4143DC). */
typedef struct {
    /* 0x00 */ s32 f0;
    /* 0x04 */ s32 width;
    /* 0x08 */ s32 height;
    /* 0x0C */ f32 scaleX;
    /* 0x10 */ f32 scaleY;
    /* 0x14 */ s32 x0, x1, y0, y1;
    /* 0x24 */ s32 pad24;
} MenuInfo_4145B0;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 x;
    /* 0x18 */ u8 pad18[0x1C - 0x18];
    /* 0x1C */ s32 y;
} MenuBox_4145B0;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 left;
    /* 0x18 */ s32 right;
} MenuColumn_4145B0;

extern void func_0041363C_4143DC(MenuItem_4145B0 *item, MenuInfo_4145B0 *info) __asm__("func_0044C63C");
extern void func_00413F74_4143DC(MenuItem_4145B0 *item, MenuBox_4145B0 *box, MenuColumn_4145B0 *column, s32 window, s32 data) __asm__("func_0044CF74");

static __inline__ void fin_4145B0(MenuItem_4145B0 *item, MenuBox_4145B0 *box, MenuColumn_4145B0 *column, s32 window) {
    func_00413F74_4143DC(item, box, column, window, item->data);
}

/* Draws a slider: its label, its bar and its knob at the item's value. */
void func_0044D5B0(MenuItem_4145B0 *it, MenuBox_4145B0 *box, MenuColumn_4145B0 *column, s32 window) {
    MenuItem_4145B0 cp;
    MenuInfo_4145B0 o1;
    MenuInfo_4145B0 o2;
    MenuInfo_4145B0 o3;
    s32 mid;
    s32 t;

    mid = (column->left + column->right) / 2;
    cp = *it;
    cp.kind = 0;
    func_0041363C_4143DC(&cp, &o1);
    cp = *it;
    cp.kind = 1;
    cp.data = 0;
    func_0041363C_4143DC(&cp, &o2);
    cp = *it;
    cp.kind = 1;
    cp.data = 0;
    func_0041363C_4143DC(&cp, &o3);
    cp = *it;
    cp.data = 0;
    box->y = box->y + o1.height;
    if (it->flags & 1) {
        box->x = mid - ((o2.width + o3.width) / 2);
    }
    box->x = box->x + o3.width / 2;
    func_0044D3DC((void *) &cp, (void *) box, (s32) column, (void *) window);
    cp = *it;
    cp.data = 0;
    t = it->value * o2.width;
    box->x = box->x + (t / 256 - o3.width / 2);
    box->y = box->y + (o2.height / 2 - o3.height / 2);
    func_0044D3DC((void *) &cp, (void *) box, (s32) column, (void *) window);
    cp = *it;
    if (it->flags & 1) {
        box->x = mid - o1.width / 2;
    }
    fin_4145B0(&cp, box, column, window);
}


/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004409A8.c, func_004409A8, with Turok 2's layouts. */

typedef struct {
    f32 x, y, z;
} Vec3f_414938;

typedef struct {
    u32 w0, w1;
} GfxWords_414938;

typedef struct {
    f32 m[16];
} Mtx4f_414938;

typedef struct {
    /* 0x000 */ u8 pad0[0x28];
    /* 0x028 */ f32 yaw;
    /* 0x02C */ u8 pad2C[0x14C - 0x2C];
    /* 0x14C */ u8 view[0x288 - 0x14C];
    /* 0x288 */ f32 width;
    /* 0x28C */ f32 height;
    /* 0x290 */ u8 pad290[0x368 - 0x290];
    /* 0x368 */ Mtx4f_414938 mtx[2];
} MenuCamera_414938;

typedef struct {
    /* 0x00 */ s16 id;
    /* 0x02 */ u8 pad2[0x30 - 0x2];
    /* 0x30 */ f32 fade;
    /* 0x34 */ f32 alpha;
    /* 0x38 */ u8 pad38[0x40 - 0x38];
    /* 0x40 */ MenuCamera_414938 *cam;
} MenuWindow_414938;

/* Where and what to show in a model item. */
typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ s32 model;
    /* 0x08 */ s32 variant;
    /* 0x0C */ Vec3f_414938 pos;
    /* 0x18 */ Vec3f_414938 scale;
    /* 0x24 */ u8 pad24[0x2C - 0x24];
    /* 0x2C */ f32 spin;
} MenuModelPart_414938;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s16 h0C;
} ModelSub_414938;

/* The scene object a model item draws. */
typedef struct {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3f_414938 pos;
    /* 0x010 */ u8 pad10[0x14 - 0x10];
    /* 0x014 */ ModelSub_414938 *sub;
    /* 0x018 */ u8 pad18[0x34 - 0x18];
    /* 0x034 */ Vec3f_414938 scale;
    /* 0x040 */ u8 pad40[0x50 - 0x40];
    /* 0x050 */ f32 rotY;
    /* 0x054 */ u8 pad54[0x194 - 0x54];
    /* 0x194 */ Vec3f_414938 pos194;
    /* 0x1A0 */ u8 pad1A0[0x234 - 0x1A0];
    /* 0x234 */ void *draw;
} MenuFx_414938;

typedef struct {
    s32 w[0x27];
} ModelInfo_414938; /* 0x9C bytes */

typedef struct {
    /* 0x000 */ s32 id;
    /* 0x004 */ s32 f4;
    /* 0x008 */ s32 f8;
    /* 0x00C */ f32 fC;
    /* 0x010 */ u8 pad10[0x18 - 0x10];
    /* 0x018 */ s32 f18;
    /* 0x01C */ s32 f1C;
    /* 0x020 */ s32 f20;
    /* 0x024 */ u8 pad24[0x34 - 0x24];
    /* 0x034 */ f32 height;
    /* 0x038 */ u8 pad38[0xA0 - 0x38];
    /* 0x0A0 */ MenuFx_414938 fx;
    /* 0x2D4 */ u8 pad2D4[0x328 - 0xA0 - sizeof(MenuFx_414938)];
    /* 0x328 */ s32 loaded;
    /* 0x32C */ f32 drawHeight;
} MenuModel_414938;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ u32 flags;
    /* 0x0C */ u8 padC[0x14 - 0xC];
    /* 0x14 */ MenuModelPart_414938 *part;
    /* 0x18 */ u8 pad18[0x20 - 0x18];
    /* 0x20 */ MenuModel_414938 *model;
} MenuItem_414938;

typedef struct {
    s32 id;
} MenuSelection_414938;

extern GfxWords_414938 *D_800D8E24;
extern s32 D_800B6D1C;
extern s32 D_800B6D20; /* frame counter */
extern s32 D_800B6D00;
extern f32 D_800B6D28; /* frame time scale */
extern s32 D_800C2030; /* screen width */
extern s32 D_800C2034; /* screen height */
extern s32 D_800C1BD0_4143DC __asm__("D_800C1BD0"); /* current draw mode */
extern s32 D_801308E0;
extern u8 D_800F7078[];
extern s32 D_800ADB30[];

extern void func_00266C5C(void *p, s32 c, s32 n); /* memset */
extern f32 func_002982F0(f32 angle);             /* sin */
extern ModelInfo_414938 *func_00225EBC(void *world, s32 model, s32 variant);
extern void func_0020F85C(void *m, Vec3f_414938 *in, Vec3f_414938 *out);
extern void func_00219BC0(MenuFx_414938 *fx, s32 a, s32 b, s32 c, void *d, s32 e, s32 f, f32 g, Vec3f_414938 h, s32 i, Vec3f_414938 j,
                          Vec3f_414938 k, s32 l);
extern void func_0021E5D4(MenuFx_414938 *fx);
extern void func_0021EBC0(MenuFx_414938 *fx);
extern void func_0021A344(MenuFx_414938 *fx);
extern s32 func_00206078(void);
extern void func_00206068(s32 a);
extern void func_002241D8(void *world, MenuFx_414938 *fx, s32 c, s32 d);
extern void func_0020B5D4(void);
extern void func_0020B624(void);
extern void func_0021D39C(MenuFx_414938 *fx, MenuCamera_414938 *cam);
extern void func_004171D0_4143DC(void) __asm__("func_004501D0");
extern void func_004171F4_4143DC(void) __asm__("func_004501F4");

/* Draws a menu item's 3D model, loading it the first time. */
void func_0044D938(MenuItem_414938 *o, MenuSelection_414938 *idp, s32 unused, MenuWindow_414938 *w) {
    Vec3f_414938 v50;
    Vec3f_414938 v60;
    Vec3f_414938 v70;
    Vec3f_414938 v80;
    MenuModel_414938 *x;
    MenuModel_414938 *x2;
    MenuCamera_414938 *c;
    MenuModelPart_414938 *p;
    MenuFx_414938 *fx;
    ModelInfo_414938 *src;
    void *s3;
    s32 s2;
    GfxWords_414938 *d;
    f32 k;
    f32 k2;
    f32 tt;
    f32 ta, tb;

    func_00266C5C(&v50, 0, 12);
    if (o->model != NULL) {
        x = o->model;
        p = o->part;
        fx = &x->fx;
        if (x->loaded != 0) {
            {
                GfxWords_414938 **pp = &D_800D8E24;
                d = (*pp)++;
                d->w0 = 0xDA380007;
                d->w1 = (u32) &w->cam->mtx[D_800B6D1C];
            }
            c = w->cam;
            k = c->width / (f32) D_800C2030;
            k2 = c->height / (f32) D_800C2034;
            v80.x = p->scale.x * k;
            v80.z = p->scale.z * k;
            v80.y = p->scale.y * k2;
            x->drawHeight = x->height;
            s3 = NULL;
            if (!(o->flags & 0x2000000)) {
                s2 = p->model;
                x2 = o->model;
                src = func_00225EBC(D_800F7078, s2, p->variant);
                if (src == NULL) {
                    x2->loaded = 0;
                } else {
                    x2->id = s2;
                    x2->loaded = 1;
                    *(ModelInfo_414938 *) &x2->f4 = *src;
                    s3 = &x2->f4;
                    x2->f4 = 0;
                    x2->f8 = 0;
                    x2->f18 = 0;
                    x2->f1C = 0;
                    x2->f20 = 0;
                    x2->fC = 10240.0f;
                    x2->drawHeight = x2->height;
                }
            }
            v60 = p->pos;
            v60.y = v60.y - x->drawHeight * v80.y * 0.8f;
            func_0020F85C(w->cam->view, &v60, &v70);
            if (s3 != NULL && !(o->flags & 0x2000000)) {
                func_00219BC0(fx, -1, p->type, D_800B6D00, s3, -1, 0, w->cam->yaw + 3.141593f, v70, 0, p->scale, v50,
                              0);
                func_0021E5D4(fx);
                o->flags = o->flags | 0x2000000;
            }
            tt = func_002982F0((f32) (u32) (D_800B6D20 * 10) * 0.017453294f);
            ta = w->fade;
            tb = w->alpha;
            D_801308E0 = (s32) (ta * tb * ((tt + 1.0f) * 50.0f + 50.0f));
            if (w->id == idp->id) {
                fx->draw = (void *) func_004171F4_4143DC;
            } else {
                fx->draw = (void *) func_004171D0_4143DC;
            }
            if (func_00206078() == 0) {
                func_00206068(1);
            }
            if (x->id == 0xB) {
                func_002241D8(D_800F7078, fx, D_800ADB30[fx->sub->h0C], 1);
            }
            if (D_800C1BD0_4143DC != 4) {
                D_800C1BD0_4143DC = 4;
                {
                    GfxWords_414938 **pp = &D_800D8E24;
                    GfxWords_414938 *q = (*pp)++;
                    q->w0 = 0xE7000000;
                    q->w1 = 0;
                }
                {
                    GfxWords_414938 **pp = &D_800D8E24;
                    GfxWords_414938 *q = (*pp)++;
                    q->w0 = 0xE3000A01;
                    q->w1 = 0x100000;
                }
                {
                    GfxWords_414938 **pp = &D_800D8E24;
                    GfxWords_414938 *q = (*pp)++;
                    q->w0 = 0xE3000C00;
                    q->w1 = 0x80000;
                }
                {
                    GfxWords_414938 **pp = &D_800D8E24;
                    GfxWords_414938 *q = (*pp)++;
                    q->w0 = 0xE3001201;
                    q->w1 = 0x2000;
                }
            }
            fx->scale = v80;
            fx->pos = v70;
            fx->pos194 = v70;
            func_0021EBC0(fx);
            fx->pos = v70;
            fx->pos194 = v70;
            func_0021A344(fx);
            fx->rotY = fx->rotY + p->spin * D_800B6D28;
            fx->pos = v70;
            fx->pos194 = v70;
            func_0020B5D4();
            func_0021D39C(fx, w->cam);
            func_0020B624();
        }
    }
}


/* ---- func_0044DEA8 ---- */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00440F10.c, func_00440F10.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct { s16 type; u8 pad2[2]; u32 flags; u16 w8; u16 wA; u8 bC, bD, bE, bF; s32 f10; u8 pad14[0x24 - 0x14]; } Q04Tpl;
typedef struct { void *unk0; s16 n; u8 pad6[2]; void (*unk8)(void *, void *); u8 pad0C[0x18 - 0xC]; u16 w18; u16 w1A; u16 w1C; } Q04Hdr;
typedef struct { u8 pad[4]; u32 flags; u8 pad8[0x1C - 8]; void (*unk1C)(void *, void *, void *); } Q04ItemDef;
typedef struct { s32 idx; u16 w4; u8 pad6[2]; u32 flags; u16 wC; u16 wE; u8 b10, b11, b12, b13; s32 f14; Q04ItemDef *def; s32 f1C; void *p20; void *owner; } Q04Itm;
typedef struct { u16 a; u8 pad2[2]; u16 b; u8 pad6[2]; u16 c; u8 padA[3]; u8 d; u8 pad0E[3]; u8 e; u8 pad12[3]; u8 f; u8 pad16[2]; } Q04Tab;
typedef struct { u16 w0, w2, w4, w6, w8, wA; u8 bC, bD, bE, bF; } Q04Rec;
typedef struct {
    u16 w0, w2, w4;
    u8 pad6[2];
    void *p8;
    Q04Itm *items;
    s32 n;
    void *hdr;
    s32 f18;
    s32 f1C;
    void *f20;
    s32 f24;
    u16 w28;
    u16 w2A;
    s32 f2C, f30, f34, f38, f3C;
    s32 f40;
    void *f44;
    Q04Rec rec[2][8];
} Q04Panel_414EA8;
extern void *func_00416B28_4143DC() __asm__("func_0044FB28");
extern void func_00416C00_4143DC() __asm__("func_0044FC00");
extern void func_00416C0C_4143DC(void *p) __asm__("func_0044FC0C");
extern Q04Tab D_800C1CD2[];
void func_0044DEA8(Q04Panel_414EA8 *p, void *a1, Q04Hdr *t, s32 a3, s32 a4, void *a5, s32 a6, void *a7)
{
  s32 i;
  s32 j;
  u8 new_var;
  s32 k;
  s32 k2;
  s32 off;
  u8 *e;
  s32 new_var2;
  Q04Itm *ib;
  u8 *ext;
  u8 **pc;
  p->w0 = t->w18;
  p->w2 = t->w1A;
  p->w4 = t->w1C;
  p->p8 = a1;
  ib = (Q04Itm *) (((u8 *) p) + 0x1D8);
  p->items = ib;
  p->n = t->n;
  ext = (u8 *) ib;
  ext = ((u8 *) ib) + (t->n * 0x28);
  for (i = 0; i < t->n; i++)
  {
    Q04Itm *it = (Q04Itm *) ((i * 0x28) + ((s32) p->items));
    Q04Tpl *tp = (Q04Tpl *) ((i * 0x24) + ((s32) t->unk0));
    it->idx = i;
    pc = &ext;
    {
      it->w4 = tp->type;
      it->flags = tp->flags | 0x01800000;
      it->wC = tp->w8;
      it->wE = tp->wA;
      it->b10 = tp->bC;
    }
    it->b11 = tp->bD;
    it->b12 = tp->bE;
    it->b13 = tp->bF;
    new_var2 = tp->f10;
    it->def = (Q04ItemDef *) tp;
    it->f1C = -1;
    it->owner = p;
    it->f14 = new_var2;
    k = 0;
    if (tp->type == 3)
    {
      k = 0x330;
    }
    if (k != 0)
    {
      e = *pc;
      it->p20 = e;
      if (tp->type == 3)
      {
        *((s32 *) (e + 0x328)) = 1;
      }
      k2 = 0;
      if (tp->type == 3)
      {
        k2 = 0x330;
      }
      *pc += k2;
    }
  }
  p->hdr = t;
  p->f18 = a3;
  p->f1C = a4;
  p->f20 = a5;
  p->f24 = a6;
  p->w28 = 0;
  p->w2A = *((u16 *) (((u8 *) a7) + 0x14));
  p->f2C = 0;
  p->f30 = 0;
  p->f34 = 0;
  p->f38 = 1;
  p->f3C = 0;
  p->f40 = (s32) func_00416B28_4143DC(a7);
  p->f44 = a7;
  for (j = 0; j < 2; j++)
  {
    for (i = 0; i < 8; i++)
    {
      u8 *rb = ((u8 *)p) + ((j << 7) + i * 16);
      *((u16 *) (rb + 0x48)) = D_800C1CD2[i].a;
      *((u16 *) (rb + 0x4A)) = D_800C1CD2[i].b;
      *((u16 *) (rb + 0x4C)) = D_800C1CD2[i].c;
      *((u8 *) (rb + 0x54)) = D_800C1CD2[i].d;
      *((u8 *) (rb + 0x55)) = D_800C1CD2[i].e;
      new_var = D_800C1CD2[i].f;
      *((u8 *) (rb + 0x57)) = 0xFF;
      *((u16 *) (rb + 0x50)) = 0;
      *((u16 *) (rb + 0x52)) = 0;
      *((u8 *) (rb + 0x56)) = new_var;
    }
  }
  *((s32 *) (((u8 *) a5) + 0xB0)) = 0;
  *((s32 *) (((u8 *) a5) + 0xB4)) = 0;
  *((s32 *) (((u8 *) a5) + 0xBC)) = 0;
  func_00416C00_4143DC(p);
  func_00416C0C_4143DC(p);
  if (t->unk8 != 0)
  {
    t->unk8(p, a7);
  }
  {
    Q04Itm *it;
    i = p->n;
    it = p->items;
    i--;
    if (i != (-1))
    {
      do
      {
        Q04ItemDef *d = it->def;
        if (d->flags & 0x04000000)
        {
          if (d->unk1C != (off = 0))
          {
            d->unk1C(it, p, a7);
          }
        }
        i--;
        it++;
      }
      while (i != (-1));
    }
  }
}


/* ---- func_0044E1B4 ---- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043F52C_de_4.c, func_804410AC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */


void func_0044E1B4(struct MenuRecAt10 *menu, u8 dir) {
    struct EntryRecAt18V2 *entry;
    s32 old;
    entry = &menu->entries[menu->cursor];
    do {
        old = menu->cursor;
        menu->cursor += ((s8 *)entry)[dir];
        if (menu->cursor < 0) {
            menu->cursor = menu->count - 1;
        } else if (menu->cursor >= menu->count) {
            menu->cursor = 0;
        }
        entry = &menu->entries[menu->cursor];
        old -= menu->cursor;
        switch (dir) {
        case 16:
            if (entry->flags & 0x80000) {
                entry->back[1] = old;
            }
            break;
        case 17:
            if (entry->flags & 0x40000) {
                entry->back[0] = old;
            }
            break;
        case 18:
            if (entry->flags & 0x200000) {
                entry->back[3] = old;
            }
            break;
        case 19:
            if (entry->flags & 0x100000) {
                entry->back[2] = old;
            }
            break;
        }
    } while ((entry->flags & 0x1800000) != 0x1800000 || entry->target->locked == 0);
}


/* ---- func_0044E31C ---- */
/* WRONG CUT: real function 0x0041531C, size 0x3E0 (through 0x004156FC): splat cut it into
 * func_0044E31C, func_0044E4C4, func_0044E500, func_0044E508, func_0044E5AC,
 * func_0044E60C and func_0044E6D8 at branch targets. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00441384.c, func_00441384, with Turok 2's layouts. */

typedef struct {
    /* 0x00 */ u8 pad0[0xB0];
    /* 0xB0 */ u32 buttons;
} MenuController_41531C;

typedef struct MenuItemDef_41531C {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ void (*update)(void *item, void *menu, void *arg);
    /* 0x1C */ s32 (*select)(void *item, void *menu, void *arg);
} MenuItemDef_41531C;

typedef struct {
    /* 0x00 */ s32 index;
    /* 0x04 */ s16 type;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ u32 flags;
    /* 0x0C */ u8 padC[0xC];
    /* 0x18 */ MenuItemDef_41531C *def;
    /* 0x1C */ s32 value;
    /* 0x20 */ u8 pad20[8];
} MenuItem_41531C; /* 0x28 bytes */

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 (*poll)(void);
} MenuTemplate_41531C;

typedef struct {
    /* 0x00 */ s16 cursor;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ MenuItem_41531C *items;
    /* 0x10 */ s32 count;
    /* 0x14 */ MenuTemplate_41531C *tmpl;
    /* 0x18 */ u8 pad18[8];
    /* 0x20 */ MenuController_41531C *ctl;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s16 state;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ f32 time;
    /* 0x30 */ f32 fade;
    /* 0x34 */ f32 open;
} Menu_41531C;

extern f32 D_800B6D28; /* frame delta */
extern s32 func_002855E8(MenuController_41531C *ctl);
extern s32 func_00285628_4143DC(MenuController_41531C *ctl) __asm__("func_00285628");
extern s32 func_00285670_4143DC(MenuController_41531C *ctl) __asm__("func_00285670");
extern s32 func_002856B8(MenuController_41531C *ctl);
extern s32 func_00285700(MenuController_41531C *ctl);
extern void func_00416C28_4143DC(Menu_41531C *menu) __asm__("func_0044FC28");

s32 func_0044E31C(Menu_41531C *menu, void *arg) {
    MenuItem_41531C *item;
    s32 quit;
    s32 i;
    s32 moveV;
    u32 horizontal;

    quit = 0;
    if (menu->tmpl->poll != NULL) {
        quit = menu->tmpl->poll();
    }
    i = menu->count;
    item = menu->items;
    i--;
    if (i != -1) {
        do {
            if (item->def->update != NULL) {
                item->def->update(item, menu, arg);
            }
            i--;
            item++;
        } while (i != -1);
    }
    item = (MenuItem_41531C *) ((u8 *) menu->items + menu->cursor * 0x28);
    menu->time = menu->time + D_800B6D28;
    horizontal = item->flags & 0x400000;
    func_00416C28_4143DC(menu);
    switch (menu->state) {
    case 0:
        menu->fade = menu->fade + D_800B6D28;
        if (1.0f <= menu->fade) {
            menu->fade = 1.0f;
            menu->state = 1;
        }
        break;
    case 1:
        menu->open = menu->open + D_800B6D28 * 0.4f;
        if (1.0f <= menu->open) {
            menu->open = 1.0f;
            menu->state = 2;
        }
        break;
    case 3:
        menu->open = menu->open - D_800B6D28 * 0.4f;
        if (menu->open <= 0.0f) {
            menu->open = 0.0f;
            menu->state = 4;
        }
        break;
    case 4:
        menu->fade = menu->fade - D_800B6D28;
        if (menu->fade <= 0.0f) {
            menu->fade = 0.0f;
            return 1;
        }
        break;
    }
    if (menu->state < 3) if (menu->state >= 0) {
        if (quit != 0) {
            menu->state = 4;
        }
        if (!(item->flags & 8)) {
            u32 buttons;

            moveV = 1;
            if (item->type == 2) {
                buttons = menu->ctl->buttons;
                if (buttons & 0x20202) {
                    moveV = 0;
                    item->value -= 2;
                    if (item->value < 0) {
                        item->value = 0;
                    }
                } else if (buttons & 0x40101) {
                    item->value += 2;
                    moveV = 0;
                    if (item->value >= 0x100) {
                        item->value = 0xFF;
                    }
                }
            }
            if (horizontal == 0) {
                if (func_00285628_4143DC(menu->ctl) != 0) {
                    func_0044E1B4((struct MenuRecAt10 *) menu, 0x10);
                }
                if (func_00285670_4143DC(menu->ctl) != 0) {
                    func_0044E1B4((struct MenuRecAt10 *) menu, 0x11);
                }
            } else if (menu->ctl->buttons & 0x60303) {
                moveV = 0;
            }
            if (moveV != 0) {
                if (func_002856B8(menu->ctl) != 0) {
                    func_0044E1B4((struct MenuRecAt10 *) menu, 0x12);
                }
                if (func_00285700(menu->ctl) != 0) {
                    func_0044E1B4((struct MenuRecAt10 *) menu, 0x13);
                }
            }
            if (menu->state == 2 && (func_002855E8(menu->ctl) != 0 || (func_00285628_4143DC(menu->ctl) != 0 && horizontal != 0)
                                     || (func_00285670_4143DC(menu->ctl) != 0 && horizontal != 0))) {
                if (item->def->select == NULL || item->def->select(item, menu, arg) != 0) {
                    menu->state = 3;
                }
            }
        }
    }
    return 0;
}


/* ---- func_0044E6FC ---- */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00441764.c, func_00441764.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

f32 func_002982F0(f32 x);
typedef struct Q04Item_4156FC {
    s32 idx;
    s16 w4;
    u8 pad6[2];
    u32 flags;
    u16 wC;
    u16 wE;
    u8 b10, b11, b12, b13;
    s32 f14;
    void *def;
    s32 f1C;
    void *p20;
    void *owner;
} Q04Item_4156FC;
typedef struct Q04Info_4156FC {
    s32 f0, f4, f8;
    f32 fC, f10;
    s32 x0, x1, y0, y1;
} Q04Info_4156FC;
typedef struct { u8 pad[0x14]; void (*f14)(void *, void *, void *, void *); } Q04DefB;
typedef struct { s16 cur; s16 w2; s16 w4; u8 pad6[6]; Q04Item_4156FC *items; s32 n; u8 pad14[0x30 - 0x14]; f32 f30; f32 f34; } Q04Pn;
typedef struct { u8 pad[0x288]; f32 f288; f32 f28C; f32 f290; f32 f294; } Q04Cm; /* Turok 2: viewport at 0x288 (Rage Wars: 0x29C) */
extern void func_002778A8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j);
extern f32 func_002982F0(f32 x);
extern s32 D_800C1BD0_4143DC __asm__("D_800C1BD0");
void func_0044E6FC(Q04Pn *p, void *a1) {
    struct { s32 f, w, h, g; } sz;
    struct { s32 f, x0, x1, y0, y1; } r;
    Q04Info_4156FC info;
    Q04Info_4156FC cp;
    Q04Cm *cam;
    Q04Item_4156FC *it;
    s32 i, v, v2, v3, v4;
    f32 cx, cy, hw, hh, fa, fb, sv, tt;
    u32 al;
    i = 0;
    cam = func_00416B28_4143DC(a1);
    r.x0 = 0x400; r.x1 = -0x400; r.y0 = 0x400; r.y1 = -0x400;
    info.f4 = 0; info.f8 = 0; info.x0 = 0; info.x1 = 0; info.y0 = 0; info.y1 = 0;
    for (; i < p->n; i++) {
        it = &p->items[i];
        if (it->flags & 0x800000) {
            if (it->flags & 0x20000000) {
                info.x0 = 0x400; info.x1 = -0x400; info.y0 = 0x400; info.y1 = -0x400;
                info.f4 = 0; info.f8 = 0;
            } else {
                func_0041363C_4143DC((MenuItem_4145B0 *) it, (MenuInfo_4145B0 *) &info);
            }
            v = info.x0; if (r.x0 < v) v = r.x0; r.x0 = v;
            v2 = info.x1; if (v2 < r.x1) v2 = r.x1; r.x1 = v2;
            v3 = info.y0; if (r.y0 < v3) v3 = r.y0; r.y0 = v3;
            v4 = info.y1; if (v4 < r.y1) v4 = r.y1; r.y1 = v4;
        }
    }
    sz.w = r.x1 - r.x0;
    sz.h = r.y1 - r.y0;
    if (sz.w >= 0 && sz.h >= 0) {
        cx = cam->f290 + cam->f288 * 0.5f + (f32)p->w2;
        cy = cam->f294 + cam->f28C * 0.5f + (f32)p->w4;
        info.f4 = 0; info.f8 = 0; info.x0 = 0; info.x1 = 0;
        hw = (f32)(sz.w / 2);
        hh = (f32)(sz.h / 2);
        info.y0 = 0; info.y1 = 0;
        r.x0 = (s32)(cx - hw);
        r.x1 = (s32)(cx + hw);
        r.y0 = (s32)(cy - hh);
        r.y1 = (s32)(cy + hh);
        D_800C1BD0_4143DC = -1;
        for (i = 0; i < p->n; i++) {
            it = &p->items[i];
            if (it->flags & 0x800000) {
                info.f0 = i;
                if (it->flags & 0x20000000) {
                    func_0041363C_4143DC((MenuItem_4145B0 *) it, (MenuInfo_4145B0 *) &info);
                    info.f4 = 0; info.f8 = 0;
                    info.x0 = (s32)cam->f290;
                    info.x1 = 0;
                    info.y0 = (s32)cam->f294;
                    info.y1 = 0;
                } else {
                    func_0041363C_4143DC((MenuItem_4145B0 *) it, (MenuInfo_4145B0 *) &info);
                }
                cp = info;
                if (!(it->flags & 0x20000000)) {
                    cp.x0 += r.x0;
                    cp.x1 += r.x0;
                    cp.y0 += r.y0;
                    cp.y1 += r.y0;
                }
                if (it->flags & 1) {
                    cp.x0 = (s32)(cx - (f32)(info.f4 / 2));
                    cp.x1 = cp.x0 + info.f4;
                }
                if (it->flags & 2) {
                    cp.x0 = r.x0;
                    cp.x1 = r.x0 + info.f4;
                }
                if (it->flags & 4) {
                    cp.x1 = r.x1;
                    cp.x0 = r.x1 - info.f4;
                }
                if (it->flags & 0x800000) {
                    if (p->cur == i && it->w4 != 3) {
                        if (!(it->flags & 8)) {
                            fb = (f32)cp.y0;
                            fa = (f32)cp.y1;
                            fa = fb + (fa - fb);
                            func_0027AF4C(it);
                            tt = func_002982F0((f32)((u32) D_800B6D20 * 10) * 0.017453294f);
                            sv = p->f30 * p->f34 * ((tt + 1.0f) * 50.0f + 50.0f);
                            if (it->flags & 0x10) {
                                func_002778A8(cp.x0 - 4, (s32)fb, cp.x1 + 4, (s32)fa, 0, 0, 0xFF, 0, 10, (u32)sv & 0xFF);
                            } else {
                                func_002778A8(cp.x0 - 4, (s32)fb, cp.x1 + 4, (s32)fa, 0, 0, 0xFF, 0, 10, (u32)sv & 0xFF);
                            }
                            D_800C1BD0_4143DC = -1;
                        }
                    }
                    ((Q04DefB *)it->def)->f14(it, &cp, &sz, p);
                }
            }
        }
    }
}

/* ---------------- func_0044ECE8 ---------------- */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00441D50.c, func_00441D50.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */






typedef struct Q04Item {
    s32 idx;
    s16 w4;
    u8 pad6[2];
    u32 flags;
    u16 wC;
    u16 wE;
    u8 b10, b11, b12, b13;
    s32 f14;
    void *def;
    s32 f1C;
    void *p20;
    void *owner;
} Q04Item;
typedef struct Q04Info {
    s32 f0, f4, f8;
    f32 fC, f10;
    s32 x0, x1, y0, y1;
} Q04Info;
typedef struct { u8 pad0[2]; s16 unk2; s16 unk4; u8 pad6[6]; Q04Item *unkC; s32 unk10; u8 pad14[0x30 - 0x14]; f32 unk30; u8 pad34[0x40 - 0x34]; void *unk40; } Q04W;
typedef struct { u8 pad[0x288]; f32 unk288; f32 unk28C; f32 unk290; f32 unk294; } Q04V; /* fields at 0x288 in Turok 2 (0x29C in Rage Wars) */
typedef struct { u8 pad[8]; u32 unk8; } Q04B;
extern void func_0041363C_415CE8(Q04Item *it, void *info) __asm__("func_0044C63C");
extern void func_0027AF4C_415CE8(void) __asm__("func_0027AF4C");
extern void func_002778A8_415CE8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j) __asm__("func_002778A8");
extern s32 D_800C2030;
extern s32 D_800C2034;
void func_0044ECE8(Q04W *a, Q04B *b, s32 c) {
    struct { s32 f, w, h, g; } sz;
    struct { s32 f, x0, x1, y0, y1; } r;
    Q04Info info;
    Q04V *cam;
    s32 i, v, v2, v3, v4;
    f32 fu; register f32 base asm("$f1"); f32 half, t, y0, k;
    u32 al;
    cam = a->unk40;
    switch (b->unk8 & 0x3FE0) {
    case 0x20: { register s32 hi asm("$2"); s32 lo; hi = 0x400; lo = -0x400; r.x0 = hi; r.x1 = lo; r.y0 = hi; r.y1 = lo; } break;
    case 0x40: { register s32 hi asm("$2"); s32 lo; hi = 0x400; lo = -0x400; r.x0 = hi; r.x1 = lo; r.y0 = hi; r.y1 = lo; } break;
    case 0x200: { register s32 hi asm("$2"); s32 lo; hi = 0x400; lo = -0x400; r.x0 = hi; r.x1 = lo; r.y0 = hi; r.y1 = lo; } break;
    default: { register s32 hi asm("$2"); s32 lo; hi = 0x400; lo = -0x400; r.x0 = hi; r.x1 = lo; r.y0 = hi; r.y1 = lo; } break;
    }
    info.f4 = 0; info.f8 = 0; info.x0 = 0; info.x1 = 0; info.y0 = 0; info.y1 = 0;
    for (i = 0; i < a->unk10; i++) {
        func_0041363C_415CE8(a->unkC + i, &info);
        v = info.x0; if (r.x0 < v) v = r.x0; r.x0 = v;
        v2 = info.x1; if (v2 < r.x1) v2 = r.x1; r.x1 = v2;
        v3 = info.y0; if (r.y0 < v3) v3 = r.y0; r.y0 = v3;
        v4 = info.y1; if (v4 < r.y1) v4 = r.y1; r.y1 = v4;
    }
    sz.w = r.x1 - r.x0;
    sz.h = r.y1 - r.y0;
    if (sz.w >= 0 && sz.h >= 0) {
        base = cam->unk290 + cam->unk288 * 0.5f + (f32)a->unk2;
        sz.w = D_800C2030;
        half = (f32)(D_800C2030 / 2);
        r.x0 = (s32)(base - half);
        r.x1 = (s32)(base + half);
        y0 = (f32)D_800C2034;
        t = y0 * 0.21621622f;
        t -= (D_800C2034 >= 0xDF) ? 13.0f : 9.0f;
        if (c != 0) y0 = (cam->unk294 + (f32)D_800C2034 + (f32)a->unk4) - t;
        else { register f32 pa asm("$f0"); f32 pb; pa = cam->unk294; pb = (f32)a->unk4; y0 = pa + pb; }
        r.y0 = (s32)y0;
        r.y1 = (s32)((f32)r.y0 + t);
        sz.h = r.y1 - r.y0;
        func_0027AF4C_415CE8();
        func_002778A8_415CE8(r.x0, r.y0, r.x1, r.y1, 1, 0, 0, 0, 0, (u32)(a->unk30 * 192.0f) & 0xFF);
    }
}

/* ---------------- func_0044EFB8 ---------------- */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00442020.c, func_00442020.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */



typedef struct { s16 unk0; u8 pad[0x22]; } O06I_415FB8;
typedef struct { O06I_415FB8 *unk0; s16 unk4; u8 pad6[0x1A]; s32 unk20; } O06B_415FB8;
typedef struct { u8 pad[0x14]; s16 unk14; } O06A_415FB8;
typedef struct { u8 pad[0x18]; O06B_415FB8 *unk18; s32 unk1C; s32 unk20; s32 unk24; } O06C_415FB8;
/* Static inline in the Rage Wars source file (dropped by the importer); 0x330 in Turok 2, 0x480 in Rage Wars. */
static inline s32 O06_ItemSize_415FB8(s16 *a) { if (*a == 3) return 0x330; return 0; }
extern s32 *func_00204EDC();
extern void func_00414EA8_415FB8() __asm__("func_0044DEA8");
extern void CList__AddHead(O06A_415FB8 *a, s32 b);
s32 func_0044EFB8(O06A_415FB8 *a, O06C_415FB8 *c) {
    s32 size, i;
    s32 *r;
    s32 s;
    O06B_415FB8 *b = c->unk18;
    s32 p1, p2, p3, p4;
    if (b != 0) {
        p1 = b->unk20; p2 = c->unk1C; p3 = c->unk20; p4 = c->unk24;
        size = b->unk4 * 40 + 0x1D8;
        for (i = 0; i < b->unk4; i++) size += O06_ItemSize_415FB8(&b->unk0[i].unk0);
        r = func_00204EDC(0, size, 0x3B, "menu");
        if (r != 0) {
            s = *r;
            if (s != 0) {
                func_00414EA8_415FB8(s, r, b, p1, p2, p3, p4, a);
                CList__AddHead(a, s);
                { s16 t = a->unk14 + 1; a->unk14 = t; if (t >= 4) a->unk14 = 0; }
                return s;
            }
        }
    }
    return 0;
}

/* ---------------- func_0044F0F0 ---------------- */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00442158.c, func_00442158, with Turok 2's layouts. */

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 type;
    /* 0x06 */ u8 pad6[0xE];
    /* 0x14 */ u8 *text;
} MenuItemTmpl_4160F0;

extern u8 *func_00413230_4160F0(void) __asm__("func_0044C230");

/* Length of the item's text up to the end or the first newline. */
s32 func_0044F0F0(MenuItemTmpl_4160F0 *a) {
    s32 n = 0;
    u8 *s;

    if (a->type == 5) {
        s = func_00413230_4160F0();
    } else {
        s = a->text;
    }
    for (; s && *s && *s != '\n'; s++) {
        n++;
    }
    return n;
}

/* ---------------- shared by func_0044F168 to func_00450418 ---------------- */
/* An open menu as the pool code below sees it (Menu_41531C has the same head). */
typedef struct {
    /* 0x00 */ u8 pad0[0xB0];
    /* 0xB0 */ s32 buttons;
    /* 0xB4 */ s32 pressed;
    /* 0xB8 */ u8 padB8[4];
    /* 0xBC */ s32 held;
} MenuNodeCtl;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 (*close)();
    /* 0x10 */ s32 (*draw)();
    /* 0x14 */ u8 pad14[0x1C - 0x14];
    /* 0x1C */ s32 value;
} MenuNodeDef;

typedef struct MenuNode {
    /* 0x000 */ u8 pad0[8];
    /* 0x008 */ s32 mem;            /* the allocation the menu lives in */
    /* 0x00C */ u8 padC[0x14 - 0xC];
    /* 0x014 */ MenuNodeDef *def;
    /* 0x018 */ u8 pad18[0x20 - 0x18];
    /* 0x020 */ MenuNodeCtl *ctl;
    /* 0x024 */ u8 pad24[0x28 - 0x24];
    /* 0x028 */ s16 state;
    /* 0x02A */ u8 pad2A[0x1C8 - 0x2A];
    /* 0x1C8 */ s32 flashTime;
    /* 0x1CC */ s32 flashStep;
    /* 0x1D0 */ struct MenuNode *prev;
    /* 0x1D4 */ struct MenuNode *next;
} MenuNode;

/* The open menus: a CList linked through MenuNode.prev/next, and a counter. */
typedef struct {
    /* 0x00 */ MenuNode *head;
    /* 0x04 */ MenuNode *tail;
    /* 0x08 */ s32 lastOffset;
    /* 0x0C */ s32 nextOffset;
    /* 0x10 */ s32 count;
    /* 0x14 */ s16 opened;
} MenuNodePool;

/* ---------------- func_0044F168 ---------------- */
s32 func_00413F74_416168(s32, s32, s32, s32, s32) __asm__("func_0044CF74");

void func_0044F168(MenuNode *arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_00413F74_416168((s32) arg0, arg1, arg2, arg3, (s32) arg0->def);
}

/* ---------------- func_0044F188 ---------------- */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00442224.c, func_00442224, with Turok 2's layouts. */

typedef struct {
    /* 0x00 */ u8 pad0[0x17];
    /* 0x17 */ u8 ch;
} MenuItemTmpl_416188;

extern void func_00413F74_416188(MenuItemTmpl_416188 *a, s32 b, s32 c, s32 d, u8 *text) __asm__("func_0044CF74");

/* Draws the item's single character. */
void func_0044F188(MenuItemTmpl_416188 *a, s32 b, s32 c, s32 d) {
    u8 buf[2];

    buf[0] = a->ch;
    buf[1] = 0;
    func_00413F74_416188(a, b, c, d, buf);
}

/* ---------------- func_0044F1B4 ---------------- */
s32 func_00413230_4161B4(s32) __asm__("func_0044C230");
s32 func_00413F74_4161B4(s32, s32, s32, s32, s32) __asm__("func_0044CF74");

void func_0044F1B4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_00413F74_4161B4(arg0, arg1, arg2, arg3, func_00413230_4161B4(arg0));
}

/* ---------------- func_0044F214 ---------------- */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004422BC.c, func_004422BC, with Turok 2's layouts. */

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 unk8;
    /* 0x0C */ u8 padC[8];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 pad20[0x10];
    /* 0x30 */ f32 unk30;
    /* 0x34 */ u8 pad34[0x68];
} MenuBlob; /* 0x9C bytes */

typedef struct {
    /* 0x000 */ s32 id;
    /* 0x004 */ MenuBlob blob;
    /* 0x0A0 */ u8 padA0[0x328 - 0xA0];
    /* 0x328 */ s32 valid;
    /* 0x32C */ f32 unk32C;
} MenuItemExt; /* 0x330 bytes */

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ MenuItemExt *ext;
} MenuItem_416214;

extern u8 D_800F7078[];
extern MenuBlob *func_00225EBC_416214(void *pool, s32 id, s32 arg) __asm__("func_00225EBC");

MenuBlob *func_0044F214(MenuItem_416214 *a, s32 id, s32 arg) {
    MenuItemExt *p = a->ext;
    MenuBlob *v = func_00225EBC_416214(D_800F7078, id, arg);

    if (v == NULL) {
        p->valid = 0;
        return NULL;
    }
    p->id = id;
    p->valid = 1;
    p->blob = *v;
    p->blob.unk0 = 0;
    p->blob.unk4 = 0;
    p->blob.unk14 = 0;
    p->blob.unk18 = 0;
    p->blob.unk1C = 0;
    p->blob.unk8 = 10240.0f;
    p->unk32C = p->blob.unk30;
    return &p->blob;
}

/* ---------------- func_0044F2E4 ---------------- */
s32 func_0044F2E4(MenuNode *arg0) {
    s32 temp_v1;
    s32 var_v0;

    temp_v1 = (s32) arg0->ctl;
    var_v0 = 0;
    if (temp_v1 != 0) {
        var_v0 = temp_v1 + 0xA0;
    }
    return var_v0;
}

/* ---------------- func_0044F2FC ---------------- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043F52C_de_6.c, func_8044222C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern float func_0044F2FC(int c, unsigned char next, float sx, float sy);
f32 func_0044F2FC(s32 arg0, u8 arg1, f32 arg2, f32 arg3) {
    f32 var_f0;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_a1_2;
    temp_a0 = arg0 & 0xFF;
    var_f0 = 1.0f;
    switch (temp_a0) {
    case 0x24:
    case 0x26:
    case 0x7E:
        return arg3;
    case 0x49:
    case 0x69:
        var_f0 = 0.5f;
    default:
        break;
    case 0x4C:
    case 0x6C:
        temp_a1 = arg1 & 0xFF;
        if ((temp_a1 == 0x74) || (temp_a1 == 0x54)) {
            var_f0 = 0.7f;
        }
        break;
    case 0x41:
    case 0x61:
        temp_a1_2 = arg1 & 0xFF;
        if ((temp_a1_2 == 0x74) || (temp_a1_2 == 0x54)) {
            var_f0 = 0.8f;
        }
        break;
    case 0x4D:
    case 0x6D:
        var_f0 = 1.2f;
        break;
    case 0x21:
    case 0x2E:
        var_f0 = 0.4f;
        break;
    }
    return var_f0 * arg2;
}

/* ---------------- func_0044F3C0 ---------------- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043F52C_de_6.c, func_804422F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern float func_004162FC_4163C0(int c, unsigned char next, float sx, float sy) __asm__("func_0044F2FC");
f32 func_004162FC_4163C0(s32 arg0, u8 arg1, f32 arg2, f32 arg3) __asm__("func_0044F2FC");
f32 func_0044F3C0(u8 *p, f32 sx, f32 sy) {
    f32 total;
    u8 c;
    total = 0.0f;
    while (p != 0 && (c = *p) != 0 && c != '\n') {
        p++;
        total += func_004162FC_4163C0(c, *p, sx, sy);
    }
    return total;
}

/* ---------------- func_0044F454 ---------------- */
s32 func_00415FB8_416454(s32, s32) __asm__("func_0044EFB8");

s32 func_0044F454(s32 arg0, s32 arg1, s32 arg2) {
    return func_00415FB8_416454(arg2, arg1) != 0;
}

/* ---------------- func_0044F474 ---------------- */
s32 func_0044F474(void) {
    return 0;
}

/* ---------------- func_0044F47C ---------------- */
s32 func_0044F47C(MenuNode *arg0) {
    return arg0->def->value;
}

/* ---------------- func_0044F48C ---------------- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043F52C_de_6.c, func_804423BC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern int func_00285628_41648C(struct ControllerProfile *object) __asm__("func_00285628");
extern int func_00285670_41648C(struct ControllerProfile *object) __asm__("func_00285670");
s32 func_002855E8_41648C(s32) __asm__("func_002855E8");
s32 func_0044F48C(func_8022A404_S1 *menu,s32 value,s32 step,s32 minimum,s32 maximum,s32 wrap) {
 if(((s32 (*)(s32))func_00285628_41648C)(menu->unk20))value-=step;
 if(((s32 (*)(s32))func_00285670_41648C)(menu->unk20)||func_002855E8_41648C(menu->unk20))value+=step;
 if(value<minimum) {
  value=minimum;if(wrap)value=maximum;
 } else if(value>maximum) {
  value=maximum;if(wrap)value=minimum;
 }
 return value;
}

/* ---------------- func_0044F558 ---------------- */
s32 func_00285628_416558(s32) __asm__("func_00285628");
s32 func_00285670_416558(s32) __asm__("func_00285670");
s32 func_0044F558(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
  s32 var_s0;
  s32 var_s1;
  s32 var_s2;
  s32 var_s4;
  s32 var_s5;
  s32 var_v0;
  int new_var;
  void *var_s3;
  ;
  var_s0 = arg1;
  var_s4 = arg2;
  var_s1 = arg4;
  var_s5 = arg5;
  var_s2 = arg3;
  if (func_00285628_416558(*((s32 *) (0x20 + ((s8 *) arg0)))) != 0)
  {
    var_s0 -= var_s4;
    if (var_s0 < var_s2)
    {
      var_s0 = var_s2;
      if (var_s5 != 0)
      {
        if (var_s2 || arg0)
        {
          var_s0 = var_s1;
        }
        else
        {
          var_s0 = var_s1;
        }
      }
    }
  }
  var_v0 = var_s0;
 new_var = 0x20; if (func_00285670_416558(*((s32 *) (((s8 *) arg0) + new_var))) != 0) { var_s0 += var_s4;
    var_v0 = var_s0;
    if (var_s1 < var_s0)
    {
      var_s0 = var_s1;
      if (var_s5 != 0)
      {
        var_s0 = var_s2;
      }
      var_v0 = var_s0;
    }
  }
  return var_v0;
}

/* ---------------- func_0044F614 ---------------- */
void CList__Construct(s32, s32, s32);

void func_0044F614(s32 arg0) {
    CList__Construct(arg0, 0x1D0, 0x1D4);
    ((MenuNodePool *) arg0)->opened = 0;
}

/* ---------------- func_0044F644 ---------------- */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_004426E4.c, func_004426E4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */



typedef struct { s16 unk0; u8 pad[0x22]; } O06I_416644;
typedef struct { O06I_416644 *unk0; s16 unk4; } O06B_416644;
typedef struct { u8 pad[0x14]; s16 unk14; } O06A_416644;
/* Extra allocation per item: kind 3 items carry a 0x330-byte block (0x480 in Rage Wars). */
static __inline__ s32 O06_ItemSize_416644(s16 *a) {
    s32 n = 0;
    if (*a == 3) n = 0x330;
    return n;
}
extern s32 *func_00204EDC_416644() __asm__("func_00204EDC");
extern void func_00414EA8_416644() __asm__("func_0044DEA8");
extern void CList__AddHead_416644(O06A_416644 *a, s32 b) __asm__("CList__AddHead");
s32 func_0044F644(O06A_416644 *a, O06B_416644 *b, s32 c, s32 d, s32 e) {
    s32 size, i;
    s32 *r;
    s32 s;
    size = b->unk4 * 40 + 0x1D8;
    for (i = 0; i < b->unk4; i++) size += O06_ItemSize_416644(&b->unk0[i].unk0);
    r = func_00204EDC_416644(0, size, 0x3B, "menu");
    if (r == 0) return 0;
    s = *r;
    if (s == 0) return 0;
    func_00414EA8_416644(s, r, b, 0, c, d, e, a);
    CList__AddHead_416644(a, s);
    { s16 t = a->unk14 + 1; a->unk14 = t; if (t >= 4) a->unk14 = 0; }
    return s;
}

/* ---------------- func_0044F760 ---------------- */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00442800.c, func_00442800.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */



typedef struct { s16 unk0; u8 pad[0x22]; } O06I_416760;
typedef struct { O06I_416760 *unk0; s16 unk4; } O06B_416760;
typedef struct { u8 pad[0x14]; s16 unk14; } O06A_416760;
typedef struct { u8 pad[0x14]; s32 unk14; s32 unk18; s32 unk1C; s32 unk20; s32 unk24; } O06C_416760;
/* Turok 2 inlines the per-item size (Rage Wars calls it): kind 3 adds 0x330 */
static inline s32 O06_ItemSize_416760(s16 *a) {
    s32 n;
    if (*a == 3) n = 0x330; else n = 0;
    return n;
}
extern s32 *func_00204EDC_416760() __asm__("func_00204EDC");
extern void func_00414EA8_416760() __asm__("func_0044DEA8");
extern void CList__AddHead_416760(O06A_416760 *a, s32 b) __asm__("CList__AddHead");
s32 func_0044F760(O06A_416760 *a, O06C_416760 *c, O06B_416760 *b) {
    s32 size, i;
    s32 *r;
    s32 s;
    s32 p1 = c->unk18, p2 = c->unk1C, p3 = c->unk20, p4 = c->unk24;
    size = b->unk4 * 40 + 0x1D8;
    for (i = 0; i < b->unk4; i++) size += O06_ItemSize_416760(&b->unk0[i].unk0);
    r = func_00204EDC_416760(0, size, 0x3B, "menu");
    if (r == 0) return 0;
    s = *r;
    if (s == 0) return 0;
    func_00414EA8_416760(s, r, b, p1, p2, p3, p4, a);
    CList__AddHead_416760(a, s);
    { s16 t = a->unk14 + 1; a->unk14 = t; if (t >= 4) a->unk14 = 0; }
    return s;
}

/* ---------------- func_0044F894 ---------------- */
/* WRONG CUT: real function 0x00416894, size 0x134 (through 0x004169C8): splat cut its
 * epilogue off as func_0044F9A0. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00442020.c, func_00442020, with Turok 2's layouts
 * (the template is passed directly instead of read from the caller). */

typedef struct {
    /* 0x00 */ s16 type;
    /* 0x02 */ u8 pad2[0x22];
} MenuItemTmpl_416894; /* 0x24 bytes */

typedef struct {
    /* 0x00 */ MenuItemTmpl_416894 *items;
    /* 0x04 */ s16 count;
} MenuTmpl_416894;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s16 opened;
} MenuPool_416894;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
} MenuCaller;

/* Each item of type 3 gets a 0x330-byte extension after the item array. */
static __inline__ s32 ItemExtSize(s16 *type) {
    if (*type == 3) {
        return 0x330;
    }
    return 0;
}

extern s32 *func_00204EDC_416894(s32, s32, s32, void *) __asm__("func_00204EDC");
extern void func_00414EA8_416894(s32 menu, s32 *mem, MenuTmpl_416894 *t, s32, s32, s32, s32, MenuPool_416894 *pool) __asm__("func_0044DEA8");
extern void CList__AddHead_416894(MenuPool_416894 *a, s32 b) __asm__("CList__AddHead");

s32 func_0044F894(MenuPool_416894 *pool, MenuCaller *c, MenuTmpl_416894 *t) {
    s32 size, i;
    s32 *r;
    s32 s;
    s32 p1, p2, p3, p4;

    p1 = c->unk14;
    p2 = c->unk1C;
    p3 = c->unk20;
    p4 = c->unk24;
    size = t->count * 40 + 0x1D8;
    for (i = 0; i < t->count; i++) {
        size += ItemExtSize(&t->items[i].type);
    }
    r = func_00204EDC_416894(0, size, 0x3B, "menu");
    if (r == 0) {
        return 0;
    }
    s = *r;
    if (s == 0) {
        return 0;
    }
    func_00414EA8_416894(s, r, t, p1, p2, p3, p4, pool);
    CList__AddHead_416894(pool, s);
    {
        s16 n = pool->opened + 1;
        pool->opened = n;
        if (n >= 4) {
            pool->opened = 0;
        }
    }
    return s;
}

/* ---------------- func_0044F9C8 ---------------- */
s32 CList__Remove(s32, s32);
s32 func_002052D8(s32, s32);
s32 func_0041531C_4169C8(s32, s32) __asm__("func_0044E31C");

extern MenuNodePool D_80119E2C;

void func_0044F9C8(MenuNodePool *arg0) {
    s32 (*temp_v0)();
    MenuNode *temp_s2;
    MenuNodeCtl *temp_v0_2;
    MenuNode *var_s0;

    var_s0 = arg0->head;
    if (var_s0 != NULL) {
        do {
            temp_s2 = var_s0->next;
            if ((var_s0->state == 2) && (arg0 != &D_80119E2C)) {
                if (D_80119E2C.count == 0) {
                    goto block_6;
                }
            } else {
block_6:
                if (func_0041531C_4169C8((s32) var_s0, (s32) arg0) != 0) {
                    temp_v0 = var_s0->def->close;
                    if (temp_v0 != NULL) {
                        temp_v0(var_s0, arg0);
                    }
                    temp_v0_2 = var_s0->ctl;
                    temp_v0_2->buttons = 0;
                    temp_v0_2->pressed = 0;
                    temp_v0_2->held = 0;
                    CList__Remove((s32) arg0, (s32) var_s0);
                    func_002052D8(0, var_s0->mem);
                }
            }
            var_s0 = temp_s2;
        } while (var_s0 != NULL);
    }
}

/* ---------------- func_0044FAA4 ---------------- */
void func_0044FAA4(MenuNodePool *arg0) {
    MenuNode *var_s0;

    var_s0 = arg0->tail;
    if (var_s0 != NULL) {
        do {
            var_s0->def->draw(var_s0, arg0);
            var_s0 = var_s0->prev;
        } while (var_s0 != NULL);
    }
}

/* ---------------- func_0044FAF8 ---------------- */
s32 func_0044FAF8(MenuNodePool *arg0) {
    s32 var_v1;
    MenuNode *var_a0;

    var_a0 = arg0->head;
    var_v1 = 0;
    if (var_a0 != NULL) {
        do {
            if (var_a0->state != 4) {
                var_v1 += 1;
            }
            var_a0 = var_a0->next;
        } while (var_a0 != NULL);
    }
    return var_v1;
}

/* ---------------- func_0044FB28 ---------------- */
s32 func_0044FB28(s32 arg0) {
    return arg0 - 0x53C;
}

/* ---------------- func_0044FB30 ---------------- */
/* WRONG CUT: real function 0x00416B30, size 0x7C (through 0x00416BAC): splat cut the loop
 * off as func_0044FB44. */

typedef struct {
    /* 0x00 */ u8 pad0[0xB0];
    /* 0xB0 */ s32 unkB0;
    /* 0xB4 */ s32 unkB4;
    /* 0xB8 */ u8 padB8[4];
    /* 0xBC */ s32 unkBC;
} MenuController;

typedef struct Menu_416B30 Menu_416B30;
typedef struct MenuPool_416B30 MenuPool_416B30;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ void (*close)(Menu_416B30 *menu, MenuPool_416B30 *pool);
} MenuTmpl_416B30;

struct Menu_416B30 {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ void *mem;
    /* 0x0C */ u8 padC[8];
    /* 0x14 */ MenuTmpl_416B30 *tmpl;
    /* 0x18 */ u8 pad18[8];
    /* 0x20 */ MenuController *ctl;
};

struct MenuPool_416B30 {
    /* 0x00 */ Menu_416B30 *head;
};

extern void CList__Remove_416B30(MenuPool_416B30 *list, Menu_416B30 *item) __asm__("CList__Remove");
extern void func_002052D8_416B30(s32, void *mem) __asm__("func_002052D8");

/* Closes and frees every open menu. */
void func_0044FB30(MenuPool_416B30 *pool) {
    Menu_416B30 *menu;
    MenuController *ctl;

next:
    menu = pool->head;
    if (menu != NULL) {
        if (menu->tmpl->close != NULL) {
            menu->tmpl->close(menu, pool);
        }
        ctl = menu->ctl;
        ctl->unkB0 = 0;
        ctl->unkB4 = 0;
        ctl->unkBC = 0;
        CList__Remove_416B30(pool, menu);
        func_002052D8_416B30(0, menu->mem);
        goto next;
    }
}

/* ---------------- func_0044FBAC ---------------- */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 type;
    /* 0x06 */ u8 pad6[0xE];
    /* 0x14 */ u8 *text;
} MenuItemTmpl_416BAC;

extern u8 *func_00413230_416BAC(void) __asm__("func_0044C230");
extern f32 func_004163C0_416BAC(u8 *text, f32 sx, f32 sy) __asm__("func_0044F3C0");

/* Width of the item's text at the given scale. */
f32 func_0044FBAC(MenuItemTmpl_416BAC *a, f32 scale) {
    u8 *s;

    if (a->type == 5) {
        s = func_00413230_416BAC();
    } else {
        s = a->text;
    }
    return func_004163C0_416BAC(s, scale, scale);
}

/* ---------------- func_0044FC00 ---------------- */
void func_0044FC00(MenuNode *arg0) {
    arg0->flashStep = 0;
    arg0->flashTime = 0;
}

/* ---------------- func_0044FC0C ---------------- */
void func_0044FC0C(MenuNode *arg0) {
    if (arg0->flashStep == 0) {
        arg0->flashStep = 1;
        arg0->flashTime = 0;
    }
}

/* ---------------- func_0044FC28 ---------------- */
void func_0044FC28(MenuNode *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->flashTime + arg0->flashStep;
    arg0->flashTime = temp_v0;
    if (temp_v0 >= 0x30) {
        arg0->flashTime = 0;
        arg0->flashStep = 0;
    }
}

/* ---------------- func_0044FC54 ---------------- */
typedef struct {
    char pad0[0x1C8];
    s32 unk_1C8; /* 0x1C8 */
} Obj1C8;

s32 func_0044FC54(Obj1C8 *obj) {
    s32 v = obj->unk_1C8;
    if (v < 0x18) {
        return v;
    }
    return 0x2F - v;
}

/* ---------------- func_0044FC74 ---------------- */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00442D18.c, func_00442D18.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */





typedef struct { u32 w0; u32 w1; } O06Gfx;
typedef struct { u8 pad[0x288]; f32 unk288; f32 unk28C; f32 unk290; f32 unk294; } O06V; /* fields at 0x288 in Turok 2 (0x29C in Rage Wars) */
typedef struct { u8 pad[0x40]; O06V *unk40; u8 pad2[0x184]; s32 unk1C8; s32 unk1CC; } O06A_416C74;
extern void func_00277F6C(void);
extern void func_00207FEC(s32 a);
extern void func_0027B320_416C74(s32 a, s32 b, s16 c, s16 d, f32 e, f32 f, s32 g) __asm__("func_0027B320");
extern O06Gfx *D_800D8E24_416C74 __asm__("D_800D8E24");
extern s32 D_800C2030;
extern s32 D_800C2034;
void func_0044FC74(O06A_416C74 *a) {
    s32 v, t;
    f32 x, y;
    O06Gfx *g;
    if (a->unk1CC == 0) return;
    t = a->unk1C8;
    v = (t < 0x18) ? t : 0x2F - t;
    func_00277F6C();
    func_00207FEC(0x15);
    x = a->unk40->unk288;
    y = a->unk40->unk28C;
    g = D_800D8E24_416C74++;
    g->w0 = 0xFB000000; g->w1 = 0xFF0000FF;
    g = D_800D8E24_416C74++;
    g->w0 = 0xFA000000; g->w1 = 0xE00000FF;
    func_0027B320_416C74(0x67, v, (s32)a->unk40->unk290, (s32)a->unk40->unk294,
                  x / (f32)D_800C2030 * 5.0f,
                  y / (f32)D_800C2034 * 4.0f, 1);
}

/* ---------------- func_0044FDA4 ---------------- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80442BC8.CPP, func_80442CD8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */


extern s32 func_0044FDA4(s16 *record);
s32 func_0044FDA4(s16 *record) {
    if (*record == 3) {
        return 0x330;
    }
    return 0;
}

/* ---------------- func_0044FDC0 ---------------- */
void func_0044FDC0(s16 *arg0, MenuModel_414938 *arg1) {
    if (*arg0 == 3) {
        arg1->loaded = 1;
    }
}

/* ---------------- func_0044FDDC ---------------- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80442BC8.CPP, func_80442D10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct WidgetDescriptor;
struct WidgetDescriptor {
    short type;
    short pad;
    int flags;
    unsigned short x;
    unsigned short y;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    int value;
};
struct ObjectRecAt478;
struct ObjectRecAt478 {
    char pad[0x328];
    s32 flag;
};
struct ObjectRecAt478;
struct WidgetDescriptor;
struct CMenuItem_t;
struct CMenuItem_t {
    int id;
    unsigned short type;
    unsigned short pad;
    int flags;
    unsigned short x;
    unsigned short y;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    int value;
    struct WidgetDescriptor *desc;
    int index;
    void *extra;
    int m_pMenu;
};
struct WidgetDescriptor;
typedef struct WidgetDescriptor WidgetDescriptor;
struct CMenuItem_t;
typedef struct CMenuItem_t CMenuItem;
extern void func_0044FDDC(CMenuItem *w, WidgetDescriptor *d, char **arena, int id, int arg);
void func_0044FDDC(CMenuItem *w, WidgetDescriptor *d, char **arena, int id, int arg) {
 int size; char *extra;
 w->id=id; w->type=d->type; w->flags=d->flags|0x1800000;
 w->x=d->x; w->y=d->y; w->r=d->r; w->g=d->g; w->b=d->b; w->a=d->a;
 w->value=d->value; w->desc=d; w->index=-1; w->m_pMenu=arg;
 size=0; if(d->type==3) size=0x330;
 if(size) {
 w->extra=*arena;
 extra=*arena; if(d->type==3) ((struct ObjectRecAt478 *)(extra))->flag=1;
 { int step=0; if(d->type==3) step=0x330;
 *arena+=step; }
 }
}

/* ---------------- func_0044FEA8 ---------------- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80442A58_de.c, func_80442DDC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern char D_800C1BD4_416EA8[] __asm__("D_800C1BD4");
void *func_0044FEA8(ObjRecAt8V4 *arg0) {
 s32 index=0;
 switch(arg0->flags & 0x3fe0) {
 case 0x20: break;
 case 0x40: index=1; break;
 case 0x80: index=2; break;
 case 0x100: index=3; break;
 case 0x200: index=4; break;
 case 0x400: index=5; break;
 case 0x800: index=6; break;
 case 0x1000: index=7; break;
 case 0x2000: index=8; break;
 default: index=0; break;
 }
 return D_800C1BD4_416EA8+index*28;
}

/* ---------------- func_0044FF8C ---------------- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80442EC0_de.c, func_80442EC0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

f32 func_0044FF8C(ItemRecAt14 *arg0) {
 Binding *v = arg0->binding;
 void *p = v->storage();
 switch(v->type) {
 default: return 0.0f;
 case 0: return *(f32 *)p;
 case 3: return *(s8 *)p;
 case 4: return *(s16 *)p;
 case 1: case 2: case 5: return *(s32 *)p;
 case 6: return *(u8 *)p;
 case 7: return *(u16 *)p;
 case 8: return *(u32 *)p;
 }
}

/* ---------------- func_0045007C ---------------- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80442FB0_de.c, func_80442FB0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

void func_0045007C(ItemRecAt14 *item,float value) {
 float *storage;
 struct Binding *binding;
 unsigned int *word;
 binding=item->binding;
 storage=binding->storage();
 switch(binding->type) {
 case 0:*(float *)storage=value;return;
 case 3:*(signed char *)storage=(int)value;return;
 case 4:*(short *)storage=(int)value;return;
 case 1:case 2:case 5:*(int *)storage=(int)value;return;
 case 6:*(unsigned char *)storage=(unsigned int)value;return;
 case 7:*(unsigned short *)storage=(unsigned int)value;return;
 case 8:word=(unsigned int *)storage;*word=(unsigned int)value;return;
 }
}

/* ---------------- func_004501D0 ---------------- */
s32 func_0021ECA0(s32, s32, s32, s32);

extern s32 D_800AB0E8;

void func_004501D0(s32 arg0, s32 arg1, s32 arg2) {
    func_0021ECA0(arg0, arg1, arg2, (s32) &D_800AB0E8);
}

/* ---------------- func_004501F4 ---------------- */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00443298.c, func_00443298.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */




typedef struct { u32 w[6]; } O06Seg;
typedef struct { u8 pad[0x110]; O06Seg seg[1]; } O06A_4171F4; /* seg at 0x110 in Turok 2 (0x140 in Rage Wars) */
typedef struct { f32 f0; s32 unk4; s32 unk8; f32 fC; s32 unk10; } O06Cam;
extern s32 D_800AB8B0;
extern O06Cam D_800AB8B4[];
extern O06Seg D_800AB0E8_4171F4 __asm__("D_800AB0E8");
extern s32 D_800B6D1C;
extern s32 D_801308E0;
extern void func_0021D928(void *a, s32 b, s32 c);
void func_004501F4(O06A_4171F4 *a, s32 b, s32 c) {
 D_800AB8B0 = 3;
 a->seg[D_800B6D1C] = D_800AB0E8_4171F4;
 D_800AB8B4->f0 = 255.0f;
 D_800AB8B4->unk4 = 0;
 D_800AB8B4->unk8 = 0;
 D_800AB8B4->unk10 = 0;
 D_800AB8B4->fC = (f32)(D_801308E0 / 2);
 func_0021D928(a, b, c);
 D_800AB8B0 = 0;
}

/* ---------------- func_004502B0 ---------------- */
void func_004502B0(MenuNode *arg0) {
    s32 (*temp_v0)();
    MenuNodeCtl *temp_v0_2;

    temp_v0 = arg0->def->close;
    if (temp_v0 != NULL) {
        temp_v0();
    }
    temp_v0_2 = arg0->ctl;
    temp_v0_2->buttons = 0;
    temp_v0_2->pressed = 0;
    temp_v0_2->held = 0;
}

/* ---------------- func_004502F8 ---------------- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80442BC8_11.CPP, func_8044322C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */


struct func_8025E5B0_S1;
struct func_8025E5B0_S1 {
    char pad0[0x14];
    short unk14;
};
struct Entry_func_80441EB0_de;
struct Entry_func_80441EB0_de {
    s16 type;
    char pad[0x22];
};
struct Entry_func_80441EB0_de;
struct Dialog;
struct Dialog {
    struct Entry_func_80441EB0_de *entries;
    s16 count;
    char pad6[0x1A];
    signed int value;
};
struct func_8025E5B0_S1;
typedef struct func_8025E5B0_S1 func_8025E5B0_S1;
struct Entry_func_80441EB0_de;
extern void func_00414EA8_4172F8(void *, void **, struct Dialog *, s32, s32, s32, s32, struct func_8025E5B0_S1 *) __asm__("func_0044DEA8");
void CList__AddHead_4172F8(void *list, void *node) __asm__("CList__AddHead");
extern void **func_00204EDC_4172F8(s32, s32, s32, char *) __asm__("func_00204EDC");
void *func_004502F8(struct func_8025E5B0_S1 *owner, struct Dialog *list, s32 a, s32 b, s32 c, s32 d) {
    s32 size = list->count * 40 + 0x1D8;
    s32 i;
    void **block;
    void *first;
    for (i = 0; i < list->count; i++) {
        s32 extra = 0;
        if (list->entries[i].type == 3) {
            extra = 0x330; /* 0x480 in Rage Wars */
        }
        size += extra;
    }
    block = func_00204EDC_4172F8(0, size, 0x3B, "menu");
    if (block == 0) {
        return 0;
    }
    first = *block;
    if (first == 0) {
        return 0;
    }
    func_00414EA8_4172F8(first, block, list, a, b, c, d, owner);
    CList__AddHead_4172F8(owner, first);
    if (++owner->unk14 >= 4) {
        owner->unk14 = 0;
    }
    return first;
}

/* ---------------- func_00450418 ---------------- */
s32 CList__Remove_417418(s32, s32) __asm__("CList__Remove");
s32 func_002052D8_417418(s32, s32) __asm__("func_002052D8");

void func_00450418(s32 arg0, MenuNode *arg1) {
    s32 (*temp_v0)();
    MenuNodeCtl *temp_v0_2;

    temp_v0 = arg1->def->close;
    if (temp_v0 != NULL) {
        temp_v0(arg1, arg0);
    }
    temp_v0_2 = arg1->ctl;
    temp_v0_2->buttons = 0;
    temp_v0_2->pressed = 0;
    temp_v0_2->held = 0;
    CList__Remove_417418(arg0, (s32) arg1);
    func_002052D8_417418(0, arg1->mem);
}
