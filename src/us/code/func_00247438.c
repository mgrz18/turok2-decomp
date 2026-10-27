#include "common.h"
#include "gfx.h"

typedef struct { f32 x, y, z; } Vec3f;
typedef struct { s32 m[4][4]; } Mtx64;   /* a packed N64 Mtx, 0x40 bytes */

/* A model shown in the HUD (the weapon held up to the view). */
typedef struct HudModel {
    /* 0x00 */ s32 current;
    /* 0x04 */ s32 requested;
    /* 0x08 */ s32 object;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ Mtx64 mtx[2];        /* one per frame buffer */
    /* 0x90 */ f32 scaleX;
    /* 0x94 */ f32 scaleY;
    /* 0x98 */ f32 scaleZ;
    /* 0x9C */ f32 rotX;
    /* 0xA0 */ f32 rotY;
    /* 0xA4 */ u8 padA4[4];
    /* 0xA8 */ Vec3f pos;
    /* 0xB4 */ s32 state;
    /* 0xB8 */ f32 time;
} HudModel;

typedef struct {
    /* 0x000 */ u8 pad0[0x288];
    /* 0x288 */ f32 width;
    /* 0x28C */ f32 height;
    /* 0x290 */ f32 left;
    /* 0x294 */ f32 top;
} HudView;

extern u8 D_800AB8CC;
extern u8 D_800AB8CD;
extern u8 D_800AB0D0[];     /* lights */
extern s32 D_800B6D1C;      /* frame buffer index */
extern s32 D_800C2038;
extern f32 D_800ADDB8;
extern u8 D_800F6CB0[];

void func_0020B5B8(void);
void func_0020B5D4(void);
void func_0020B624(void);
void func_0028852C(void *vp, s32 left, s32 right, s32 top, s32 bottom);
void func_0020FC74(f32 *mtx, f32 x, f32 y, f32 z);
void func_00210968(f32 *mtx, f32 angle);
void func_00210790(f32 *mtx, f32 angle);
void func_00210318(f32 *mtx, f32 x, f32 y, f32 z);
void func_00210C3C(f32 *mtx);
void func_0020FF80(f32 *dst, f32 *src);
void func_00210478(f32 *mtx, f32 x, f32 y, f32 z);
void func_0020D274(f32 *src, Mtx64 *dst);
void func_0020F85C(f32 *mtx, Vec3f *in, Vec3f *out);
void func_0020B7F8(s32 key, void *matrix, void *lights, s32 lod, s32 pass);


void func_00247438(HudModel *m, HudView *view) {
    f32 mf[4][4];
    f32 mfDraw[4][4];
    Vec3f out;

    func_0020B5B8();
    if (m->object == 0 || m->state == 0) {
        return;
    }
    D_800AB8CC = 1;
    D_800AB8CD = 0x20;
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xE3001801;
        g->w1 = 0xC0;
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xE3000C00;
        g->w1 = 0x80000;
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xE3001201;
        g->w1 = 0x2000;
    }
    func_0020B5D4();
    func_0028852C(D_800F6CB0, view->left, view->width + view->left, view->top, view->height + view->top);
    func_0020FC74(&mf[0][0], m->scaleX, m->scaleY, m->scaleZ);
    func_00210968(&mf[0][0], m->rotY);
    func_00210790(&mf[0][0], m->rotX);
    func_00210318(&mf[0][0], m->pos.x, m->pos.y, m->pos.z);
    func_00210C3C(&mf[0][0]);
    func_0020FF80(&mfDraw[0][0], &mf[0][0]);
    if (D_800C2038 == 2) {
        func_00210478(&mfDraw[0][0], D_800ADDB8, 1.0f, 1.0f);
    }
    func_0020D274(&mfDraw[0][0], &m->mtx[D_800B6D1C]);
    func_0020F85C(&mf[0][0], &m->pos, &out);
    func_0020B7F8(m->object, &m->mtx[D_800B6D1C], D_800AB0D0, 0, -1);
    func_0020B624();
    D_800AB8CC = 0;
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xE3001801;
        g->w1 = 0;
    }
}
