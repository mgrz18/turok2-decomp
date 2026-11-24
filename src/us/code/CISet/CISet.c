/* WRONG CUT: splat's RNC_UnpackMethod2 (0x2010C0, 0x714 bytes) is not one C function.
 * 0x2010C0-0x2017A8 is hand-written asm (the decompressor next to the RNC code:
 * `addi`, every register saved, jalr to 0x201168 inside the block) and stays asm;
 * 0x2017A8-0x2017B0 is the ASCII padding "meSkyLay"; the only C here is the
 * object that starts at 0x2017B0 (size 0x24), written below as CISet__GetDataSize. */
#include "common.h"
#include "protos/iset.h"

/* Returns the word that follows the table at `data` (count at data[0]). */
s32 CISet__GetDataSize(s32 *data) {
    s32 *table;
    s32 value;

    CISet__GetSetAndSize(data, &table, &value);
    return value;
}

#include "common.h"

/* A resource block starts with a table of word offsets (entry 0 skipped): returns the address of section arg1. */
void *CISet__GetBlock(void *arg0, s32 arg1) {
    return (u8 *) arg0 + ((s32 *) ((u8 *) arg0 + (arg1 << 2)))[1];
}

#include "m2c_macros.h"

sptr CISet__GetBlockAndSize(s32 *t, s32 i, s32 *size) {
    *size = *(s32 *)((s8 *)t + ((i + 1) << 2) + 4) - *(s32 *)((s8 *)t + (i << 2) + 4);
    return (sptr)t + *(s32 *)((s8 *)t + (i << 2) + 4);
}

s32 CISet__GetBlockSize(s32 *table, s32 index) {
    s32 nextIndex = index + 1;
    s32 *next = &table[nextIndex];
    s32 *entry = &table[index];
    s32 size;
    s32 *output = &size;
    *output = next[1] - entry[1];
    return size;
}

#include "structs/table.h"

s32 CISet__GetROMAddress(sptr arg0, s32 arg1, s32 arg2) {
    return arg1 + ((OffsetTable *)arg0)->offsets[arg2];
}

/* Offset table: returns base + the i-th offset and stores the i-th entry's size. */
sptr CISet__GetROMAddressAndSize(s32 *t, sptr base, sptr i, s32 *size) {
    *size = *(s32 *)((s8 *)t + ((i + 1) << 2) + 4) - *(s32 *)((s8 *)t + (i << 2) + 4);
    return base + *(s32 *)((s8 *)t + (i << 2) + 4);
}

s32 CISet__GetOffsetAndSize(s32 *t, s32 i, s32 *size) {
    *size = *(s32 *)((s8 *)t + ((i + 1) << 2) + 4) - *(s32 *)((s8 *)t + (i << 2) + 4);
    return *(s32 *)((s8 *)t + (i << 2) + 4);
}

void CISet__GetSetAndSize(s32 *arg0, s32 **arg1, s32 *arg2) {
    *arg1 = arg0;
    *arg2 = *(s32 *)((s8 *)arg0 + (*arg0 << 2) + 4);
}

/* Bytes taken by a table: count * element size plus the 8-byte header. */
s32 CUSet__GetTotalSize(ArrayTable *arg0) {
    return (arg0->count * arg0->size) + 8;
}

/* The table's address and total size in bytes. */
void CUSet__GetSetAndSize(ArrayTable *arg0, void **arg1, s32 *arg2) {
    *arg1 = arg0;
    *arg2 = (arg0->count * arg0->size) + 8;
}

/* The table data and its size in bytes. */
void *CUSet__GetArrayAndSize(ArrayTable *arg0, s32 *arg1) {
    *arg1 = arg0->count * arg0->size;
    return arg0->data;
}

extern void *func_0029E460(void *dst, void *src, s32 size); /* memcpy */

/* Transpose src into dst; with copyBack, copy the result back over src. */
void CUSet__Transpose(ByteImage *src, ByteImage *dst, s32 copyBack) {
    u8 *in = src->data;
    u8 *out;
    s32 width, height;
    s32 x, y;

    func_0029E460(dst, src, 8);
    out = dst->data;
    height = src->height;
    width = src->width;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            *out++ = in[x * height + y];
        }
    }
    if (copyBack) {
        func_0029E460(src, dst, height * width + 8);
    }
}

#include "ultra_types.h"
#include "protos/ultra.h"

s32 __osDisableInt();                                /* extern */
s32 __osRestoreInt(s32);                         /* extern */

void CMutex__Wait(CMutex *arg0) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;

    if (arg0->count != 0) {
        temp_a0 = __osDisableInt();
        temp_v1 = arg0->count + 1;
        arg0->count = temp_v1;
        if (temp_v1 != 1) {
            __osRestoreInt(temp_a0);
            osRecvMesg(arg0, 0, 1);
        } else {
            __osRestoreInt(temp_a0);
        }
        temp_v0 = __osDisableInt();
        temp_v1_2 = arg0->count - 1;
        arg0->count = temp_v1_2;
        if (temp_v1_2 != 0) {
            __osRestoreInt(temp_v0);
            osSendMesg(arg0, 0, 1);
            return;
        }
        __osRestoreInt(temp_v0);
    }
}

void CMutex__Construct(CMutex *arg0) {
    arg0->count = 0;
    osCreateMesgQueue(&arg0->queue, &arg0->msg, 1);
}
