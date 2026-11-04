/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCENE/802508E0_4.CPP, func_8025193C_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/list.h"
#include "structs/cache.h"

typedef struct Manager {
    char queue[0x92C];
    CList pending;  /* 0x92C */
    CList active;   /* 0x940 */
    char padActive[0x20 - sizeof(CList)];
    char lock[0x18];        /* 0x960, OSMesgQueue */
    char unk978[0x24];
    s32 mask;               /* 0x99C */
} Manager;

typedef struct ResourceHashEntry {
    u32 key;
    CCacheEntry *value;
    s32 unk08;
    struct ResourceHashEntry *next;
} ResourceHashEntry;

extern s32 D_800D8DCC;
extern Manager D_800D8450;
extern s32 D_800D81E0;
extern u32 D_800D81C0;
extern ResourceHashEntry *D_800D81C4;
extern s32 D_800D1CD8;
extern s32 D_800D8DF0;
extern s32 D_800D6D68;

extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32);
extern s32 osRecvMesg(void *mq, void **msg, s32 flag);
extern s32 osSendMesg(void *mq, void *msg, s32 flag);
extern void func_00200818(void *, CMemEntry *);
extern CCacheEntry *func_002066F8(s32, u32);
extern CMemEntry *func_002031E8(s32, u32);
extern void func_002062F8(s32, CMemEntry *);
extern void func_00206498(s32, CMemEntry *);
extern void *func_002063B4(s32, u32, u32, u32);
extern void func_00201B50(s32 *, CCacheEntry *);
extern CCacheEntry *func_002044A8(s32, CCacheEntry *, s32);
extern void func_00201DBC(s32 *, u32, u32, void *);
extern void func_00201E94(s32 *, u32, u32, void *, void *, CCacheEntry *, s32);
extern void func_002066B0(s32, CCacheEntry *);
extern void CList__Remove(void *list, void *node);
extern s32 CList__AddTail(void *list, void *node);

static __inline__ void acquire(void) {
    u32 token = __osDisableInt();
    s32 count = D_800D8DCC + 1;
    D_800D8DCC = count;
    if (count != 1) {
        __osRestoreInt(token);
        osRecvMesg(D_800D8450.lock, 0, 1);
    } else {
        __osRestoreInt(token);
    }
}

static __inline__ void release(void) {
    u32 token = __osDisableInt();
    s32 count = D_800D8DCC - 1;
    D_800D8DCC = count;
    if (count != 0) {
        __osRestoreInt(token);
        osSendMesg(D_800D8450.lock, 0, 1);
    } else {
        __osRestoreInt(token);
    }
}

static __inline__ void find(u32 key, CCacheEntry **out) {
    ResourceHashEntry *node = D_800D81C4 + (((key << 5) ^ (key >> 1) ^ (key >> 9) ^ (key >> 17)) & D_800D81C0);
    if (node->key != key) goto not_initial;
    *out = node->value;
    goto done;
loop_found:
    *out = node->value;
    goto done;
not_initial:
    *out = 0;
    if (node == 0) goto done;
loop:
    if (node->key == key) goto loop_found;
    node = node->next;
    if (node != 0) goto loop;
done:;
}

static __inline__ void addref(CMemEntry *node) {
    node->lockCount++;
    node->flags |= 0x100;
}

static __inline__ void stamp(CMemEntry *node) {
    node->age = D_800D8DF0;
    func_00200818(&D_800D81E0, node);
}

static __inline__ CMemEntry *allocate(u32 size, u32 flags) {
    CMemEntry *node = func_002031E8(0, flags);
    if (node != 0) {
        addref(node);
        node->data = func_002063B4(0, size, (flags >> 5) & 1, flags);
        if (node->data != 0) {
            node->size = size;
            node->flags |= flags;
            func_00206498(0, node);
        } else {
            if (--node->lockCount == 0) node->flags &= ~0x100;
            func_002062F8(0, node);
            node = 0;
        }
    }
    return node;
}

/* Returns the cached resource for `key` (loading `size` bytes from ROM offset
 * `key` when absent): synchronously when `direct` (or the manager mask) is set,
 * otherwise queues the read and returns 0. */
CMemEntry *func_0020367C(s32 unused, u32 key, u32 size, u32 mode, s32 owner, void *tag, s32 spare, u32 direct) {
    CCacheEntry *request;
    CCacheEntry *scan;
    CCacheEntry *resolved;
    CMemEntry *node;
    u32 flags;
    u32 oldFlags;
    s32 busy;
    CCacheEntry *allocated;

    acquire();
    find(key, &request);
    if (request != 0 && request->requestLength <= size) {
        addref(request->memEntry);
        stamp(request->memEntry);
        release();
        return request->memEntry;
    }
    direct |= D_800D8450.mask;
    busy = 0;
    if (direct != 0) {
        allocated = func_002066F8(0, mode);
    } else {
        scan = D_800D8450.active.m_pHead;
        while (scan != 0) {
            if (scan->requestId == key && size <= scan->requestLength) {
                busy = 1;
                break;
            }
            scan = scan->next;
        }
        if (direct == 0 && busy) goto failed;
        allocated = func_002066F8(0, mode);
    }
    request = allocated;
    if (request != 0) {
        if (tag != 0) flags = (mode & 0x1C) | 0x22;
        else flags = mode;
        request->referencedMemEntry = 0;
        node = allocate(size, flags);
        request->memEntry = node;
        if (request->memEntry != 0) {
            if (size & 1) size++;
            request->requestId = key;
            request->requestLength = size;
            request->notifyId = owner;
            request->decompressCallback = tag;
            request->memFlags = mode;
            CList__Remove(&D_800D8450.pending, request);
            CList__AddTail(&D_800D8450.active, request);
            if (direct != 0) {
                if (tag != 0) request->flags |= 4;
                release();
                func_00201DBC(&D_800D1CD8, key, size, request->memEntry->data);
                if (tag != 0) func_00201B50(&D_800D6D68, request);
                acquire();
                if (tag != 0) request->flags &= ~4;
                resolved = func_002044A8(0, request, 0);
                if (resolved != 0 && resolved != request) addref(resolved->memEntry);
                release();
                if (resolved != 0) return resolved->memEntry;
                return 0;
            } else {
                oldFlags = request->flags;
                request->flags = oldFlags | 2;
                if (tag != 0) request->flags = oldFlags | 0xE;
                release();
                func_00201E94(&D_800D1CD8, key, size, request->memEntry->data, &D_800D8450, request, 0);
                return 0;
            }
        }
        func_002066B0(0, request);
    }
failed:
    release();
    return 0;
}
