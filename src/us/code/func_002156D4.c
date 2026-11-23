#include "common.h"

#include "structs/vector.h"

/* Host body (docs/port/ABI.md, abicheck struct_scalar): a struct-returning wrapper. On MIPS a0 is
 * the hidden result pointer (returned in v0) and a1 the object, both passed straight through
 * to GameObject_GetGroundRotation; the caller (Actor_SpawnParticle) declares it CQuaternion f(obj). */
extern CQuaternion GameObject_GetGroundRotation(void *obj);
CQuaternion Actor_GetGroundRotation(void *obj) {
    return GameObject_GetGroundRotation(obj);
}
