#include "common.h"
#include "m2c_macros.h"
#include "structs/vector.h"

extern void func_00237FAC(sptr, CVector3, s32);

void func_00238824(sptr arg0, sptr arg1, s32 arg2, CVector3 arg3) { /* impact event (a door opening): the actors pointer-wide */
    func_00237FAC(arg0, arg3, 1);
}
