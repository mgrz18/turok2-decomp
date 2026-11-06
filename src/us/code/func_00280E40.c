#include "common.h"
#include "m2c_macros.h"
#include "structs/vector.h"

CVector3 CCamera__GetViewOrigin(u8 *arg0) {
    return *(CVector3 *)(arg0 + 0x24C);
}
