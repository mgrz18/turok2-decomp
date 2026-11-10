#include "common.h"

#ifdef TARGET_PORT
#include "structs/vector.h"

/* Host body (docs/port/ABI.md, abicheck struct_scalar): a struct-returning wrapper. On MIPS a0 is
 * the hidden result pointer (returned in v0) and a1 the object, both passed straight through
 * to GameObject_GetGroundRotation; the caller (func_00246690) declares it CQuaternion f(obj). */
extern CQuaternion GameObject_GetGroundRotation(void *obj);
CQuaternion func_002156D4(void *obj) {
    return GameObject_GetGroundRotation(obj);
}
#else
s32 GameObject_GetGroundRotation();                            /* extern */

s32 func_002156D4(sptr arg0) {
    GameObject_GetGroundRotation();
    return arg0;
}
#endif
