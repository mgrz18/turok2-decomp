#include "common.h"

/* One file: func_004143DC, func_004145B0 and func_00414938. func_00414938's
 * .rodata (0x800C0BEC) starts 4 past an 8-byte boundary, right after
 * func_004143DC's literal, so its pool continues this file's. Each part keeps
 * its own types (suffixed by address) from when it was matched alone. */

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

extern GameState_4143DC D_8011AAF0;
extern s32 D_800C1BD0; /* current draw mode */

extern s32 func_00266B80(void);
extern void func_0027AF4C(void);
extern void func_002778A8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j);
extern void func_00279028(s32 alpha);
extern void func_0027B320(s32 image, s32 b, s16 x, s16 y, f32 scaleX, f32 scaleY, s32 g);

/* Draws a menu image: a filled box when it is the focused item, the image otherwise. */
void func_004143DC(MenuItem_4143DC *item, MenuBox_4143DC *box, s32 unused, MenuWindow_4143DC *window) {
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

/* Layout of an item (func_0041363C). */
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

extern void func_0041363C(MenuItem_4145B0 *item, MenuInfo_4145B0 *info);
extern void func_00413F74(MenuItem_4145B0 *item, MenuBox_4145B0 *box, MenuColumn_4145B0 *column, s32 window, s32 data);

static __inline__ void fin_4145B0(MenuItem_4145B0 *item, MenuBox_4145B0 *box, MenuColumn_4145B0 *column, s32 window) {
    func_00413F74(item, box, column, window, item->data);
}

/* Draws a slider: its label, its bar and its knob at the item's value. */
void func_004145B0(MenuItem_4145B0 *it, MenuBox_4145B0 *box, MenuColumn_4145B0 *column, s32 window) {
    MenuItem_4145B0 cp;
    MenuInfo_4145B0 o1;
    MenuInfo_4145B0 o2;
    MenuInfo_4145B0 o3;
    s32 mid;
    s32 t;

    mid = (column->left + column->right) / 2;
    cp = *it;
    cp.kind = 0;
    func_0041363C(&cp, &o1);
    cp = *it;
    cp.kind = 1;
    cp.data = 0;
    func_0041363C(&cp, &o2);
    cp = *it;
    cp.kind = 1;
    cp.data = 0;
    func_0041363C(&cp, &o3);
    cp = *it;
    cp.data = 0;
    box->y = box->y + o1.height;
    if (it->flags & 1) {
        box->x = mid - ((o2.width + o3.width) / 2);
    }
    box->x = box->x + o3.width / 2;
    func_004143DC((void *) &cp, (void *) box, (s32) column, (void *) window);
    cp = *it;
    cp.data = 0;
    t = it->value * o2.width;
    box->x = box->x + (t / 256 - o3.width / 2);
    box->y = box->y + (o2.height / 2 - o3.height / 2);
    func_004143DC((void *) &cp, (void *) box, (s32) column, (void *) window);
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
extern s32 D_800C1BD0; /* current draw mode */
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
extern void func_004171D0(void);
extern void func_004171F4(void);

/* Draws a menu item's 3D model, loading it the first time. */
void func_00414938(MenuItem_414938 *o, MenuSelection_414938 *idp, s32 unused, MenuWindow_414938 *w) {
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
                fx->draw = (void *) func_004171F4;
            } else {
                fx->draw = (void *) func_004171D0;
            }
            if (func_00206078() == 0) {
                func_00206068(1);
            }
            if (x->id == 0xB) {
                func_002241D8(D_800F7078, fx, D_800ADB30[fx->sub->h0C], 1);
            }
            if (D_800C1BD0 != 4) {
                D_800C1BD0 = 4;
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
