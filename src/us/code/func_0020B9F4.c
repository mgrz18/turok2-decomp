#include "common.h"
#include "structs/model.h"

s32 CCache__Request(s32, s32, s32, s32, s32, s32, s32, s32);
s32 CISet__GetROMAddressAndSize(s32, s32, s32, s32 *);
s32 *CISet__GetBlock(s32, s32);
void Render_FlushModelBatch(void);
void CCache__TouchAndLock(s32, s32);
void CCache__Unlock(s32, s32);
extern u8 D_800A53D8[];
extern u8 D_800A53EC[];
extern void Model_DecompressCallbackTerminated();
extern s32 D_800D8E10;
extern s32 D_800D8E14;
extern s32 D_800D8E28;
extern s32 D_800D8E34;
extern s32 D_800DD170;
extern s32 D_800DD178[];

void func_0020B9F4(s32 res, s32 arg1, s32 arg2, s32 index, s32 arg4) {
    s32 size;
    s32 file;
    s32 handle;
    CModelInstance *e;
    s32 *count;
    s32 n;

    file = CCache__Request(0, res, 0x18, 0, 0, 0, (s32) D_800A53D8, 0);
    if (file != 0) {
        handle = CCache__Request(0, CISet__GetROMAddressAndSize(*(s32 *) file, res, 1, &size), size, 0, 0,
                               (s32) Model_DecompressCallbackTerminated, (s32) D_800A53EC, 0);
        if (handle != 0) {
            count = CISet__GetBlock(*(s32 *) handle, 2);
            if (index >= 0 && index < *count) {
                if (D_800D8E28 != handle || D_800D8E14 != index || D_800D8E10 != arg4 || D_800D8E34 == 32) {
                    Render_FlushModelBatch();
                    if (D_800DD170 == 256) {
                        goto done;
                    }
                    CCache__TouchAndLock(0, handle);
                    D_800DD178[D_800DD170] = handle;
                    D_800DD170++;
                }
                n = D_800D8E34;
                D_800D8E28 = handle;
                D_800D8E14 = index;
                D_800D8E10 = arg4;
                e = &D_800DD578[n++];
                D_800D8E34 = n;
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
