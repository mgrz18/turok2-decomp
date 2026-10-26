#include "common.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Mtx44 {
    f32 m[4][4];
} Mtx44;

typedef struct ObjDef {
    u16 pad0;
    u16 flags;                  /* 0x02 */
} ObjDef;

typedef struct ShadowActor {
    u8 pad0[0x4];
    Vec3 pos;                   /* 0x04 */
    ObjDef *def;                /* 0x10 */
    u8 pad14[0x40 - 0x14];
    u8 rot[0x54 - 0x40];        /* 0x40 */
    f32 unk54;                  /* 0x54 */
    u8 pad58[0x10E - 0x58];
    u8 shadowAlpha;             /* 0x10E */
} ShadowActor;

extern u8 D_800F7148[];

s32 func_0026AFC0(ObjDef *def);
void *func_00206950(void *pool, s32 count);
f32 func_00267648(ShadowActor *actor, s32 arg1);    /* ground height */
f32 func_00268304(ShadowActor *actor);              /* radius */
void func_0020F510(Vec3 *v, f32 arg1);
void func_00211114(void *rot, Mtx44 *m);
void func_0021034C(Mtx44 *m, f32 x, f32 y, f32 z);  /* scale */
void func_00210318(Mtx44 *m, f32 x, f32 y, f32 z);  /* translate */
void func_0020D274(Mtx44 *m, void *dst);
void func_002086BC(void *mtx, s32 arg1, s32 alpha);

/* Draws the blob shadow under an actor. */
void func_0021E03C(ShadowActor *actor, Vec3 *offset, s32 arg2) {
    Mtx44 mtx;
    Vec3 pos;
    void *gmtx;
    s32 visible;
    f32 ground;
    f32 height;
    f32 size;
    f32 lift;
    f32 a;
    u8 alpha;

    visible = !func_0026AFC0(actor->def);
    if (actor->def != NULL && (actor->def->flags & 0x4000)) {
        visible = 0;
    }
    if (!visible) {
        return;
    }
    gmtx = func_00206950(D_800F7148, 1);
    if (gmtx == NULL) {
        return;
    }
    ground = func_00267648(actor, 0);
    height = actor->pos.y + actor->unk54 - ground;
    size = MAX(MIN(func_00268304(actor), func_00268304(actor) - height * 0.1f), 0.0f);
    if (arg2) {
        lift = 15.36f;
        size *= 0.5f;
    } else {
        lift = 0.0f;
        size *= 0.7f;
    }
    pos.x = actor->pos.x + offset->x;
    pos.z = actor->pos.z + offset->z;
    pos.y = ground + lift + offset->y;
    func_0020F510(&pos, 20000.0f);
    func_00211114(actor->rot, &mtx);
    func_0021034C(&mtx, size, 1.0f, size);
    func_00210318(&mtx, pos.x, pos.y, pos.z);
    func_0020D274(&mtx, gmtx);
    a = 200.0f - height * 1.2207031f;
    alpha = a > 255.0f ? 255 : a < 0.0f ? 0 : a > 255.0f ? 255 : (u8)a;
    actor->shadowAlpha = actor->shadowAlpha + (alpha - actor->shadowAlpha) * 0.5f;
    func_002086BC(gmtx, arg2 == 0, actor->shadowAlpha);
}
