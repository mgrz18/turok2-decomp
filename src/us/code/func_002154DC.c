/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WORLD_2/8024E914_6.CPP, func_8024EF28_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"

#define MIN_LT(a, b) ((a) < (b) ? (a) : (b))

typedef struct {
    /* 0x00 */ u8 pad00[4];
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 z;
    /* 0x10 */ s32 region;
    /* 0x14 */ u8 pad14[0xA4];
    /* 0xB8 */ f32 shadowMatrices[4][16];
} ShadowActor;

f32 CGameRegion__GetHeight(s32, f32, f32);
f32 GameObject_GetWallRadius(ShadowActor *);
void GameObject_GetGroundRotation(f32 *, ShadowActor *);
void CVector3__ClampLength(CVector3 *, f32);
void CQuaternion__ToMatrix(f32 *, f32 *);
void CMtxF__ScaleAxes(f32 *, f32, f32, f32);
void CMtxF__Translate(f32 *, f32, f32, f32);
void CMtxF__ToMtxAffine(f32 *, f32 *);
void Render_DrawShadowQuad(f32 *, s32, s32);

extern s32 gEvenOdd;

/* Places the actor's ground shadow: scaled by its radius, shrinking with its height above the ground. */
void Actor_DrawGroundShadow(ShadowActor *actor) {
    f32 matrix[16];
    CVector3 position;
    f32 rotation[4];
    f32 ground;
    f32 height;
    f32 size;

    ground = CGameRegion__GetHeight(actor->region, actor->x, actor->z);
    height = actor->y - ground;
    if (!(MIN_LT(GameObject_GetWallRadius(actor) * 0.5f, GameObject_GetWallRadius(actor) * 0.5f - height * 0.1f) < 0.0f)) {
        size = MIN_LT(GameObject_GetWallRadius(actor) * 0.5f, GameObject_GetWallRadius(actor) * 0.5f - height * 0.1f);
    } else {
        size = 0.0f;
    }
    GameObject_GetGroundRotation(rotation, actor);
    position.x = actor->x;
    position.z = actor->z;
    position.y = ground;
    CVector3__ClampLength(&position, 20000.0f);
    CQuaternion__ToMatrix(rotation, matrix);
    CMtxF__ScaleAxes(matrix, size, 1.0f, size);
    CMtxF__Translate(matrix, position.x, position.y, position.z);
    CMtxF__ToMtxAffine(matrix, actor->shadowMatrices[gEvenOdd]);
    Render_DrawShadowQuad(actor->shadowMatrices[gEvenOdd], 1, 0x96);
}
