#include "common.h"
#include "rw/abu_types.h"

/* The second VM module's copy of src/us/code/func_004143DC.c, byte for byte,
 * at the same link address (named 0x44D3DC-0x44ECE8, linked 0x4143DC-0x415CE8):
 * the same source, with the functions under their second-module names. Its
 * .rodata goes NOLOAD over the first module's (versions/shared_rodata.us.txt):
 * func_0044E6FC's double is 8-aligned only counted from the file's start, as
 * in the original. */

/* One file from func_0044D3DC to func_0044E6FC (0x4143DC-0x415CE8). Its pools
 * run on from one function to the next: func_0044D938's (0x800C0BEC) starts 4
 * past an 8-byte boundary right after func_0044D3DC's literal, and
 * func_0044E6FC's double at 0x800C0C28 is 8-aligned only counted from this
 * file's start (0x800C0BE8). func_0044DEA8, func_0044E1B4 and func_0044E31C
 * do not start 16-aligned, so they cannot open an object of their own.
 * Each part keeps its own types (suffixed by address) from when it was
 * matched alone.
 *
 * The object is larger still: objects start 16-aligned in .text and in
 * .rodata (with junk words before), the nearest such start is 0x413230
 * (after stray_00413228; its pool opens at 0x800C0AF0 after the junk at
 * 0x800C0AE8), and the string "menu" (0x800C0C54) is shared by
 * func_00415FB8, func_00416644, func_00416760, func_00416894 and
 * func_004172F8. So the file most likely runs 0x413230-0x417484 with
 * .rodata 0x800C0AF0-0x800C0E64; folding the rest in is still to do. */

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
extern void *func_00416B28();
extern void func_00416C00();
extern void func_00416C0C(void *p);
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
  p->f40 = (s32) func_00416B28(a7);
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
  func_00416C00(p);
  func_00416C0C(p);
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
 * func_0044E31C, func_004154C4, func_00415500, func_00415508, func_004155AC,
 * func_0041560C and func_004156D8 at branch targets. */
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
extern s32 func_00285628(MenuController_41531C *ctl);
extern s32 func_00285670(MenuController_41531C *ctl);
extern s32 func_002856B8(MenuController_41531C *ctl);
extern s32 func_00285700(MenuController_41531C *ctl);
extern void func_00416C28(Menu_41531C *menu);

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
    func_00416C28(menu);
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
                if (func_00285628(menu->ctl) != 0) {
                    func_0044E1B4((struct MenuRecAt10 *) menu, 0x10);
                }
                if (func_00285670(menu->ctl) != 0) {
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
            if (menu->state == 2 && (func_002855E8(menu->ctl) != 0 || (func_00285628(menu->ctl) != 0 && horizontal != 0)
                                     || (func_00285670(menu->ctl) != 0 && horizontal != 0))) {
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
extern s32 D_800C1BD0;
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
    cam = func_00416B28(a1);
    r.x0 = 0x400; r.x1 = -0x400; r.y0 = 0x400; r.y1 = -0x400;
    info.f4 = 0; info.f8 = 0; info.x0 = 0; info.x1 = 0; info.y0 = 0; info.y1 = 0;
    for (; i < p->n; i++) {
        it = &p->items[i];
        if (it->flags & 0x800000) {
            if (it->flags & 0x20000000) {
                info.x0 = 0x400; info.x1 = -0x400; info.y0 = 0x400; info.y1 = -0x400;
                info.f4 = 0; info.f8 = 0;
            } else {
                func_0041363C((MenuItem_4145B0 *) it, (MenuInfo_4145B0 *) &info);
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
        D_800C1BD0 = -1;
        for (i = 0; i < p->n; i++) {
            it = &p->items[i];
            if (it->flags & 0x800000) {
                info.f0 = i;
                if (it->flags & 0x20000000) {
                    func_0041363C((MenuItem_4145B0 *) it, (MenuInfo_4145B0 *) &info);
                    info.f4 = 0; info.f8 = 0;
                    info.x0 = (s32)cam->f290;
                    info.x1 = 0;
                    info.y0 = (s32)cam->f294;
                    info.y1 = 0;
                } else {
                    func_0041363C((MenuItem_4145B0 *) it, (MenuInfo_4145B0 *) &info);
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
                            D_800C1BD0 = -1;
                        }
                    }
                    ((Q04DefB *)it->def)->f14(it, &cp, &sz, p);
                }
            }
        }
    }
}
