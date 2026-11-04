#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ CVector3 *a;
    /* 0x8 */ CVector3 *b;
    /* 0xC */ CVector3 *c;
} Triangle;

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ f32 y;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ Triangle *tri;
} Unk00268B98;

/* Height of arg0 above the lowest vertex of its triangle. */
f32 func_00268B98(Unk00268B98 *arg0) {
    f32 temp_f1;
    f32 var_f0;
    f32 var_f1;
    Triangle *temp_a1;

    temp_a1 = arg0->tri;
    var_f0 = temp_a1->b->y;
    temp_f1 = temp_a1->a->y;
    if (!(var_f0 <= temp_f1)) {
        var_f0 = temp_f1;
    }
    var_f1 = temp_a1->c->y;
    if (!(var_f1 <= var_f0)) {
        var_f1 = var_f0;
    }
    return arg0->y - var_f1;
}
