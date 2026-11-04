#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ f32 minX;
    /* 0x04 */ f32 minZ;
    /* 0x08 */ f32 maxX;
    /* 0x0C */ f32 maxZ;
} Bounds2D;

s32 func_0026CDAC(s32, s32, s32);

/* Starts the x/z bounds at the first point, then grows them over the rest. */
void func_0026CD70(Bounds2D *arg0, s32 arg1, CVector3 *arg2) {
    f32 temp_f0;
    f32 temp_f0_2;

    if (arg1 != 0) {
        temp_f0 = arg2->x;
        arg0->maxX = temp_f0;
        arg0->minX = temp_f0;
        temp_f0_2 = arg2->z;
        arg0->maxZ = temp_f0_2;
        arg0->minZ = temp_f0_2;
        func_0026CDAC((s32) arg0, arg1 - 1, (s32) (arg2 + 1));
    }
}
