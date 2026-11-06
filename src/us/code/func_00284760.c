#include "common.h"
#include "globals/scene.h"

typedef struct {
    /* 0x0 */ s8 kind;
    /* 0x1 */ s8 value;
    /* 0x2 */ s16 id;
} Arg0;

s16 func_002261A8(s32 *);                       /* extern */

void CPathTrack__StartAtNearestPoint(Arg0 *arg0, s8 arg1) {
    arg0->kind = 2;
    arg0->value = arg1;
    arg0->id = func_002261A8(&gScene);
}
