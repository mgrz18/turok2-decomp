/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A6170.c, func_002A6170.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "gfx.h"
#include "globals/render.h"

typedef struct VInfo {
    u8 pad0[0x18];
    s16 x;
    s16 y;
    s16 z;
} VInfo;
typedef struct VNode VNode;
struct VNode {
               u8 pad0[4];
               VNode *next;
               VInfo info;
};
typedef struct VList {
    VNode *head;
    u8 pad4[0x10];
} VList;
typedef struct VCtx {
                 u8 pad0[0x94D8];
                 VList lists[10];
                 u8 pad95A0[0x10];
                 s32 mode;
} VCtx;
typedef struct Vec3fv { f32 x, y, z; } Vec3fv;
extern u8 D_800ACFC8[];
extern u8 D_8011AD48[];
extern f32 gCameraVisibleBox[6]; /* local type: the CBox read as six floats by index */
void Render_InvalidateModeCache(void);
void Texture_InvalidateCache(void);
void CList__Remove(VList *, VNode *);
void CList__AddTail(void *, VNode *);
void func_00235C30(VCtx *c) {
    s32 i;
    VList *l;
    VNode *n, *next;
    Vec3fv v;
    VInfo *p;
    if (c->mode == 100) {
        return;
    }
    { Gfx *_g = gDList++; _g->w0 = (0xE7000000); _g->w1 = (0); }
    { Gfx *_g = gDList++; _g->w0 = (0xE3000A01); _g->w1 = (0x100000); }
    { Gfx *_g = gDList++; _g->w0 = (0xFD900000); _g->w1 = ((u32)gShadowTexture); }
    { Gfx *_g = gDList++; _g->w0 = (0xF5900000); _g->w1 = (0x07080200); }
    { Gfx *_g = gDList++; _g->w0 = (0xE6000000); _g->w1 = (0); }
    { Gfx *_g = gDList++; _g->w0 = (0xF3000000); _g->w1 = (0x0707F400); }
    { Gfx *_g = gDList++; _g->w0 = (0xE7000000); _g->w1 = (0); }
    { Gfx *_g = gDList++; _g->w0 = (0xF5880400); _g->w1 = (0x00080200); }
    { Gfx *_g = gDList++; _g->w0 = (0xF2000000); _g->w1 = (0x0003C03C); }
    { Gfx *_g = gDList++; _g->w0 = (0xFA000000); _g->w1 = (-0x38); }
    { Gfx *_g = gDList++; _g->w0 = (0xDE000000); _g->w1 = ((u32)D_800ACFC8); }
    for (i = 0; i < 10; i++) {
        l = &c->lists[i];
        for (n = l->head; n != ((void *)0); n = next) {
            next = n->next;
            p = &n->info;
            v.x = p->x;
            v.y = p->y;
            v.z = p->z;
            if (!(v.x < gCameraVisibleBox[3] && v.y < gCameraVisibleBox[4] && v.z < gCameraVisibleBox[5] && gCameraVisibleBox[0] <= v.x && gCameraVisibleBox[1] <= v.y && gCameraVisibleBox[2] <= v.z)) {
                CList__Remove(l, n);
                CList__AddTail((u8 *)c + 0x95A0, n);
            } else {
                { Gfx *_g = gDList++; _g->w0 = (0xDA380003); _g->w1 = ((u32)p); }
                { Gfx *_g = gDList++; _g->w0 = (0x01004008); _g->w1 = ((u32)D_8011AD48); }
                { Gfx *_g = gDList++; _g->w0 = (0x06000204); _g->w1 = (0x00040600); }
            }
        }
    }
    Render_InvalidateModeCache();
    Texture_InvalidateCache();
}
