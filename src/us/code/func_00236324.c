#include "common.h"
#include "structs/vector.h"
#include "structs/boss.h"

/* The globals at D_800AB8B0 and D_800AB8C0 this sets. */
typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ CVector3 color;
} LightColor;

typedef struct {
    /* 0x0 */ f32 intensity;
    /* 0x4 */ f32 unk4;
} LightLevel;

extern LightColor D_800AB8B0;
extern LightLevel D_800AB8C0;

void func_00236324(ScriptAnimator *arg0) {
    CVector3 *temp_v0;
    CVector3 *temp_v0_2;

    if (arg0->alpha != 0.0f) {
        temp_v0 = &D_800AB8B0.color;
        D_800AB8B0.color.x = (f32) arg0->rgb[0];
        temp_v0->y = (f32) arg0->rgb[1];
        temp_v0->z = (f32) arg0->rgb[2];
    } else {
        temp_v0_2 = &D_800AB8B0.color;
        D_800AB8B0.color.x = 0.0f;
        temp_v0_2->y = 0.0f;
        temp_v0_2->z = 0.0f;
    }
    D_800AB8C0.intensity = (f32) arg0->alpha;
    D_800AB8C0.unk4 = (f32) arg0->value;
}
