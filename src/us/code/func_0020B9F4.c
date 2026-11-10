#include "common.h"
#include "structs/model.h"
#include "globals/batch.h"
#include "protos/iset.h"
#include "protos/cache.h"

void Render_FlushModelBatch(void);
extern u8 D_800A53D8[];
extern u8 D_800A53EC[];
extern void Model_DecompressCallbackTerminated();

void func_0020B9F4(sptr res, sptr arg1, sptr arg2, s32 index, s32 arg4) {
    s32 size;
    sptr file;
    sptr handle;
    CModelInstance *e;
    s32 *count;
    s32 n;

    file = CCache__Request(0, res, 0x18, 0, 0, 0, D_800A53D8, 0);
    if (file != 0) {
#ifdef TARGET_PORT
        /* the cache entries hold host pointers: read them pointer-wide (--high-memory) */
        handle = CCache__Request(0, CISet__GetROMAddressAndSize(*(s32 **) file, res, 1, &size), size, 0, 0,
                               Model_DecompressCallbackTerminated, D_800A53EC, 0);
        if (handle != 0) {
            count = CISet__GetBlock(*(void **) handle, 2);
#else
        handle = CCache__Request(0, CISet__GetROMAddressAndSize(*(s32 *) file, res, 1, &size), size, 0, 0,
                               Model_DecompressCallbackTerminated, D_800A53EC, 0);
        if (handle != 0) {
            count = CISet__GetBlock(*(s32 *) handle, 2);
#endif
            if (index >= 0 && index < *count) {
                if (sBatchResource != (void **) handle || sBatchLod != index || sBatchPass != arg4 || sBatchInstanceCount == 32) {
                    Render_FlushModelBatch();
                    if (sBatchResourceCount == 256) {
                        goto done;
                    }
                    CCache__TouchAndLock(0, handle);
                    sBatchResources[sBatchResourceCount] = (void **) handle;
                    sBatchResourceCount++;
                }
                n = sBatchInstanceCount;
                sBatchResource = (void **) handle;
                sBatchLod = index;
                sBatchPass = arg4;
                e = &sBatchInstances[n++];
                sBatchInstanceCount = n;
                e->m_pMatrix = (void *)arg1;
                e->m_Segment = 0;
                e->m_pLights = (void *)arg2;
                e->m_pTextures = 0;
            }
        done:
            CCache__Unlock(0, handle);
        }
        CCache__Unlock(0, file);
    }
}
