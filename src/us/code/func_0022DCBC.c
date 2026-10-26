#include "common.h"
#include "structs/game_object.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))
/* MIN as a statement expression: its value lives in its own temporary. */
#define MINF(a, b) ({ f32 _m = (a); if (!(_m <= (b))) _m = (b); _m; })

typedef union {
    struct { u32 w0, w1; } words;
} Gfx;

typedef struct { f32 x, y, z; } Vec3;
typedef struct { f32 x, y, z, w; } Quat;
typedef struct { f32 m[4][4]; } Mtx44;

/* The leading 0x34 bytes of a game object, as the collision test reads them. */
typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3 pos;
    /* 0x10 */ void *region;
    /* 0x14 */ u8 pad14[0x4];
    /* 0x18 */ Vec3 velocity;
    /* 0x24 */ u8 pad24[0x10];
} ObjectHead; /* size 0x34 */

typedef struct {
    u8 pad0[0x18];
    /* 0x18 */ f32 shadowScale;
} ShadowParams;

typedef struct {
    /* 0x00 */ u32 flags;
    u8 pad4[0x10];
    /* 0x14 */ s32 bigShadow;
    u8 pad18[0xC];
    /* 0x24 */ ShadowParams *params;
} ShadowDef;

typedef struct {
    u8 pad0[0x4];
    /* 0x004 */ Vec3 pos;
    /* 0x010 */ void *region;
    u8 pad14[0xE4];
    /* 0x0F8 */ ShadowDef *def;
    u8 padFC[0x20];
    /* 0x11C */ f32 fade;
    u8 pad120[0x8];
    /* 0x128 */ s16 fadeTime;
    u8 pad12A[0xA];
    /* 0x134 */ f32 radius;
} ShadowActor;

/* The result of the last collision test. */
typedef struct {
    /* 0x00 */ void *hit;
    u8 pad4[0x4];
    /* 0x08 */ Vec3 pos;
    u8 pad14[0xD0];
    /* 0xE4 */ Vec3 normal;
} CollisionResult;

struct CollisionInfo;
extern u8 D_800F7148[];
extern struct CollisionInfo D_800F1B40;
extern CollisionResult D_800F1CE0;
extern f32 D_800DD78C;
extern Gfx *D_800D8E24;

void *func_00206950(void *pool, s32 count);
void func_0026A9C0(Quat *out, void *region);
f32 func_00267648(ShadowActor *actor, s32 arg1);    /* ground height */
s32 func_00239F00(void *obj, Vec3 to, struct CollisionInfo *info);
void func_0020E810(Quat *out, Vec3 *dir);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
void func_00210F68(Quat *out, Quat *a, Quat *b);
void func_0020F510(Vec3 *v, f32 arg1);
void func_00211114(Quat *rot, Mtx44 *m);
void func_0021034C(Mtx44 *m, f32 x, f32 y, f32 z);  /* scale */
void func_00210318(Mtx44 *m, f32 x, f32 y, f32 z);  /* translate */
void func_0020D274(Mtx44 *m, void *dst);
void func_002086BC(void *mtx, s32 arg1, u32 alpha);

/* Draws the blob shadow under an actor: on the ground of its region, or
 * where a ray cast straight down from it lands, tilted to the surface. */
void func_0022DCBC(ShadowActor *actor) {
    Mtx44 mtx;
    Vec3 pos;
    Quat rot;
    Quat tilt;
    Quat spin;
    ObjectHead probe;
    void *gmtx;
    f32 ground;
    f32 height;
    f32 alpha;
    f32 a;
    f32 size;

    gmtx = func_00206950(D_800F7148, 1);
    if (gmtx == NULL) {
        return;
    }
    if (actor->region != NULL) {
        func_0026A9C0(&rot, actor->region);
        ground = func_00267648(actor, 0);
    } else {
        probe = *(ObjectHead *) actor;
        probe.pos.y += 10.24f;
        pos.x = probe.pos.x;
        pos.y = probe.pos.y - 102400.0f;
        pos.z = probe.pos.z;
        probe.velocity.x = 0;
        probe.velocity.y = 0;
        probe.velocity.z = 0;
        func_00239F00(&probe, pos, &D_800F1B40);
        if (D_800F1CE0.hit == NULL) {
            return;
        }
        func_0020E810(&tilt, &D_800F1CE0.normal);
        spin.x = D_800DD78C = func_002982F0(0.785398245f);
        spin.y = 0.0f;
        spin.z = 0.0f;
        spin.w = func_002974C0(0.785398245f);
        func_00210F68(&rot, &spin, &tilt);
        ground = D_800F1CE0.pos.y;
    }
    height = actor->pos.y - ground;
    a = 200.0f - height * 0.30517578f;
    alpha = MAX(MIN(a, 255.0f), 0.0f);
    size = actor->radius * 7.68f;
    if (actor->def->bigShadow) {
        size *= 8.0f;
    }
    size *= actor->def->params->shadowScale;
    size = MAX(MINF(size - height * 0.1f, size), 0.0f);
    if ((actor->def->flags & 1) && actor->fadeTime > 0.0f) {
        alpha *= (actor->fadeTime - actor->fade) / actor->fadeTime;
    }
    pos.x = actor->pos.x;
    pos.z = actor->pos.z;
    pos.y = ground;
    func_0020F510(&pos, 20000.0f);
    func_00211114(&rot, &mtx);
    func_0021034C(&mtx, size, 1.0f, size);
    func_00210318(&mtx, pos.x, pos.y, pos.z);
    func_0020D274(&mtx, gmtx);
    { Gfx *_g = (Gfx *) (D_800D8E24++); _g->words.w0 = 0xE7000000; _g->words.w1 = 0; }
    { Gfx *_g = (Gfx *) (D_800D8E24++); _g->words.w0 = 0xE3000A01; _g->words.w1 = 0x100000; }
    func_002086BC(gmtx, 1, (u8) alpha);
    { Gfx *_g = (Gfx *) (D_800D8E24++); _g->words.w0 = 0xE7000000; _g->words.w1 = 0; }
    { Gfx *_g = (Gfx *) (D_800D8E24++); _g->words.w0 = 0xE3000A01; _g->words.w1 = 0; }
}
