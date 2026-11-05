#include "common.h"
#include "structs/light.h"

/* A trail's colour contribution. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ f32 alpha;
    /* 0x18 */ u8 pad18[0x24 - 0x18];
    /* 0x24 */ f32 level;
    /* 0x28 */ u8 pad28[0x34 - 0x28];
    /* 0x34 */ f32 color[3];
} Arg0;

void func_00236390(Arg0 *arg0) {
    f32 *temp_v0;

    if (arg0->level != 0.0f) {
        temp_v0 = &D_800AB8B0.m_Color.x; /* [3] runs into D_800AB8C0[0] */
        D_800AB8B0.m_Color.x = (f32) (D_800AB8B0.m_Color.x + arg0->color[0]);
        temp_v0[1] = (f32) (temp_v0[1] + arg0->color[1]);
        temp_v0[2] = (f32) (temp_v0[2] + arg0->color[2]);
        temp_v0[3] = (f32) (temp_v0[3] + arg0->level);
    }
    D_800AB8C0[1] = (f32) (D_800AB8C0[1] + arg0->alpha);
}
