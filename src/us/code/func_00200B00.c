/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802846C8_de_4.c, func_80285180_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "protos/cache.h"

extern void *func_0029E460(void *destination, const void *source, int count);
s32 RNC_UnpackMethod1();
s32 RNC_UnpackMethod2();
#ifdef TARGET_PORT
#include "structs/cache.h"
CMemEntry *CCache__AllocDecompressBlock(CCacheEntry *request, u32 size);

/* The host never sees an RNC stream here: the archive stores every RNC unit unpacked
 * (tools/assets/mkarchive.py refuses units with RNC children) and the loader already put it
 * in host byte order, so only the N64's plain-data branch is left: with arg1 == 0 the unit
 * is copied into a block of its own (CCache__DeallocAndReplaceCompressedData swaps it in),
 * with arg1 != 0 it stays where it was loaded (the N64 decompresses an RNC unit into a block
 * at the heap top there; plain data is left in place, as here). arg0 is the request
 * (CCacheEntry, whose first field is the CMemEntry). */
s32 ProPackDecompress(void **arg0, s32 arg1) {
    CCacheEntry *request = (CCacheEntry *)arg0;
    CMemEntry *src = request->memEntry;
    CMemEntry *dst;

    if (arg1 != 0) {
        return 1;
    }
    dst = CCache__AllocDecompressBlock(request, (u32)src->size);
    if (dst == NULL) {
        return 0;
    }
    func_0029E460(dst->data, src->data, src->size);
    CCache__DeallocAndReplaceCompressedData(0, &request->memEntry, dst);
    return 1;
}
#else
s32 ProPackDecompress(void **arg0, s32 arg1) {
    s32 var_s0;
    u8 temp_v1;
    void **var_s1;
    ObjRecAt4V2 *temp_a2;
    ObjRecAt12 *temp_s0;
    ObjRecAt4V2 *temp_v0;
    temp_a2 = *arg0;
    temp_s0 = temp_a2->unk0;
    if ((temp_s0->unk0 & ~0xFF) == 0x524E4300) {
        if (arg1 != 0) {
            var_s1 = CCache__AllocateMemoryForDecompressionTop(0, (sptr) arg0, temp_s0->unk4);
        } else {
            var_s1 = CCache__AllocateMemoryForDecompression(0, arg0, temp_s0->unk4);
        }
        if (var_s1 != ((void *)0)) {
            temp_v1 = (((Access_u8_3 *)(temp_s0))->field);
            switch (temp_v1) {
            case 1:
                RNC_UnpackMethod1(&temp_s0->unk12, *var_s1, temp_s0->unk8, temp_s0->unk4);
                break;
            case 2:
                RNC_UnpackMethod2(temp_s0, *var_s1, temp_s0->unk8, temp_s0->unk4);
                break;
            default:
                break;
            }
            var_s0 = 1;
        } else {
            var_s0 = 0;
        }
    } else if (arg1 == 0) {
        var_s1 = CCache__AllocateMemoryForDecompression(0, arg0, temp_a2->unk4);
        if (var_s1 != ((void *)0)) {
            temp_v0 = *arg0;
            var_s0 = 1;
            func_0029E460(*var_s1, temp_v0->unk0, temp_v0->unk4);
        } else {
            var_s0 = 0;
        }
    } else {
        var_s0 = 1;
        goto done;
    }
    CCache__DeallocAndReplaceCompressedData(0, arg0, var_s1);
done:
    return var_s0;
}
#endif
