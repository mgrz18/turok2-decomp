#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ CVector3 min;
    /* 0x0C */ CVector3 max;
} Box;

/* The eight corners of the box: bottom face (y = min) then top face. */
void func_002406D8(Box *arg0, CVector3 *arg1) {
    arg1[0].x = (f32) arg0->min.x;
    arg1[0].y = (f32) arg0->min.y;
    arg1[0].z = (f32) arg0->min.z;
    arg1[1].x = (f32) arg0->max.x;
    arg1[1].y = (f32) arg0->min.y;
    arg1[1].z = (f32) arg0->min.z;
    arg1[2].x = (f32) arg0->max.x;
    arg1[2].y = (f32) arg0->min.y;
    arg1[2].z = (f32) arg0->max.z;
    arg1[3].x = (f32) arg0->min.x;
    arg1[3].y = (f32) arg0->min.y;
    arg1[3].z = (f32) arg0->max.z;
    arg1[4].x = (f32) arg0->min.x;
    arg1[4].y = (f32) arg0->max.y;
    arg1[4].z = (f32) arg0->min.z;
    arg1[5].x = (f32) arg0->max.x;
    arg1[5].y = (f32) arg0->max.y;
    arg1[5].z = (f32) arg0->min.z;
    arg1[6].x = (f32) arg0->max.x;
    arg1[6].y = (f32) arg0->max.y;
    arg1[6].z = (f32) arg0->max.z;
    arg1[7].x = (f32) arg0->min.x;
    arg1[7].y = (f32) arg0->max.y;
    arg1[7].z = (f32) arg0->max.z;
}
