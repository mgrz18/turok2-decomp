#include "common.h"

extern f32 D_800C1064;
extern f32 D_8011ACC0;

/* Plain pointer arithmetic, not struct fields: GCC marks struct accesses
 * MEM_IN_STRUCT and would then hoist them above the global store. */
#define OBJ(arg)        (*(u8 **) ((u8 *) (arg) + 0xC))
#define OBJ_FLAGS(obj)  (*(s32 *) ((u8 *) (obj) + 0x30))

void func_0041A420(void *arg0) {
    u8 *obj;

    D_8011ACC0 = D_800C1064;
    obj = OBJ(arg0);
    OBJ_FLAGS(obj) &= ~0x04000000;
}
