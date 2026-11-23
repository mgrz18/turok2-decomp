#include "common.h"
#include "m2c_macros.h"
#include "protos/iset.h"
#include "protos/cache.h"

extern M2C_UNK D_800A5808;
extern M2C_UNK D_800A582C;
extern M2C_UNK Model_DecompressCallback;

#include "structs/level_actor.h"
s32 *CStaticObject__LoadGeometry(void *arg0) {
    CStaticObject *o = arg0;
    s32 *temp_v0;

    if (o->m_GeometryAddr == 0) {
        temp_v0 = (s32 *) CCache__Request(0, o->m_ObjectAddr, 0x18, 4, 0, NULL, (char *) &D_800A5808, 0);
        if (temp_v0 != NULL) {
            o->m_GeometryAddr = CISet__GetROMAddressAndSize(*(void **) temp_v0, o->m_ObjectAddr, 1, &o->m_GeometrySize);
            CCache__Unlock(0, (void *) temp_v0);
        }
        if (o->m_GeometryAddr == 0) {
            return NULL;
        }
    }
    return (s32 *) CCache__Request(0, o->m_GeometryAddr, o->m_GeometrySize, 8, 0, &Model_DecompressCallback, (char *) &D_800A582C, 0);
}
