/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WORLD_2/8024E914_6.CPP, func_8024EF28_de, with Turok 2's layouts. */
#include "common.h"

#define MIN_LT(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
    f32 x, y, z;
} Vec3;

typedef struct {
    /* 0x00 */ u8 pad00[4];
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 z;
    /* 0x10 */ s32 region;
    /* 0x14 */ u8 pad14[0xA4];
    /* 0xB8 */ f32 shadowMatrices[4][16];
} ShadowActor;

f32 func_0026BE60(s32, f32, f32);
f32 func_00268304(ShadowActor *);
void func_00267944(f32 *, ShadowActor *);
void func_0020F510(Vec3 *, f32);
void func_00211114(f32 *, f32 *);
void func_0021034C(f32 *, f32, f32, f32);
void func_00210318(f32 *, f32, f32, f32);
void func_0020D274(f32 *, f32 *);
void func_002086BC(f32 *, s32, s32);

extern s32 D_800B6D1C;

/* Places the actor's ground shadow: scaled by its radius, shrinking with its height above the ground. */
void func_002154DC(ShadowActor *actor) {
    f32 matrix[16];
    Vec3 position;
    f32 rotation[4];
    f32 ground;
    f32 height;
    f32 size;

    ground = func_0026BE60(actor->region, actor->x, actor->z);
    height = actor->y - ground;
    if (!(MIN_LT(func_00268304(actor) * 0.5f, func_00268304(actor) * 0.5f - height * 0.1f) < 0.0f)) {
        size = MIN_LT(func_00268304(actor) * 0.5f, func_00268304(actor) * 0.5f - height * 0.1f);
    } else {
        size = 0.0f;
    }
    func_00267944(rotation, actor);
    position.x = actor->x;
    position.z = actor->z;
    position.y = ground;
    func_0020F510(&position, 20000.0f);
    func_00211114(rotation, matrix);
    func_0021034C(matrix, size, 1.0f, size);
    func_00210318(matrix, position.x, position.y, position.z);
    func_0020D274(matrix, actor->shadowMatrices[D_800B6D1C]);
    func_002086BC(actor->shadowMatrices[D_800B6D1C], 1, 0x96);
}
