#include "common.h"
#include "structs/game_object.h"

/* A vertical cylinder: its centre and its squared radius. */
typedef struct {
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ f32 unkC;
    /* 0x10 */ f32 radiusSq;
} Cylinder;

s32 func_0023CEC0(Cylinder *arg0, CVector3 *arg1) {
    f32 temp_f0;
    f32 temp_f1;
    s32 var_v0;

    temp_f1 = arg0->pos.x - arg1->x;
    temp_f0 = arg0->pos.z - arg1->z;
    var_v0 = 1;
    if (!(((temp_f1 * temp_f1) + (temp_f0 * temp_f0)) <= arg0->radiusSq)) {
        var_v0 = 0;
    }
    return var_v0;
}
