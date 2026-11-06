#include "common.h"

void CMtxF__PreRotateY(void *mtx, f32 angle);

void WeaponNode_Rotate_AF324(void *mtx, void *arg1) {
    f32 angle = *(f32 *) (*(s8 **) arg1 + 0x24C) * 1.5f;

    if (*(s32 *) ((s8 *) arg1 + 0x90) == 2) {
        CMtxF__PreRotateY(mtx, -angle);
    }
}
