#include "common.h"
#include "m2c_macros.h"
#include "protos/iset.h"

s32 *CCache__Request(M2C_UNK, s32, s32, M2C_UNK, s32, M2C_UNK *, M2C_UNK *, s32); /* extern */
M2C_UNK CCache__Unlock(M2C_UNK, s32 *);              /* extern */
extern M2C_UNK D_800A5808;
extern M2C_UNK D_800A582C;
extern M2C_UNK Model_DecompressCallback;

s32 *CStaticObject__LoadGeometry(void *arg0) {
    s32 *temp_v0;

    if (M2C_FIELD(arg0, s32 *, 0x1C) == 0) {
        temp_v0 = CCache__Request(0, M2C_FIELD(arg0, s32 *, 0x18), 0x18, 4, 0, NULL, &D_800A5808, 0);
        if (temp_v0 != NULL) {
            M2C_FIELD(arg0, s32 *, 0x1C) = CISet__GetROMAddressAndSize(*temp_v0, M2C_FIELD(arg0, s32 *, 0x18), 1, arg0 + 0x20);
            CCache__Unlock(0, temp_v0);
        }
        if (M2C_FIELD(arg0, s32 *, 0x1C) == 0) {
            return NULL;
        }
        goto block_5;
    }
block_5:
    return CCache__Request(0, M2C_FIELD(arg0, s32 *, 0x1C), M2C_FIELD(arg0, s32 *, 0x20), 8, 0, &Model_DecompressCallback, &D_800A582C, 0);
}
