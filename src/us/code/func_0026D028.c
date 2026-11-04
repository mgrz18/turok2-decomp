#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ CVector3 *a;
    /* 0x8 */ CVector3 *b;
    /* 0xC */ CVector3 *c;
} Triangle;

/* The highest y of the triangle's three vertices. */
f32 func_0026D028(Triangle *arg0) {
    f32 temp_f0;
    f32 var_f0;
    f32 var_f1;

    var_f1 = arg0->b->y;
    temp_f0 = arg0->a->y;
    if (!(temp_f0 <= var_f1)) {
        var_f1 = temp_f0;
    }
    var_f0 = arg0->c->y;
    if (!(var_f1 <= var_f0)) {
        var_f0 = var_f1;
    }
    return var_f0;
}
