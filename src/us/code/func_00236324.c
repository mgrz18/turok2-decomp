#include "common.h"
#include "structs/vector.h"
#include "structs/light.h"

/* The light source read here. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ f32 unk14;
    /* 0x18 */ u8 pad18[0x24 - 0x18];
    /* 0x24 */ f32 intensity;
    /* 0x28 */ u8 pad28[0x34 - 0x28];
    /* 0x34 */ CVector3 color;
} Light;

void func_00236324(Light *arg0) {
    CVector3 *temp_v0;
    CVector3 *temp_v0_2;

    if (arg0->intensity != 0.0f) {
        temp_v0 = &D_800AB8B0.m_Color;
        D_800AB8B0.m_Color.x = (f32) arg0->color.x;
        temp_v0->y = (f32) arg0->color.y;
        temp_v0->z = (f32) arg0->color.z;
    } else {
        temp_v0_2 = &D_800AB8B0.m_Color;
        D_800AB8B0.m_Color.x = 0.0f;
        temp_v0_2->y = 0.0f;
        temp_v0_2->z = 0.0f;
    }
    D_800AB8C0[0] = (f32) arg0->intensity;
    D_800AB8C0[1] = (f32) arg0->unk14;
}
