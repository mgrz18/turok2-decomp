#include "common.h"
#include "m2c_macros.h"
#include "protos/impact_event.h"


f32 Blast_GetSourceScale(u8 *arg0) {
    if (*arg0 == 1) {
        return CGameObjectInstance__GetAverageScale((sptr) arg0);
    }
    return 1.0f;
}
