#include "common.h"
#include "structs/game_object.h"

/* out = a + b */
void CVector3__Add(CVector3 *arg0, CVector3 *arg1, CVector3 *arg2) {
    arg0->x = (f32) (arg1->x + arg2->x);
    arg0->y = (f32) (arg1->y + arg2->y);
    arg0->z = (f32) (arg1->z + arg2->z);
}
