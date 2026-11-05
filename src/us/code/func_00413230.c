/* One file, 0x413230-0x417484, .rodata 0x800C0AF0-0x800C0E64: the menu
 * code from func_00413230 to func_00417418. Objects start 16-aligned in
 * .text and in .rodata, with junk words before: stray_00413228 opens it,
 * stray_00417484 closes it, and its pool opens after junk at 0x800C0AE8 and
 * ends with junk at 0x800C0E64, before the next object's at 0x800C0E70.
 * The pools run on from one function to the next (func_004156FC's double at
 * 0x800C0C28 is 8-aligned only counted from 0x800C0AF0), and the string
 * "menu" (0x800C0C54) is one constant for func_00415FB8, func_00416644,
 * func_00416760, func_00416894 and func_004172F8.
 *
 * Each part was matched alone first and keeps its own types; names two
 * parts both define carry the part's address as a suffix. The second
 * module holds this file byte for byte at the same link address
 * (func_0044C230.c). */
#include "common.h"
#include "structs/game.h"
#include "rw/abu_types.h"
#include "structs/menu.h"
#include "structs/menu_model.h"
#include "structs/camera.h"
#include "gfx.h"

/* ---------------- func_00413230 ---------------- */
/* WRONG CUT: splat's func_0041321C (0x41321C-0x4133A4) is the shared return of func_004131B0 (3 words), a padding nop,
 * one junk word (0x41322C) and this function, which really starts at 0x413230 (0x174 bytes). */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0043F290.c, func_0043F290, with Turok 2's layouts. */

extern s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */
extern char D_801308A0[];
extern char D_80130860[];

/* Reads the item's bound variable as a float, whatever its type. */
static __inline__ f32 MenuItem_GetValue(MenuItem *item) {
    MenuBinding *binding = item->binding;
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
char *func_00413230(MenuItem *item) {
    MenuBinding *binding = item->binding;
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

/* ---------------- func_004133A4 ---------------- */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0043F404.c, func_0043F404, with Turok 2's layouts. */
/* A menu slider: step its bound variable (any numeric type) with left/right. */

extern s32 func_00285628(ControllerProfile *controller);   /* left pressed */
extern s32 func_00285670(ControllerProfile *controller);   /* right pressed */

static __inline__ f32 bindingGet(MenuItem *item) {
    MenuBinding *b = item->binding;
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

static __inline__ void bindingSet(MenuItem *item, f32 value) {
    MenuBinding *b = item->binding;
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

s32 func_004133A4(MenuItem *item, Menu *menu) {
    MenuBinding *b = item->binding;
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

/* ---------------- func_0041363C ---------------- */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0043F69C.c, func_0043F69C, with Turok 2's layouts. */

extern s32 D_800C2030; /* screen width */
extern s32 D_800C2034; /* screen height */

extern s32 func_00266B80(void);
extern CCamera *func_00416B28_41363C(void *pool) __asm__("func_00416B28");
extern u8 *func_00413230_41363C(MenuItem *item) __asm__("func_00413230");
extern f32 func_004163C0_41363C(u8 *text, f32 a, f32 b) __asm__("func_004163C0");
extern void func_0027AEA8(s32 image, s32 b, s32 *width, s32 *height);

static __inline__ MenuTextStyle *MenuItem_Style_41363C(MenuItem *it) {
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
void func_0041363C(MenuItem *it, MenuItemLayout *o) {
    CCamera *cam;
    MenuTextStyle *E;
    MenuItem cp;
    MenuItemLayout a, b, c;
    u8 *p;
    MenuModelPart *sub;
    s32 n, n2;
    f32 fs, v, fa, fb, vt, vt2;

    cam = func_00416B28_41363C(it->menu->pool);
    o->scaleY = 1.0f;
    o->scaleX = 1.0f;
    switch (it->type) {
        case 0:
        case 5:
            E = MenuItem_Style_41363C(it);
            if (E->font == 0) {
                fs = E->charWidth;
                if (it->type == 5) {
                    p = func_00413230_41363C(it);
                } else {
                    p = (u8 *) it->data;
                }
                v = func_004163C0_41363C(p, fs, fs);
                o->width = v;
            } else {
                n = 0;
                p = (it->type == 5 ? func_00413230_41363C(it) : (u8 *) it->data);
                for (; p != NULL && *p != 0 && *p != 10; p++) {
                    n++;
                }
                o->width = n * E->charWidth;
            }
            if (it->flags & 0x08000000) {
                o->height = (f32) D_800C2034 * 0.10810811f;
            } else if (E->font == 2 && D_800C2034 >= 0xDF) {
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
            if ((D_8011AAD8.mode == 13 && (it->flags & 0x40000000)) ||
                (D_8011AAD8.multiplayer != 0 && (s32) it->flags < 0 && func_00266B80() == 0x400000)) {
                o->width = 0x11C;
                o->height = 0xDE;
            } else {
                func_0027AEA8(it->data, 0, &o->width, &o->height);
            }
            if ((it->flags & 0x10000000) && (f32) o->width != 0.0f && (f32) o->height != 0.0f) {
                fa = cam->m_Viewport.m_Width / (f32) o->width;
                fb = cam->m_Viewport.m_Height / (f32) o->height;
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
            cp.type = 0;
            func_0041363C(&cp, &a);
            b = *o;
            cp = *it;
            cp.type = 1;
            cp.data = 0;
            func_0041363C(&cp, &b);
            c = *o;
            cp = *it;
            cp.type = 1;
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
            sub = it->part;
            o->width = ((f32) sub->width * cam->m_Viewport.m_Width) / (f32) D_800C2030;
            o->height = ((f32) sub->height * cam->m_Viewport.m_Height) / (f32) D_800C2034;
            break;
        default:
            o->width = 1;
            o->height = 1;
            break;
    }
    vt = (f32) ((it->x * D_800C2030) / 284);
    if (!(it->flags & 0x8000)) {
        if (it->flags & 0x4000) {
            o->left = (f32) o->right + vt;
        } else {
            o->left = vt;
        }
    }
    o->right = (f32) o->left + (f32) o->width * o->scaleX;
    vt2 = (f32) ((it->y * D_800C2034) / 222);
    if (!(it->flags & 0x20000)) {
        if (it->flags & 0x10000) {
            o->top = (f32) o->bottom + vt2;
        } else {
            o->top = vt2;
        }
    }
    o->bottom = (f32) o->top + (f32) o->height * o->scaleY;
}

/* ---------------- func_00413F74 ---------------- */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0043FFAC.c, func_0043FFAC, with Turok 2's layouts. */

extern s32 D_800C1BD0; /* current draw mode */
extern s32 D_801101B0[];
extern s32 D_800C2030; /* screen width */
extern s32 D_800C2034; /* screen height */

extern s32 func_00266B80(void);
extern void func_00415CE8_413F74(Menu *w, MenuItem *it, s32 c) __asm__("func_00415CE8");
extern void func_00277F6C(void);
extern void func_0027836C(void);
extern void func_0027845C(void);
extern void COnScreen__SetFontColor(s32 r0, s32 g0, s32 b0, s32 r1, s32 g1, s32 b1);
extern void COnScreen__SetFontScale(f32 x, f32 y);
extern void func_00278CDC(char *text, s32 x, s32 y, s32 alpha, s32 centred, s32 shadow, f32 scaleX, f32 scaleY);

static __inline__ MenuTextStyle *MenuItem_Style_413F74(MenuItem *it) {
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
void func_00413F74(MenuItem *it, MenuItemLayout *b, MenuItemLayout *c, Menu *w, char *text) {
    u8 en;
    MenuTextStyle *E;
    f32 k;
    s32 r, g, bl, a, t, t2;

    en = 1;
    if ((D_8011AAD8.mode == 13 && (it->flags & 0x40000000)) ||
        (D_8011AAD8.multiplayer != 0 && (s32) it->flags < 0 && func_00266B80() == 0x400000)) {
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
    if (D_800C1BD0 != E->font) {
        switch (E->font) {
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
        D_800C1BD0 = E->font;
    }
    if (b->index == w->cursor) {
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
    func_00278CDC(text, b->left, b->top, (s32) (w->fade * (w->alpha * 255.0f)), 0, en, 1.0f, 1.0f);
}

/* ---------------- func_004143DC ---------------- */
/* func_004143DC to func_004156FC were matched first as one file of their
 * own (0x4143DC-0x415CE8, .rodata from 0x800C0BE8); their types carry the
 * address of the part that matched them. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0044044C.c, func_0044044C, with Turok 2's layouts. */

extern s32 D_800C1BD0_4143DC __asm__("D_800C1BD0"); /* current draw mode */

extern s32 func_00266B80(void);
extern void func_0027AF4C(); /* called with and without an argument */
extern void func_002778A8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j);
extern void func_00279028(s32 alpha);
extern void func_0027B320(s32 image, s32 b, s16 x, s16 y, f32 scaleX, f32 scaleY, s32 g);

/* Draws a menu image: a filled box when it is the focused item, the image otherwise. */
void func_004143DC(MenuItem *item, MenuItemLayout *box, MenuItemLayout *area, Menu *window) {
    s32 alpha;
    f32 fx;
    f32 fy;
    f32 t1;
    f32 t2;

    if ((D_8011AAD8.mode == 13 && (item->flags & 0x40000000)) ||
        (D_8011AAD8.multiplayer != 0 && (s32) item->flags < 0 && func_00266B80() == 0x400000)) {
        window->highlighted = 1;
        if (D_800C1BD0_4143DC != 3) {
            D_800C1BD0_4143DC = 3;
        }
        func_0027AF4C();
        t1 = (f32) box->width * box->scaleX;
        t2 = (f32) box->height * box->scaleY;
        fy = (f32) box->top;
        fx = (f32) box->left;
        fx = fx + t1;
        fy = fy + t2;
        if (D_8011AAD8.paused != 0) {
            if (D_8011AAD8.multiplayer != 0) {
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
        func_002778A8(box->left, box->top, (s32) fx, (s32) fy, 1, 0, 0, 0, 0, alpha);
    } else {
        if (D_800C1BD0_4143DC != 3) {
            D_800C1BD0_4143DC = 3;
            func_00279028((s32) (window->fade * (window->alpha * 255.0f)));
        }
        func_0027B320(item->image, 0, (s16) box->left, (s16) box->top, box->scaleX, box->scaleY, 1);
    }
}


/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00440618.c, func_00440618, with Turok 2's layouts. */

extern void func_0041363C_4143DC(MenuItem *item, MenuItemLayout *info) __asm__("func_0041363C");
extern void func_00413F74_4143DC(MenuItem *item, MenuItemLayout *box, MenuItemLayout *column, Menu *window, s32 data) __asm__("func_00413F74");

static __inline__ void fin_4145B0(MenuItem *item, MenuItemLayout *box, MenuItemLayout *column, Menu *window) {
    func_00413F74_4143DC(item, box, column, window, item->data);
}

/* Draws a slider: its label, its bar and its knob at the item's value. */
void func_004145B0(MenuItem *it, MenuItemLayout *box, MenuItemLayout *column, Menu *window) {
    MenuItem cp;
    MenuItemLayout o1;
    MenuItemLayout o2;
    MenuItemLayout o3;
    s32 mid;
    s32 t;

    mid = (column->left + column->right) / 2;
    cp = *it;
    cp.type = 0;
    func_0041363C_4143DC(&cp, &o1);
    cp = *it;
    cp.type = 1;
    cp.data = 0;
    func_0041363C_4143DC(&cp, &o2);
    cp = *it;
    cp.type = 1;
    cp.data = 0;
    func_0041363C_4143DC(&cp, &o3);
    cp = *it;
    cp.data = 0;
    box->top = box->top + o1.height;
    if (it->flags & 1) {
        box->left = mid - ((o2.width + o3.width) / 2);
    }
    box->left = box->left + o3.width / 2;
    func_004143DC(&cp, box, column, window);
    cp = *it;
    cp.data = 0;
    t = it->value * o2.width;
    box->left = box->left + (t / 256 - o3.width / 2);
    box->top = box->top + (o2.height / 2 - o3.height / 2);
    func_004143DC(&cp, box, column, window);
    cp = *it;
    if (it->flags & 1) {
        box->left = mid - o1.width / 2;
    }
    fin_4145B0(&cp, box, column, window);
}


/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_004409A8.c, func_004409A8, with Turok 2's layouts. */

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
extern CIntelligence *func_00225EBC(void *world, s32 type, s32 id);
extern void func_0020F85C(void *m, CVector3 *in, CVector3 *out);
extern void func_00219BC0(CGameObjectInstance *fx, s32 a, s32 b, s32 c, void *d, s32 e, s32 f, f32 g, CVector3 h, s32 i, CVector3 j,
                          CVector3 k, s32 l);
extern void func_0021E5D4(CGameObjectInstance *fx);
extern void func_0021EBC0(CGameObjectInstance *fx);
extern void func_0021A344(CGameObjectInstance *fx);
extern s32 func_00206078(void);
extern void func_00206068(s32 a);
extern void func_002241D8(void *world, CGameObjectInstance *fx, s32 c, s32 d);
extern void func_0020B5D4(void);
extern void func_0020B624(void);
extern void func_0021D39C(CGameObjectInstance *fx, CCamera *cam);
extern void func_004171D0_4143DC(void) __asm__("func_004171D0");
extern void func_004171F4_4143DC(void) __asm__("func_004171F4");

/* Draws a menu item's 3D model, loading it the first time. */
void func_00414938(MenuItem *o, MenuItemLayout *idp, MenuItemLayout *unused, Menu *w) {
    CVector3 v50;
    CVector3 v60;
    CVector3 v70;
    CVector3 v80;
    MenuModel *x;
    MenuModel *x2;
    CCamera *c;
    MenuModelPart *p;
    CGameObjectInstance *fx;
    CIntelligence *src;
    void *s3;
    s32 s2;
    Gfx *d;
    f32 k;
    f32 k2;
    f32 tt;
    f32 ta, tb;

    func_00266C5C(&v50, 0, 12);
    if (o->model != NULL) {
        x = o->model;
        p = o->part;
        fx = &x->instance;
        if (x->loaded != 0) {
            {
                Gfx **pp = &D_800D8E24;
                d = (*pp)++;
                d->w0 = 0xDA380007;
                d->w1 = (u32) &w->camera->m_mProjection[D_800B6D1C];
            }
            c = w->camera;
            k = c->m_Viewport.m_Width / (f32) D_800C2030;
            k2 = c->m_Viewport.m_Height / (f32) D_800C2034;
            v80.x = p->scale.x * k;
            v80.z = p->scale.z * k;
            v80.y = p->scale.y * k2;
            x->height = x->intel.m_Enemy.m_CollisionHeight;
            s3 = NULL;
            if (!(o->flags & 0x2000000)) {
                s2 = p->intelType;
                x2 = o->model;
                src = func_00225EBC(D_800F7078, s2, p->intelId);
                if (src == NULL) {
                    x2->loaded = 0;
                } else {
                    x2->intelType = s2;
                    x2->loaded = 1;
                    x2->intel = *src;
                    s3 = &x2->intel;
                    x2->intel.m_Type = INTELLIGENCE_STATIC;
                    x2->intel.m_dwFlags = 0;
                    x2->intel.m_Enemy.m_dwFlags = 0;
                    x2->intel.m_Enemy.m_Health = 0;
                    x2->intel.m_Enemy.m_StartAnim = 0;
                    x2->intel.m_LODDist2 = 10240.0f;
                    x2->height = x2->intel.m_Enemy.m_CollisionHeight;
                }
            }
            v60 = p->pos;
            v60.y = v60.y - x->height * v80.y * 0.8f;
            func_0020F85C(&w->camera->m_mfViewOrient, &v60, &v70);
            if (s3 != NULL && !(o->flags & 0x2000000)) {
                func_00219BC0(fx, -1, p->objType, D_800B6D00, s3, -1, 0, w->camera->m_ViewParams.m_RotY + 3.141593f, v70, 0, p->scale, v50,
                              0);
                func_0021E5D4(fx);
                o->flags = o->flags | 0x2000000;
            }
            tt = func_002982F0((f32) (u32) (D_800B6D20 * 10) * 0.017453294f);
            ta = w->fade;
            tb = w->alpha;
            D_801308E0 = (s32) (ta * tb * ((tt + 1.0f) * 50.0f + 50.0f));
            if (w->cursor == idp->index) {
                fx->m_AI.m_pfDraw = (void *) func_004171F4_4143DC;
            } else {
                fx->m_AI.m_pfDraw = (void *) func_004171D0_4143DC;
            }
            if (func_00206078() == 0) {
                func_00206068(1);
            }
            if (x->intelType == INTELLIGENCE_PLAYER) {
                func_002241D8(D_800F7078, fx, D_800ADB30[fx->header.m_pIntelligence->m_Id], 1);
            }
            if (D_800C1BD0_4143DC != 4) {
                D_800C1BD0_4143DC = 4;
                {
                    Gfx **pp = &D_800D8E24;
                    Gfx *q = (*pp)++;
                    q->w0 = 0xE7000000;
                    q->w1 = 0;
                }
                {
                    Gfx **pp = &D_800D8E24;
                    Gfx *q = (*pp)++;
                    q->w0 = 0xE3000A01;
                    q->w1 = 0x100000;
                }
                {
                    Gfx **pp = &D_800D8E24;
                    Gfx *q = (*pp)++;
                    q->w0 = 0xE3000C00;
                    q->w1 = 0x80000;
                }
                {
                    Gfx **pp = &D_800D8E24;
                    Gfx *q = (*pp)++;
                    q->w0 = 0xE3001201;
                    q->w1 = 0x2000;
                }
            }
            fx->m_vScale = v80;
            fx->header.m_vPos = v70;
            fx->m_AI.m_vDesiredPos = v70;
            func_0021EBC0(fx);
            fx->header.m_vPos = v70;
            fx->m_AI.m_vDesiredPos = v70;
            func_0021A344(fx);
            fx->m_RotY = fx->m_RotY + p->spin * D_800B6D28;
            fx->header.m_vPos = v70;
            fx->m_AI.m_vDesiredPos = v70;
            func_0020B5D4();
            func_0021D39C(fx, w->camera);
            func_0020B624();
        }
    }
}


/* ---- func_00414EA8 ---- */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00440F10.c, func_00440F10.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern void *func_00416B28_4143DC() __asm__("func_00416B28");
extern void func_00416C00_4143DC() __asm__("func_00416C00");
extern void func_00416C0C_4143DC(void *p) __asm__("func_00416C0C");
/* The colour records a window starts with (Menu colors): 8 records of
 * 0x18 bytes, read as halfwords at 0, 4, 8 and bytes at 0xD, 0x11, 0x15 (the
 * low parts of six words: the symbol sits 2 bytes into the table). */
typedef struct { u16 a; u8 pad2[2]; u16 b; u8 pad6[2]; u16 c; u8 padA[3]; u8 d; u8 pad0E[3]; u8 e; u8 pad12[3]; u8 f; u8 pad16[2]; } MenuColorInit;
extern MenuColorInit D_800C1CD2[];
/* Builds a window in its block: the items from the desc's table, the
 * extras after them, the colours, then the desc's init and the items' select
 * callbacks flagged 0x04000000. */
void func_00414EA8(Menu *p, void *a1, MenuDesc *t, s32 a3, struct CPlayer *a4, ControllerProfile *a5, void *a6, MenuPool *a7)
{
  s32 i;
  s32 j;
  u8 new_var;
  s32 k;
  s32 k2;
  s32 off;
  u8 *e;
  s32 new_var2;
  MenuItem *ib;
  u8 *ext;
  u8 **pc;
  p->cursor = t->cursor;
  p->offsetX = t->offsetX;
  p->offsetY = t->offsetY;
  p->block = a1;
  ib = (MenuItem *) (((u8 *) p) + 0x1D8);
  p->items = ib;
  p->count = t->count;
  ext = (u8 *) ib;
  ext = ((u8 *) ib) + (t->count * 0x28);
  for (i = 0; i < t->count; i++)
  {
    MenuItem *it = (MenuItem *) ((i * 0x28) + ((s32) p->items));
    MenuItemDesc *tp = (MenuItemDesc *) ((i * 0x24) + ((s32) t->items));
    it->index = i;
    pc = &ext;
    {
      it->type = tp->type;
      it->flags = tp->flags | 0x01800000;
      it->x = tp->x;
      it->y = tp->y;
      it->link[0] = tp->link[0];
    }
    it->link[1] = tp->link[1];
    it->link[2] = tp->link[2];
    it->link[3] = tp->link[3];
    new_var2 = tp->data;
    it->def = tp;
    it->value = -1;
    it->menu = p;
    it->data = new_var2;
    k = 0;
    if (tp->type == 3)
    {
      k = 0x330;
    }
    if (k != 0)
    {
      e = *pc;
      it->extra = e;
      if (tp->type == 3)
      {
        ((MenuModel *) e)->loaded = 1;
      }
      k2 = 0;
      if (tp->type == 3)
      {
        k2 = 0x330;
      }
      *pc += k2;
    }
  }
  p->desc = t;
  p->param = a3;
  p->player = a4;
  p->controller = a5;
  p->context = a6;
  p->state = 0;
  p->slot = a7->opened;
  p->time = 0;
  p->fade = 0;
  p->alpha = 0;
  p->visible = 1;
  p->highlighted = 0;
  p->camera = func_00416B28_4143DC(a7);
  p->pool = a7;
  for (j = 0; j < 2; j++)
  {
    for (i = 0; i < 8; i++)
    {
      /* rb + 0x48 is p->colors[j][i] (MenuColor w0..bF); written through
       * the byte offsets, as the struct's fields schedule differently */
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
  a5->buttons = 0;
  a5->buttonsSingle = 0;
  a5->buttonsDouble = 0;
  func_00416C00_4143DC(p);
  func_00416C0C_4143DC(p);
  if (t->init != 0)
  {
    t->init(p, a7);
  }
  {
    MenuItem *it;
    i = p->count;
    it = p->items;
    i--;
    if (i != (-1))
    {
      do
      {
        MenuItemDesc *d = it->def;
        if (d->flags & 0x04000000)
        {
          if (d->select != (off = 0))
          {
            d->select(it, p, a7);
          }
        }
        i--;
        it++;
      }
      while (i != (-1));
    }
  }
}


/* ---- func_004151B4 ---- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043F52C_de_4.c, func_804410AC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */


/* Moves the window's cursor by the selected item's link for dir (0x10 +
 * MENU_LINK_*: the byte at that offset), on until an item that is shown,
 * enabled (0x1800000) and selectable; the item reached links back. */
void func_004151B4(Menu *menu, u8 dir) {
    MenuItem *entry;
    s32 old;
    entry = &menu->items[menu->cursor];
    do {
        old = menu->cursor;
        menu->cursor += ((s8 *)entry)[dir];
        if (menu->cursor < 0) {
            menu->cursor = menu->count - 1;
        } else if (menu->cursor >= menu->count) {
            menu->cursor = 0;
        }
        entry = &menu->items[menu->cursor];
        old -= menu->cursor;
        switch (dir) {
        case 16:
            if (entry->flags & 0x80000) {
                entry->link[MENU_LINK_RIGHT] = old;
            }
            break;
        case 17:
            if (entry->flags & 0x40000) {
                entry->link[MENU_LINK_LEFT] = old;
            }
            break;
        case 18:
            if (entry->flags & 0x200000) {
                entry->link[MENU_LINK_DOWN] = old;
            }
            break;
        case 19:
            if (entry->flags & 0x100000) {
                entry->link[MENU_LINK_UP] = old;
            }
            break;
        }
    } while ((entry->flags & 0x1800000) != 0x1800000 || entry->def->select == NULL);
}


/* ---- func_0041531C ---- */
/* WRONG CUT: real function 0x0041531C, size 0x3E0 (through 0x004156FC): splat cut it into
 * func_0041531C, func_004154C4, func_00415500, func_00415508, func_004155AC,
 * func_0041560C and func_004156D8 at branch targets. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00441384.c, func_00441384, with Turok 2's layouts. */

extern f32 D_800B6D28; /* frame delta */
extern s32 func_002855E8(ControllerProfile *ctl);
extern s32 func_00285628_4143DC(ControllerProfile *ctl) __asm__("func_00285628");
extern s32 func_00285670_4143DC(ControllerProfile *ctl) __asm__("func_00285670");
extern s32 func_002856B8(ControllerProfile *ctl);
extern s32 func_00285700(ControllerProfile *ctl);
extern void func_00416C28_4143DC(Menu *menu) __asm__("func_00416C28");

s32 func_0041531C(Menu *menu, void *arg) {
    MenuItem *item;
    s32 quit;
    s32 i;
    s32 moveV;
    u32 horizontal;

    quit = 0;
    if (menu->desc->poll != NULL) {
        quit = menu->desc->poll();
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
    item = (MenuItem *) ((u8 *) menu->items + menu->cursor * 0x28);
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
        menu->alpha = menu->alpha + D_800B6D28 * 0.4f;
        if (1.0f <= menu->alpha) {
            menu->alpha = 1.0f;
            menu->state = 2;
        }
        break;
    case 3:
        menu->alpha = menu->alpha - D_800B6D28 * 0.4f;
        if (menu->alpha <= 0.0f) {
            menu->alpha = 0.0f;
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
                buttons = menu->controller->buttons;
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
                if (func_00285628_4143DC(menu->controller) != 0) {
                    func_004151B4(menu, 0x10);
                }
                if (func_00285670_4143DC(menu->controller) != 0) {
                    func_004151B4(menu, 0x11);
                }
            } else if (menu->controller->buttons & 0x60303) {
                moveV = 0;
            }
            if (moveV != 0) {
                if (func_002856B8(menu->controller) != 0) {
                    func_004151B4(menu, 0x12);
                }
                if (func_00285700(menu->controller) != 0) {
                    func_004151B4(menu, 0x13);
                }
            }
            if (menu->state == 2 && (func_002855E8(menu->controller) != 0 || (func_00285628_4143DC(menu->controller) != 0 && horizontal != 0)
                                     || (func_00285670_4143DC(menu->controller) != 0 && horizontal != 0))) {
                if (item->def->select == NULL || item->def->select(item, menu, arg) != 0) {
                    menu->state = 3;
                }
            }
        }
    }
    return 0;
}


/* ---- func_004156FC ---- */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00441764.c, func_00441764.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

f32 func_002982F0(f32 x);
extern void func_002778A8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j);
extern f32 func_002982F0(f32 x);
extern s32 D_800C1BD0_4143DC __asm__("D_800C1BD0");
/* Draws a window's items: lays them all out, centres the box around them
 * (the area) on the camera's viewport and the window's offset, highlights
 * the cursor's and calls each item's draw callback. */
void func_004156FC(Menu *p, void *a1) {
    struct { s32 f, w, h, g; } sz;
    struct { s32 f, x0, x1, y0, y1; } r;
    MenuItemLayout info;
    MenuItemLayout cp;
    CCamera *cam;
    MenuItem *it;
    s32 i, v, v2, v3, v4;
    f32 cx, cy, hw, hh, fa, fb, sv, tt;
    u32 al;
    i = 0;
    cam = func_00416B28_4143DC(a1);
    r.x0 = 0x400; r.x1 = -0x400; r.y0 = 0x400; r.y1 = -0x400;
    info.width = 0; info.height = 0; info.left = 0; info.right = 0; info.top = 0; info.bottom = 0;
    for (; i < p->count; i++) {
        it = &p->items[i];
        if (it->flags & 0x800000) {
            if (it->flags & 0x20000000) {
                info.left = 0x400; info.right = -0x400; info.top = 0x400; info.bottom = -0x400;
                info.width = 0; info.height = 0;
            } else {
                func_0041363C_4143DC(it, &info);
            }
            v = info.left; if (r.x0 < v) v = r.x0; r.x0 = v;
            v2 = info.right; if (v2 < r.x1) v2 = r.x1; r.x1 = v2;
            v3 = info.top; if (r.y0 < v3) v3 = r.y0; r.y0 = v3;
            v4 = info.bottom; if (v4 < r.y1) v4 = r.y1; r.y1 = v4;
        }
    }
    sz.w = r.x1 - r.x0;
    sz.h = r.y1 - r.y0;
    if (sz.w >= 0 && sz.h >= 0) {
        cx = cam->m_Viewport.m_ScreenPosX + cam->m_Viewport.m_Width * 0.5f + (f32)p->offsetX;
        cy = cam->m_Viewport.m_ScreenPosY + cam->m_Viewport.m_Height * 0.5f + (f32)p->offsetY;
        info.width = 0; info.height = 0; info.left = 0; info.right = 0;
        hw = (f32)(sz.w / 2);
        hh = (f32)(sz.h / 2);
        info.top = 0; info.bottom = 0;
        r.x0 = (s32)(cx - hw);
        r.x1 = (s32)(cx + hw);
        r.y0 = (s32)(cy - hh);
        r.y1 = (s32)(cy + hh);
        D_800C1BD0_4143DC = -1;
        for (i = 0; i < p->count; i++) {
            it = &p->items[i];
            if (it->flags & 0x800000) {
                info.index = i;
                if (it->flags & 0x20000000) {
                    func_0041363C_4143DC(it, &info);
                    info.width = 0; info.height = 0;
                    info.left = (s32)cam->m_Viewport.m_ScreenPosX;
                    info.right = 0;
                    info.top = (s32)cam->m_Viewport.m_ScreenPosY;
                    info.bottom = 0;
                } else {
                    func_0041363C_4143DC(it, &info);
                }
                cp = info;
                if (!(it->flags & 0x20000000)) {
                    cp.left += r.x0;
                    cp.right += r.x0;
                    cp.top += r.y0;
                    cp.bottom += r.y0;
                }
                if (it->flags & 1) {
                    cp.left = (s32)(cx - (f32)(info.width / 2));
                    cp.right = cp.left + info.width;
                }
                if (it->flags & 2) {
                    cp.left = r.x0;
                    cp.right = r.x0 + info.width;
                }
                if (it->flags & 4) {
                    cp.right = r.x1;
                    cp.left = r.x1 - info.width;
                }
                if (it->flags & 0x800000) {
                    if (p->cursor == i && it->type != 3) {
                        if (!(it->flags & 8)) {
                            fb = (f32)cp.top;
                            fa = (f32)cp.bottom;
                            fa = fb + (fa - fb);
                            func_0027AF4C(it);
                            tt = func_002982F0((f32)((u32) D_800B6D20 * 10) * 0.017453294f);
                            sv = p->fade * p->alpha * ((tt + 1.0f) * 50.0f + 50.0f);
                            if (it->flags & 0x10) {
                                func_002778A8(cp.left - 4, (s32)fb, cp.right + 4, (s32)fa, 0, 0, 0xFF, 0, 10, (u32)sv & 0xFF);
                            } else {
                                func_002778A8(cp.left - 4, (s32)fb, cp.right + 4, (s32)fa, 0, 0, 0xFF, 0, 10, (u32)sv & 0xFF);
                            }
                            D_800C1BD0_4143DC = -1;
                        }
                    }
                    it->def->draw(it, &cp, (MenuItemLayout *) &sz, p); /* sz and r: the area, one MenuItemLayout */
                }
            }
        }
    }
}

/* ---------------- func_00415CE8 ---------------- */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00441D50.c, func_00441D50.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */






extern void func_0041363C_415CE8(MenuItem *it, void *info) __asm__("func_0041363C");
extern void func_0027AF4C_415CE8(void) __asm__("func_0027AF4C");
extern void func_002778A8_415CE8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j) __asm__("func_002778A8");
extern s32 D_800C2030;
extern s32 D_800C2034;
/* Draws the bar behind a window's line of items (flag 0x08000000 text):
 * the screen's width, at the window's offset (c: from the bottom). */
void func_00415CE8(Menu *a, MenuItem *b, s32 c) {
    struct { s32 f, w, h, g; } sz;
    struct { s32 f, x0, x1, y0, y1; } r;
    MenuItemLayout info;
    CCamera *cam;
    s32 i, v, v2, v3, v4;
    f32 fu; register f32 base asm("$f1"); f32 half, t, y0, k;
    u32 al;
    cam = a->camera;
    switch (b->flags & 0x3FE0) {
    case 0x20: { register s32 hi asm("$2"); s32 lo; hi = 0x400; lo = -0x400; r.x0 = hi; r.x1 = lo; r.y0 = hi; r.y1 = lo; } break;
    case 0x40: { register s32 hi asm("$2"); s32 lo; hi = 0x400; lo = -0x400; r.x0 = hi; r.x1 = lo; r.y0 = hi; r.y1 = lo; } break;
    case 0x200: { register s32 hi asm("$2"); s32 lo; hi = 0x400; lo = -0x400; r.x0 = hi; r.x1 = lo; r.y0 = hi; r.y1 = lo; } break;
    default: { register s32 hi asm("$2"); s32 lo; hi = 0x400; lo = -0x400; r.x0 = hi; r.x1 = lo; r.y0 = hi; r.y1 = lo; } break;
    }
    info.width = 0; info.height = 0; info.left = 0; info.right = 0; info.top = 0; info.bottom = 0;
    for (i = 0; i < a->count; i++) {
        func_0041363C_415CE8(a->items + i, &info);
        v = info.left; if (r.x0 < v) v = r.x0; r.x0 = v;
        v2 = info.right; if (v2 < r.x1) v2 = r.x1; r.x1 = v2;
        v3 = info.top; if (r.y0 < v3) v3 = r.y0; r.y0 = v3;
        v4 = info.bottom; if (v4 < r.y1) v4 = r.y1; r.y1 = v4;
    }
    sz.w = r.x1 - r.x0;
    sz.h = r.y1 - r.y0;
    if (sz.w >= 0 && sz.h >= 0) {
        base = cam->m_Viewport.m_ScreenPosX + cam->m_Viewport.m_Width * 0.5f + (f32)a->offsetX;
        sz.w = D_800C2030;
        half = (f32)(D_800C2030 / 2);
        r.x0 = (s32)(base - half);
        r.x1 = (s32)(base + half);
        y0 = (f32)D_800C2034;
        t = y0 * 0.21621622f;
        t -= (D_800C2034 >= 0xDF) ? 13.0f : 9.0f;
        if (c != 0) y0 = (cam->m_Viewport.m_ScreenPosY + (f32)D_800C2034 + (f32)a->offsetY) - t;
        else { register f32 pa asm("$f0"); f32 pb; pa = cam->m_Viewport.m_ScreenPosY; pb = (f32)a->offsetY; y0 = pa + pb; }
        r.y0 = (s32)y0;
        r.y1 = (s32)((f32)r.y0 + t);
        sz.h = r.y1 - r.y0;
        func_0027AF4C_415CE8();
        func_002778A8_415CE8(r.x0, r.y0, r.x1, r.y1, 1, 0, 0, 0, 0, (u32)(a->fade * 192.0f) & 0xFF);
    }
}

/* ---------------- func_00415FB8 ---------------- */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00442020.c, func_00442020.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */



typedef struct { s16 unk0; u8 pad[0x22]; } O06I_415FB8;
typedef struct { O06I_415FB8 *unk0; s16 unk4; u8 pad6[0x1A]; s32 unk20; } O06B_415FB8;
typedef struct { u8 pad[0x14]; s16 unk14; } O06A_415FB8;
typedef struct { u8 pad[0x18]; O06B_415FB8 *unk18; s32 unk1C; s32 unk20; s32 unk24; } O06C_415FB8;
/* Static inline in the Rage Wars source file (dropped by the importer); 0x330 in Turok 2, 0x480 in Rage Wars. */
static inline s32 O06_ItemSize_415FB8(s16 *a) { if (*a == 3) return 0x330; return 0; }
extern s32 *func_00204EDC();
extern void func_00414EA8_415FB8() __asm__("func_00414EA8");
extern void CList__AddHead(O06A_415FB8 *a, s32 b);
s32 func_00415FB8(O06A_415FB8 *a, O06C_415FB8 *c) {
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

/* ---------------- func_004160F0 ---------------- */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00442158.c, func_00442158, with Turok 2's layouts. */

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 type;
    /* 0x06 */ u8 pad6[0xE];
    /* 0x14 */ u8 *text;
} MenuItemTmpl_4160F0;

extern u8 *func_00413230_4160F0(void) __asm__("func_00413230");

/* Length of the item's text up to the end or the first newline. */
s32 func_004160F0(MenuItemTmpl_4160F0 *a) {
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

/* ---- The window functions below use Menu, MenuDesc, ControllerProfile and MenuPool (structs/menu.h) ---- */

/* ---------------- func_00416168 ---------------- */
s32 func_00413F74_416168(s32, s32, s32, s32, s32) __asm__("func_00413F74");

void func_00416168(Menu *arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_00413F74_416168((s32) arg0, arg1, arg2, arg3, (s32) arg0->desc);
}

/* ---------------- func_00416188 ---------------- */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00442224.c, func_00442224, with Turok 2's layouts. */

typedef struct {
    /* 0x00 */ u8 pad0[0x17];
    /* 0x17 */ u8 ch;
} MenuItemTmpl_416188;

extern void func_00413F74_416188(MenuItemTmpl_416188 *a, s32 b, s32 c, s32 d, u8 *text) __asm__("func_00413F74");

/* Draws the item's single character. */
void func_00416188(MenuItemTmpl_416188 *a, s32 b, s32 c, s32 d) {
    u8 buf[2];

    buf[0] = a->ch;
    buf[1] = 0;
    func_00413F74_416188(a, b, c, d, buf);
}

/* ---------------- func_004161B4 ---------------- */
s32 func_00413230_4161B4(s32) __asm__("func_00413230");
s32 func_00413F74_4161B4(s32, s32, s32, s32, s32) __asm__("func_00413F74");

void func_004161B4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_00413F74_4161B4(arg0, arg1, arg2, arg3, func_00413230_4161B4(arg0));
}

/* ---------------- func_00416214 ---------------- */
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

MenuBlob *func_00416214(MenuItem_416214 *a, s32 id, s32 arg) {
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

/* ---------------- func_004162E4 ---------------- */
s32 func_004162E4(Menu *arg0) {
    ControllerProfile *temp_v1;
    s32 var_v0;

    temp_v1 = arg0->controller;
    var_v0 = 0;
    if (temp_v1 != 0) {
        var_v0 = (s32) &temp_v1->buttonHistory[29]; /* 0xA0 */
    }
    return var_v0;
}

/* ---------------- func_004162FC ---------------- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043F52C_de_6.c, func_8044222C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern float func_004162FC(int c, unsigned char next, float sx, float sy);
f32 func_004162FC(s32 arg0, u8 arg1, f32 arg2, f32 arg3) {
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

/* ---------------- func_004163C0 ---------------- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043F52C_de_6.c, func_804422F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern float func_004162FC_4163C0(int c, unsigned char next, float sx, float sy) __asm__("func_004162FC");
f32 func_004162FC_4163C0(s32 arg0, u8 arg1, f32 arg2, f32 arg3) __asm__("func_004162FC");
f32 func_004163C0(u8 *p, f32 sx, f32 sy) {
    f32 total;
    u8 c;
    total = 0.0f;
    while (p != 0 && (c = *p) != 0 && c != '\n') {
        p++;
        total += func_004162FC_4163C0(c, *p, sx, sy);
    }
    return total;
}

/* ---------------- func_00416454 ---------------- */
s32 func_00415FB8_416454(s32, s32) __asm__("func_00415FB8");

s32 func_00416454(s32 arg0, s32 arg1, s32 arg2) {
    return func_00415FB8_416454(arg2, arg1) != 0;
}

/* ---------------- func_00416474 ---------------- */
s32 func_00416474(void) {
    return 0;
}

/* ---------------- func_0041647C ---------------- */
s32 func_0041647C(Menu *arg0) {
    return arg0->desc->value;
}

/* ---------------- func_0041648C ---------------- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043F52C_de_6.c, func_804423BC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern int func_00285628_41648C(struct ControllerProfile *object) __asm__("func_00285628");
extern int func_00285670_41648C(struct ControllerProfile *object) __asm__("func_00285670");
s32 func_002855E8_41648C(s32) __asm__("func_002855E8");
s32 func_0041648C(func_8022A404_S1 *menu,s32 value,s32 step,s32 minimum,s32 maximum,s32 wrap) {
 if(((s32 (*)(s32))func_00285628_41648C)(menu->unk20))value-=step;
 if(((s32 (*)(s32))func_00285670_41648C)(menu->unk20)||func_002855E8_41648C(menu->unk20))value+=step;
 if(value<minimum) {
  value=minimum;if(wrap)value=maximum;
 } else if(value>maximum) {
  value=maximum;if(wrap)value=minimum;
 }
 return value;
}

/* ---------------- func_00416558 ---------------- */
s32 func_00285628_416558(s32) __asm__("func_00285628");
s32 func_00285670_416558(s32) __asm__("func_00285670");
s32 func_00416558(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
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

/* ---------------- func_00416614 ---------------- */
void CList__Construct(s32, s32, s32);

void func_00416614(s32 arg0) {
    CList__Construct(arg0, 0x1D0, 0x1D4);
    ((MenuPool *) arg0)->opened = 0;
}

/* ---------------- func_00416644 ---------------- */
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
extern void func_00414EA8_416644() __asm__("func_00414EA8");
extern void CList__AddHead_416644(O06A_416644 *a, s32 b) __asm__("CList__AddHead");
s32 func_00416644(O06A_416644 *a, O06B_416644 *b, s32 c, s32 d, s32 e) {
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

/* ---------------- func_00416760 ---------------- */
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
extern void func_00414EA8_416760() __asm__("func_00414EA8");
extern void CList__AddHead_416760(O06A_416760 *a, s32 b) __asm__("CList__AddHead");
s32 func_00416760(O06A_416760 *a, O06C_416760 *c, O06B_416760 *b) {
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

/* ---------------- func_00416894 ---------------- */
/* WRONG CUT: real function 0x00416894, size 0x134 (through 0x004169C8): splat cut its
 * epilogue off as func_004169A0. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00442020.c, func_00442020, with Turok 2's layouts
 * (the template is passed directly instead of read from the caller). */

/* Each item of type 3 gets a 0x330-byte extension after the item array. */
static __inline__ s32 ItemExtSize(s16 *type) {
    if (*type == 3) {
        return 0x330;
    }
    return 0;
}

extern s32 *func_00204EDC_416894(s32, s32, s32, void *) __asm__("func_00204EDC");
extern void func_00414EA8_416894(s32 menu, s32 *mem, MenuDesc *t, s32, s32, s32, s32, MenuPool *pool) __asm__("func_00414EA8");
extern void CList__AddHead_416894(MenuPool *a, s32 b) __asm__("CList__AddHead");

s32 func_00416894(MenuPool *pool, Menu *c, MenuDesc *t) {
    s32 size, i;
    s32 *r;
    s32 s;
    s32 p1, p2, p3, p4;

    p1 = c->desc;
    p2 = c->player;
    p3 = c->controller;
    p4 = c->context;
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

/* ---------------- func_004169C8 ---------------- */
s32 CList__Remove(s32, s32);
s32 func_002052D8(s32, s32);
s32 func_0041531C_4169C8(s32, s32) __asm__("func_0041531C");

extern MenuPool D_80119E2C;

void func_004169C8(MenuPool *arg0) {
    s32 (*temp_v0)(Menu *, MenuPool *);
    Menu *temp_s2;
    ControllerProfile *temp_v0_2;
    Menu *var_s0;

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
                    temp_v0 = var_s0->desc->close;
                    if (temp_v0 != NULL) {
                        temp_v0(var_s0, arg0);
                    }
                    temp_v0_2 = var_s0->controller;
                    temp_v0_2->buttons = 0;
                    temp_v0_2->buttonsSingle = 0;
                    temp_v0_2->buttonsDouble = 0;
                    CList__Remove((s32) arg0, (s32) var_s0);
                    func_002052D8(0, var_s0->block);
                }
            }
            var_s0 = temp_s2;
        } while (var_s0 != NULL);
    }
}

/* ---------------- func_00416AA4 ---------------- */
void func_00416AA4(MenuPool *arg0) {
    Menu *var_s0;

    var_s0 = arg0->tail;
    if (var_s0 != NULL) {
        do {
            var_s0->desc->update(var_s0, arg0);
            var_s0 = var_s0->prev;
        } while (var_s0 != NULL);
    }
}

/* ---------------- func_00416AF8 ---------------- */
s32 func_00416AF8(MenuPool *arg0) {
    s32 var_v1;
    Menu *var_a0;

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

/* ---------------- func_00416B28 ---------------- */
s32 func_00416B28(s32 arg0) {
    return arg0 - 0x53C;
}

/* ---------------- func_00416B30 ---------------- */
/* WRONG CUT: real function 0x00416B30, size 0x7C (through 0x00416BAC): splat cut the loop
 * off as func_00416B44. */

extern void CList__Remove_416B30(MenuPool *list, Menu *item) __asm__("CList__Remove");
extern void func_002052D8_416B30(s32, void *mem) __asm__("func_002052D8");

/* Closes and frees every open menu. */
void func_00416B30(MenuPool *pool) {
    Menu *menu;
    ControllerProfile *ctl;

next:
    menu = pool->head;
    if (menu != NULL) {
        if (menu->desc->close != NULL) {
            menu->desc->close(menu, pool);
        }
        ctl = menu->controller;
        ctl->buttons = 0;
        ctl->buttonsSingle = 0;
        ctl->buttonsDouble = 0;
        CList__Remove_416B30(pool, menu);
        func_002052D8_416B30(0, menu->block);
        goto next;
    }
}

/* ---------------- func_00416BAC ---------------- */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 type;
    /* 0x06 */ u8 pad6[0xE];
    /* 0x14 */ u8 *text;
} MenuItemTmpl_416BAC;

extern u8 *func_00413230_416BAC(void) __asm__("func_00413230");
extern f32 func_004163C0_416BAC(u8 *text, f32 sx, f32 sy) __asm__("func_004163C0");

/* Width of the item's text at the given scale. */
f32 func_00416BAC(MenuItemTmpl_416BAC *a, f32 scale) {
    u8 *s;

    if (a->type == 5) {
        s = func_00413230_416BAC();
    } else {
        s = a->text;
    }
    return func_004163C0_416BAC(s, scale, scale);
}

/* ---------------- func_00416C00 ---------------- */
void func_00416C00(Menu *arg0) {
    arg0->flashStep = 0;
    arg0->flash = 0;
}

/* ---------------- func_00416C0C ---------------- */
void func_00416C0C(Menu *arg0) {
    if (arg0->flashStep == 0) {
        arg0->flashStep = 1;
        arg0->flash = 0;
    }
}

/* ---------------- func_00416C28 ---------------- */
void func_00416C28(Menu *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->flash + arg0->flashStep;
    arg0->flash = temp_v0;
    if (temp_v0 >= 0x30) {
        arg0->flash = 0;
        arg0->flashStep = 0;
    }
}

/* ---------------- func_00416C54 ---------------- */
typedef struct {
    char pad0[0x1C8];
    s32 unk_1C8; /* 0x1C8 */
} Obj1C8;

s32 func_00416C54(Obj1C8 *obj) {
    s32 v = obj->unk_1C8;
    if (v < 0x18) {
        return v;
    }
    return 0x2F - v;
}

/* ---------------- func_00416C74 ---------------- */
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
void func_00416C74(O06A_416C74 *a) {
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

/* ---------------- func_00416DA4 ---------------- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80442BC8.CPP, func_80442CD8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */


extern s32 func_00416DA4(s16 *record);
s32 func_00416DA4(s16 *record) {
    if (*record == 3) {
        return 0x330;
    }
    return 0;
}

/* ---------------- func_00416DC0 ---------------- */
typedef struct {
    /* 0x000 */ u8 pad0[0x328];
    /* 0x328 */ s32 unk328;
} MenuExtra_416DC0;

void func_00416DC0(s16 *arg0, MenuExtra_416DC0 *arg1) {
    if (*arg0 == 3) {
        arg1->unk328 = 1;
    }
}

/* ---------------- func_00416DDC ---------------- */
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
extern void func_00416DDC(CMenuItem *w, WidgetDescriptor *d, char **arena, int id, int arg);
void func_00416DDC(CMenuItem *w, WidgetDescriptor *d, char **arena, int id, int arg) {
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

/* ---------------- func_00416EA8 ---------------- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80442A58_de.c, func_80442DDC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern char D_800C1BD4_416EA8[] __asm__("D_800C1BD4");
void *func_00416EA8(ObjRecAt8V4 *arg0) {
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

/* ---------------- func_00416F8C ---------------- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80442EC0_de.c, func_80442EC0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

f32 func_00416F8C(ItemRecAt14 *arg0) {
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

/* ---------------- func_0041707C ---------------- */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80442FB0_de.c, func_80442FB0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

void func_0041707C(ItemRecAt14 *item,float value) {
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

/* ---------------- func_004171D0 ---------------- */
s32 func_0021ECA0(s32, s32, s32, s32);

extern s32 D_800AB0E8;

void func_004171D0(s32 arg0, s32 arg1, s32 arg2) {
    func_0021ECA0(arg0, arg1, arg2, (s32) &D_800AB0E8);
}

/* ---------------- func_004171F4 ---------------- */
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
void func_004171F4(O06A_4171F4 *a, s32 b, s32 c) {
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

/* ---------------- func_004172B0 ---------------- */
void func_004172B0(Menu *arg0) {
    s32 (*temp_v0)();
    ControllerProfile *temp_v0_2;

    temp_v0 = arg0->desc->close;
    if (temp_v0 != NULL) {
        temp_v0();
    }
    temp_v0_2 = arg0->controller;
    temp_v0_2->buttons = 0;
    temp_v0_2->buttonsSingle = 0;
    temp_v0_2->buttonsDouble = 0;
}

/* ---------------- func_004172F8 ---------------- */
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
extern void func_00414EA8_4172F8(void *, void **, struct Dialog *, s32, s32, s32, s32, struct func_8025E5B0_S1 *) __asm__("func_00414EA8");
void CList__AddHead_4172F8(void *list, void *node) __asm__("CList__AddHead");
extern void **func_00204EDC_4172F8(s32, s32, s32, char *) __asm__("func_00204EDC");
void *func_004172F8(struct func_8025E5B0_S1 *owner, struct Dialog *list, s32 a, s32 b, s32 c, s32 d) {
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

/* ---------------- func_00417418 ---------------- */
s32 CList__Remove_417418(s32, s32) __asm__("CList__Remove");
s32 func_002052D8_417418(s32, s32) __asm__("func_002052D8");

void func_00417418(s32 arg0, Menu *arg1) {
    s32 (*temp_v0)(Menu *, s32);
    ControllerProfile *temp_v0_2;

    temp_v0 = arg1->desc->close;
    if (temp_v0 != NULL) {
        temp_v0(arg1, arg0);
    }
    temp_v0_2 = arg1->controller;
    temp_v0_2->buttons = 0;
    temp_v0_2->buttonsSingle = 0;
    temp_v0_2->buttonsDouble = 0;
    CList__Remove_417418(arg0, (s32) arg1);
    func_002052D8_417418(0, arg1->block);
}
