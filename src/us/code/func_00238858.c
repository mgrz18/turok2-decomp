#include "common.h"
#include "m2c_macros.h"
#include "structs/vector.h"

#ifdef TARGET_PORT
extern void func_00237FAC(sptr, CVector3, s32);

/* impact event 7 (a door closing): the actor pointer at full width */
void func_00238858(sptr arg0, sptr arg1, s32 arg2, CVector3 arg3) {
#else
extern void func_00237FAC(s32, CVector3, s32);

void func_00238858(s32 arg0, s32 arg1, s32 arg2, CVector3 arg3) {
#endif
    func_00237FAC(arg0, arg3, 0);
}
