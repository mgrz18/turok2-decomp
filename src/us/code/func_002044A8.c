/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCENE/802508E0_6.CPP, func_80252774_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/cache.h"

typedef struct HashEntry {
    /* 0x00 */ u32 key;
    /* 0x04 */ CCacheEntry *value;
    /* 0x08 */ s32 index;
    /* 0x0C */ struct HashEntry *next;
} HashEntry;

extern char D_800D8D90[];           /* pending request list */
extern u32 D_800D81C0;              /* hash mask */
extern HashEntry *D_800D81C4;       /* hash table */
extern char D_800D1CC0[];           /* heap */
extern s32 D_800D81E0;              /* cached node list; the current node sits 8 bytes before it, the free node count 0xBCC after */
extern char D_800D81F4[];           /* locked node list */
extern CMemEntry **D_800D81D4; /* free node stack */

void CList__Remove(void *list, void *item);
void CList__AddHead(void *list, void *item);
void func_00202A08(u32 key);
void func_002026EC(void *heap, s32 data);
void func_002066B0(s32 unused, CCacheEntry *request);
void func_00202934(u32 key, CCacheEntry *value);

static __inline__ void find(u32 key, CCacheEntry **out) {
    HashEntry *node = D_800D81C4 + (((key << 5) ^ (key >> 1) ^ (key >> 9) ^ (key >> 17)) & D_800D81C0);

    if (node->key == key) {
        *out = node->value;
        return;
    }
    *out = 0;
    while (node != 0) {
        if (node->key == key) {
            *out = node->value;
            return;
        }
        node = node->next;
    }
}

static __inline__ void release(CMemEntry *node) {
    CList__Remove(&D_800D81E0, node);
    if (node->flags & 0x1000) {
        CList__Remove(D_800D81F4, node);
    }
    if (*(&D_800D81E0 - 2) == (s32) node) {
        *(&D_800D81E0 - 2) = 0;
    }
    node->flags = 0;
    D_800D81D4[*(&D_800D81E0 + 0x2F3)] = node;
    *(&D_800D81E0 + 0x2F3) += 1;
}

CCacheEntry *func_002044A8(s32 unused, CCacheEntry *request, s32 dropReference) {
    CCacheEntry *existing;
    CMemEntry *node;

    CList__Remove(D_800D8D90, request);
    CList__AddHead(D_800D8D90 - 0x14, request);
    if (request->memEntry != 0) {
        find(request->requestId, &existing);
        if (existing != 0) {
            if ((s32) existing->requestLength < (s32) request->requestLength) {
                func_00202A08(existing->requestId);
                existing->memEntry->cacheEntry = 0;
                if (!(existing->memEntry->flags & 0x702)) {
                    func_002026EC(D_800D1CC0, existing->memEntry->data);
                    release(existing->memEntry);
                }
                func_002066B0(0, existing);
            } else {
                func_002026EC(D_800D1CC0, request->memEntry->data);
                release(request->memEntry);
                func_002066B0(0, request);
                return existing;
            }
        }
        func_00202934(request->requestId, request);
        request->memEntry->cacheEntry = request;
        if (dropReference != 0) {
            node = request->memEntry;
            if (--node->lockCount == 0) {
                node->flags &= ~0x100;
            }
        }
        return request;
    }
    func_002066B0(0, request);
    return 0;
}
