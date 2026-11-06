#include "common.h"
#include "structs/game_object.h"

/* arg0 = arg1 x arg2 (cross product). */
void CVector3__Cross(CVector3 *arg0, CVector3 *arg1, CVector3 *arg2) {
    arg0->x = ((arg1->y * arg2->z) - (arg1->z * arg2->y));
    arg0->y = ((arg1->z * arg2->x) - (arg1->x * arg2->z));
    arg0->z = ((arg1->x * arg2->y) - (arg1->y * arg2->x));
}
