/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCENE/802508E0_6.CPP, func_80252774_de, with Turok 2's layouts. */

#include "common.h"

typedef struct CachedResource {
    /* 0x00 */ s32 data;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 count;
    /* 0x0C */ u32 flags;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ struct ResourceLoadRequest *owner;
} CachedResource;

typedef struct ResourceLoadRequest {
    /* 0x00 */ CachedResource *node;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u32 key;
    /* 0x0C */ s32 priority;
} ResourceLoadRequest;

typedef struct HashEntry {
    /* 0x00 */ u32 key;
    /* 0x04 */ ResourceLoadRequest *value;
    /* 0x08 */ s32 index;
    /* 0x0C */ struct HashEntry *next;
} HashEntry;

extern char D_800D8D90[];           /* pending request list */
extern u32 D_800D81C0;              /* hash mask */
extern HashEntry *D_800D81C4;       /* hash table */
extern char D_800D1CC0[];           /* heap */
extern s32 D_800D81E0;              /* cached node list; the current node sits 8 bytes before it, the free node count 0xBCC after */
extern char D_800D81F4[];           /* locked node list */
extern CachedResource **D_800D81D4; /* free node stack */

void CList__Remove(void *list, void *item);
void CList__AddHead(void *list, void *item);
void func_00202A08(u32 key);
void func_002026EC(void *heap, s32 data);
void func_002066B0(s32 unused, ResourceLoadRequest *request);
void func_00202934(u32 key, ResourceLoadRequest *value);

static __inline__ void find(u32 key, ResourceLoadRequest **out) {
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

static __inline__ void release(CachedResource *node) {
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

ResourceLoadRequest *func_002044A8(s32 unused, ResourceLoadRequest *request, s32 dropReference) {
    ResourceLoadRequest *existing;
    CachedResource *node;

    CList__Remove(D_800D8D90, request);
    CList__AddHead(D_800D8D90 - 0x14, request);
    if (request->node != 0) {
        find(request->key, &existing);
        if (existing != 0) {
            if (existing->priority < request->priority) {
                func_00202A08(existing->key);
                existing->node->owner = 0;
                if (!(existing->node->flags & 0x702)) {
                    func_002026EC(D_800D1CC0, existing->node->data);
                    release(existing->node);
                }
                func_002066B0(0, existing);
            } else {
                func_002026EC(D_800D1CC0, request->node->data);
                release(request->node);
                func_002066B0(0, request);
                return existing;
            }
        }
        func_00202934(request->key, request);
        request->node->owner = request;
        if (dropReference != 0) {
            node = request->node;
            if (--node->count == 0) {
                node->flags &= ~0x100;
            }
        }
        return request;
    }
    func_002066B0(0, request);
    return 0;
}
