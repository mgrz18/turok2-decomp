/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00252450.c, func_00252450.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"
#include "globals/cache.h"
#include "globals/sysrc.h"
#include "protos/list.h"

u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern B12Mgr D_800D7BE0;
void CCache__ReleaseDisplayList(s32 a, u32 b);
s32 CDecompressor__DecompressAsync(CDecompressor *port, B12Msg *n, void *q);
void CCache__DetachCacheEntry(s32 a, B12Node *n);
void CHeap__Free(CHeap *heap, s32 key);
void CCache__FreeMemEntry(s32 a, B12Node *n);
void CCache__FreeCacheEntry(s32 a, B12Msg *n);
void CCache__CompleteRequest(s32 a, B12Msg *n, s32 b);
void CCache__Main(void) {
    B12Msg *msg;
    B12Node *p;
    u32 mask;
    u32 m2;
    while (1) {
        osRecvMesg(&D_800D7BE0.q870, (OSMesg *)&msg, 1);
        if ((u32)msg < 2) {
            mask = __osDisableInt();
            if (++gCache_mutex_count != 1) {
                __osRestoreInt(mask);
                osRecvMesg(&D_800D7BE0.lock.queue, 0, 1);
            } else {
                __osRestoreInt(mask);
            }
            CCache__ReleaseDisplayList(0, (u32)msg);
            m2 = __osDisableInt();
            if (--gCache_mutex_count != 0) {
                __osRestoreInt(m2);
                osSendMesg(&gCache_mutex, 0, 1);
            } else {
                __osRestoreInt(m2);
            }
        } else {
            msg->flags &= ~2;
            if (msg->flags & 8) {
                msg->flags &= ~8;
                if (CDecompressor__DecompressAsync(&gDecompressor, msg, &D_800D7BE0.q870) != 0) {
                    continue;
                }
                mask = __osDisableInt();
                if (++gCache_mutex_count != 1) {
                    __osRestoreInt(mask);
                    osRecvMesg(&D_800D7BE0.lock.queue, 0, 1);
                } else {
                    __osRestoreInt(mask);
                }
                CList__Remove(&D_800D7BE0.lists3[2], msg);
                CList__AddHead(&D_800D7BE0.lists3[1], msg);
                p = msg->node;
                p->flags &= ~2;
                while (p->refs != 0) {
                    p->refs--;
                    if (p->refs == 0) {
                        p->flags &= ~0x100;
                    }
                }
                if (!(p->flags & 0x702)) {
                    CCache__DetachCacheEntry(0, p);
                    CHeap__Free(&gHeap, p->key);
                    CCache__FreeMemEntry(0, p);
                }
                CCache__FreeCacheEntry(0, msg);
                m2 = __osDisableInt();
                if (--gCache_mutex_count != 0) {
                    __osRestoreInt(m2);
                    osSendMesg(&gCache_mutex, 0, 1);
                } else {
                    __osRestoreInt(m2);
                }
            } else {
                msg->flags &= ~4;
                mask = __osDisableInt();
                if (++gCache_mutex_count != 1) {
                    __osRestoreInt(mask);
                    osRecvMesg(&D_800D7BE0.lock.queue, 0, 1);
                } else {
                    __osRestoreInt(mask);
                }
                CCache__CompleteRequest(0, msg, 1);
                m2 = __osDisableInt();
                if (--gCache_mutex_count != 0) {
                    __osRestoreInt(m2);
                    osSendMesg(&gCache_mutex, 0, 1);
                } else {
                    __osRestoreInt(m2);
                }
            }
        }
    }
}
