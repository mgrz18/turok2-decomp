/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0024AA08.c, func_0024AA08, with Turok 2's layouts. */
#include "common.h"

/* Draw an actor's shadow. Without flag 0x10000000 it gets the blob shadow
 * (func_0021E03C). Otherwise its projected shadow model is built once, while
 * above the ground: the current view's light direction (signed bytes) and
 * strength (colour) are sampled at the actor (func_00228910), the alpha is
 * 128 + 56 per unit of light (faded by D_800AB8C0 during a fade), the
 * direction is kept at least 0.15 from horizontal and the model is projected
 * along it onto a plane just under the actor (0.9 of its height, up to 512),
 * grown by 0.003 per unit of height, turned like the actor and set on the
 * ground; then it is submitted (func_0021ED04). */

typedef struct {
    f32 x, y, z;
} Vec3;

typedef struct {
    u8 pad0[0x8];
    u8 colour[2];           /* 0x08 */
    u8 padA[0x6];
    s8 dir[3];              /* 0x10 */
    u8 pad13[0x5];
} Light;                    /* 0x18 */

typedef struct {
    u8 pad0[0x10];
    s32 blobType;           /* 0x10 */
} ShadowParams;

typedef struct {
    u8 pad0[0x4];
    Vec3 pos;               /* 0x004 */
    s32 model;              /* 0x010 */
    u8 pad14[0x2C];
    u8 rot[0x10];           /* 0x040 */
    u8 pad50[0x50];
    void *shadow;           /* 0x0A0 */
    u8 padA4[0x30];
    u32 flags;              /* 0x0D4 */
    u8 padD8[0x36];
    u8 shadowAlpha;         /* 0x10E */
    u8 pad10F;
    Light lights[2];        /* 0x110 */
} Actor;

extern s32 D_800AB8B0;      /* fade mode */
extern f32 D_800AB8C0;      /* fade level */
extern s32 D_800B6D1C;      /* current view */
extern u8 D_800F7148[];
extern u8 D_8010FF34[];

s32 func_00268160(void);
f32 func_0026872C(Actor *, s32);
void *func_00206950(void *, s32);
f32 func_00298470(f32);     /* sqrtf */
void func_00228910(void *, f32, f32, f32, Vec3 *, f32 *);
void func_0020F040(Vec3 *);
void func_00210004(void *, Vec3 *, f32);
void func_00210318(void *, f32, f32, f32);
void func_00210478(void *, f32, f32, f32);
void func_00211114(void *, void *);
void func_0020C618(void *, void *, void *);
void func_0021ED04(Actor *, ShadowParams *, void *, u8);
void func_0021E03C(Actor *, s32, s32);

void func_0021DC84(Actor *a, s32 unused, ShadowParams *s) {
    Vec3 dir;
    f32 r;
    u8 m[0x40];
    u8 q[0x40];
    f32 dy;
    f32 h;
    f32 ty;
    f32 t;
    f32 u2;
    s32 v;
    Light *light;
    register f32 py __asm__("$f1");

    if (a->model != 0 && func_00268160() != 0 && !(a->flags & 0x200000)) {
        if (a->flags & 0x10000000) {
            if (a->shadow == NULL) {
                h = func_0026872C(a, 0);
                py = a->pos.y;
                if (py < h) {
                    goto end;
                }
                t = py - h;
                dy = t;
                if (512.0f < t) {
                    dy = 512.0f;
                }
                a->shadow = func_00206950(D_800F7148, 1);
                if (a->shadow == NULL) {
                    goto end;
                }
                light = &a->lights[D_800B6D1C];
                dir.x = light->dir[0] * 0.007874016f;
                dir.y = light->dir[1] * 0.007874016f;
                dir.z = light->dir[2] * 0.007874016f;
                t = (f32) light->colour[0] * (f32) light->colour[0];
                u2 = (f32) light->colour[1] * (f32) light->colour[1];
                t = t + u2;
                r = func_00298470(t + u2) * -0.00090497744f;
                func_00228910(D_8010FF34, a->pos.x, a->pos.y, a->pos.z, &dir, &r);
                if (r < -1.0f) {
                    r = -1.0f;
                }
                v = (s32) (r * 56.0f + 128.0f);
                if (D_800AB8B0 == 1) {
                    v = (s32) ((f32) v * D_800AB8C0 * 0.003921569f);
                }
                a->shadowAlpha = v;
                dir.y = 1.0f;
                func_0020F040(&dir);
                if (dir.y < 0.0f) {
                    if (-dir.y < 0.15f) {
                        goto clampy;
                    }
                } else if (dir.y < 0.15f) {
clampy:
                    if (0.0f <= dir.y) {
                        dir.y = 0.15f;
                    } else {
                        dir.y = -0.15f;
                    }
                    func_0020F040(&dir);
                }
                ty = h + dy * 0.9f;
                func_00210004(m, &dir, ty);
                func_00210318(m, -a->pos.x, -ty, -a->pos.z);
                t = dy * 0.003f + 1.0f;
                func_00210478(m, t, t, t);
                func_00211114(a->rot, q);
                func_0020C618(a->shadow, m, q);
                func_00210318(a->shadow, a->pos.x, h, a->pos.z);
                if (a->shadow == NULL) {
                    goto end;
                }
            }
            func_0021ED04(a, s, a->shadow, a->shadowAlpha);
        } else {
            func_0021E03C(a, s->blobType, 0);
        }
    }
end:
    return;
}
