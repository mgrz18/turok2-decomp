/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00250E10.c, func_00250E10.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/list.h"
u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);
void osStartThread(OSThread *thread);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
void *memset(void *s, int c, u32 n);
void osCreateThread(OSThread *thread, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri);
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msgBuf, s32 count);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
typedef struct B12Node B12Node;
struct B12Node {
               u32 key;
               s32 unk4;
               s32 refs;
               u32 flags;
               u32 unk10;
               B12Node *link;
               B12Node *link18;
               u8 pad1C[4];
               B12Node *prev;
               B12Node *next;
};
typedef struct B12Bucket B12Bucket;
struct B12Bucket {
              u32 unk0;
              u32 unk4;
              s32 id;
              B12Bucket *next;
};
typedef struct B12Mgr {
                u8 pad0[0x5F0];
                B12Node * nodes;
                B12Node ** stk;
                B12Node * cur;
                u8 * pool2;
                CList lists[2];
                s32 tbl628[2];
                s32 tbl630[2];
                s32 f638;
                u8 pad63C[0x4];
                OSThread thread;
                u8 pad7F0[0x80];
                OSMesgQueue q870;
                OSMesg buf870[0x40];
                u8 stack[0x800];
                 CList lists3[3];
                 s32 tbl11C4[2];
                 s32 cnt;
                 OSMesgQueue q11D0;
                 OSMesg buf11D0;
                 s32 lockCnt;
                 OSMesgQueue q11F0;
                 OSMesg buf11F0;
                 s32 f120C;
                 u32 stamp;
                 u8 pad1214[0xC];
                 u32 hashMask;
                 B12Bucket * buckets;
} B12Mgr;
extern B12Mgr D_800D7BE0;
extern s32 gCache_mutex_count;
extern u32 gCacheEntryCount;
extern s32 gThreadPriorityCache;
extern u8 gHeap[];
void CMutex__Construct(void *lock);
u32 GetMemSize(void);
u8 *CHeap__Alloc(void *heap, u32 size);
void CList__Construct(CList *l, s32 prevOff, s32 nextOff);
void CList__AddTail(CList *l, void *n);
void CHashTable__Construct(u32 n);
void CCache__Main(void *arg);
void CCache__Construct(s32 unused, s32 id) {
    u32 mask;
    u32 m2;
    u32 i;
    B12Node *n;
    B12Node *n1;
    u32 i1;
    s32 *t;
    CMutex__Construct(&D_800D7BE0.q11D0);
    mask = __osDisableInt();
    if (++gCache_mutex_count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&D_800D7BE0.q11D0, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
    gCacheEntryCount = 0x400;
    if (GetMemSize() > 0x400000) {
        gCacheEntryCount *= 2;
    }
    D_800D7BE0.nodes = (B12Node *)CHeap__Alloc(gHeap, gCacheEntryCount * 0x28);
    D_800D7BE0.stk = (B12Node **)CHeap__Alloc(gHeap, gCacheEntryCount * 4);
    D_800D7BE0.pool2 = CHeap__Alloc(gHeap, gCacheEntryCount * 0x2C);
    osCreateMesgQueue(&D_800D7BE0.q11F0, &D_800D7BE0.buf11F0, 1);
    D_800D7BE0.tbl630[1] = 0;
    D_800D7BE0.tbl630[0] = 0;
    CList__Construct(&D_800D7BE0.lists[0], 0x18, 0x1C);
    CList__Construct(&D_800D7BE0.lists[1], 0x20, 0x24);
    D_800D7BE0.cnt = 0;
    for (i = 0; i < gCacheEntryCount; i++) {
        n1 = D_800D7BE0.nodes;
        n1 = (B12Node *)((u8 *)n1 + i * 0x28);
        n1->flags = 0;
        D_800D7BE0.stk[i] = n1;
        D_800D7BE0.cnt++;
    }
    t = D_800D7BE0.tbl628;
    t[1] = 1;
    t[0] = 1;
    D_800D7BE0.f638 = -1;
    D_800D7BE0.tbl11C4[0] = 0;
    D_800D7BE0.tbl11C4[1] = 0;
    D_800D7BE0.cur = 0;
    D_800D7BE0.f120C = 0; /* 1 in Rage Wars */
    D_800D7BE0.stamp = 0;
    CList__Construct(&D_800D7BE0.lists3[0], 0x28, 0x24);
    CList__Construct(&D_800D7BE0.lists3[1], 0x28, 0x24);
    CList__Construct(&D_800D7BE0.lists3[2], 0x28, 0x24);
    for (i = 0; i < gCacheEntryCount; i++) {
        n = (B12Node *)D_800D7BE0.pool2;
        n = (B12Node *)((u8 *)n + i * 0x2C);
        n->unk10 = 0;
        CList__AddTail(&D_800D7BE0.lists3[0], n);
    }
    CHashTable__Construct(gCacheEntryCount);
    osCreateMesgQueue(&D_800D7BE0.q870, D_800D7BE0.buf870, 0x40);
    m2 = __osDisableInt();
    if (--gCache_mutex_count != 0) {
        __osRestoreInt(m2);
        osSendMesg(&D_800D7BE0.q11D0, 0, 1);
    } else {
        __osRestoreInt(m2);
    }
    memset(D_800D7BE0.stack, (u8)id, 0x800);
    osCreateThread(&D_800D7BE0.thread, id, CCache__Main, 0, D_800D7BE0.stack + 0x800, gThreadPriorityCache);
    osStartThread(&D_800D7BE0.thread);
}
