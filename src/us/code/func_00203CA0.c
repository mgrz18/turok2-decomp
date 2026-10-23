/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SCENE/802508E0_5.CPP, func_80251F6C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
typedef struct IntrusiveList IntrusiveList;
struct IntrusiveList {
    void *head;
    void *tail;
    s32 prevOffset;
    s32 nextOffset;
    s32 count;
};
struct IntrusiveList;
typedef struct OSMesgQueue OSMesgQueue;
struct OSMesgQueue {
    void **mtqueue;
    void **fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    void **msg;
};
struct OSMesgQueue;
extern int D_800D8DCC;
struct OSMesgQueue;
struct OSMesgQueue;
struct Manager;
struct Manager {
    char queue[0x92C];
    IntrusiveList pending;
    IntrusiveList active;
    char padActive[0x20 - sizeof(IntrusiveList)];
    char lock[0x18];
    char unk978[0x24];
    s32 mask;
};
struct CachedResource;
typedef struct CachedResource CachedResource;
struct Manager;
struct CachedResource;
struct ResourceLoadRequest;
struct CachedResource {
    void *data;
    s32 unk04;
    s32 count;
    s32 flags;
    s32 stamp;
    struct ResourceLoadRequest *owner;
};
struct ResourceLoadRequest {
    struct CachedResource *node;
    struct CachedResource *extra;
    s32 key;
    s32 priority;
    s32 flags;
    void *tag;
    s32 owner;
    s32 mode;
    void *replyQueue;
    struct ResourceLoadRequest *next;
};
struct ResourceHashEntry;
struct ResourceLoadRequest;
struct ResourceHashEntry {
    s32 key;
    struct ResourceLoadRequest *value;
    s32 unk08;
    struct ResourceHashEntry *next;
};
struct Manager;
typedef struct Manager Manager;
struct Manager;
struct CachedResource;
struct ResourceHashEntry;
typedef struct ResourceHashEntry ResourceHashEntry;
struct ResourceHashEntry;
struct ResourceLoadRequest;
typedef struct ResourceLoadRequest ResourceLoadRequest;
struct ResourceLoadRequest;
extern s32 CList__AddTail(IntrusiveList *list, void *node);
struct OSMesgQueue;
typedef void *OSMesg;
typedef u32 OSIntMask;
extern void func_0029E010(OSIntMask mask);
extern OSIntMask func_0029DFF0(void);
extern s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
extern void func_002066B0(s32 arg0, void *arg1);
void CList__AddHead(void *list, void *node);
extern void CList__Remove(void *list, void *node);
extern s32 D_800D81E0;
extern s32 D_800D8DF0;
extern s32 D_800D81C0;
extern s32 D_800D81C4;
extern u32 func_0029DFF0(void);
extern ResourceLoadRequest *func_002066F8(s32, s32);
extern void func_00201B50(s32 *, ResourceLoadRequest *);
extern void func_00200818(void *, CachedResource *);
extern ResourceLoadRequest *func_002044A8(s32 unused, ResourceLoadRequest *request, s32 dropReference);
static __inline__ void addref(CachedResource *node) {
    node->count += 1;
    node->flags |= 0x100;
}
static __inline__ void stamp(CachedResource *node) {
    node->stamp = D_800D8DF0;
    func_00200818(&D_800D81E0, node);
}
static __inline__ void unref(CachedResource *node) {
    if (--node->count == 0) {
        node->flags &= ~0x100;
    }
}
static __inline__ void acquire(void) {
    extern Manager D_800D8450;
    u32 token = func_0029DFF0();
    s32 counter = D_800D8DCC + 1;
    D_800D8DCC = counter;
    if (counter != 1) {
        func_0029E010(token);
        osRecvMesg(D_800D8450.lock, 0, 1);
    } else {
        func_0029E010(token);
    }
}
static __inline__ void release(void) {
    extern Manager D_800D8450;
    u32 token = func_0029DFF0();
    s32 counter = D_800D8DCC - 1;
    D_800D8DCC = counter;
    if (counter != 0) {
        func_0029E010(token);
        osSendMesg(D_800D8450.lock, 0, 1);
    } else {
        func_0029E010(token);
    }
}
extern s32 func_00201BDC(void *, void *, Manager *);
extern s32 D_800D6D68;
static __inline__ void lock(void) {
    extern Manager D_800D8450;
    u32 token = func_0029DFF0();
    s32 counter = D_800D8DCC + 1;
    D_800D8DCC = counter;
    if (counter != 1) {
        func_0029E010(token);
        osRecvMesg(D_800D8450.lock, 0, 1);
    } else {
        func_0029E010(token);
    }
}
CachedResource *func_00203CA0(s32 unused, s32 key, CachedResource *first, CachedResource *second, s32 mode, void *owner,
                    void *tag, void *spare, s32 async) {
    extern Manager D_800D8450;
    ResourceHashEntry *node;
    ResourceLoadRequest *found;
    ResourceLoadRequest **out;
    ResourceLoadRequest *scan;
    s32 busy;
    acquire();
    out = &found;
    node = (ResourceHashEntry *)((s32)D_800D81C4 +
        ((((key << 5) ^ ((u32)key >> 1) ^ ((u32)key >> 9) ^
           ((u32)key >> 17)) & D_800D81C0) * 0x10));
    if (node->key != key) {
        goto not_initial;
    }
    found = node->value;
    goto done;
loop_found:
    *out = node->value;
    goto done;
not_initial:
    found = 0;
    if (node == 0) {
        goto done;
    }
loop:
    if (node->key == key) {
        goto loop_found;
    }
    node = node->next;
    if (node != 0) {
        goto loop;
    }
done:
    busy = 0;
    if (found != 0) {
        addref(found->node);
        stamp(found->node);
        release();
        return found->node;
    }
    async |= D_800D8450.mask;
    scan = D_800D8450.active.head;
    while (scan != 0) {
        if (scan->key == key) {
            busy = 1;
            break;
        }
        scan = scan->next;
    }
    if (busy == 0) {
        found = func_002066F8(0, mode);
        if (found != 0) {
            found->key = key;
            found->priority = 0;
            found->owner = (s32)owner;
            found->tag = tag;
            found->mode = mode;
            found->node = first;
            found->extra = second;
            if (first != 0) {
                addref(first);
            }
            if (second != 0) {
                addref(second);
            }
            CList__Remove(&D_800D8450.pending, found);
            CList__AddTail(&D_800D8450.active, found);
            release();
            if (async != 0) {
                found->flags &= ~0xE;
                func_00201B50(&D_800D6D68, found);
                acquire();
                found = func_002044A8(0, found, 0);
                release();
                if (found != 0) {
                    return found->node;
                }
                return 0;
            }
            if (func_00201BDC(&D_800D6D68, found, &D_800D8450) == 0) {
                acquire();
                if (first != 0) {
                    unref(first);
                }
                if (second != 0) {
                    unref(second);
                }
                CList__Remove(&D_800D8450.active, found);
                CList__AddHead(&D_800D8450.pending, found);
                func_002066B0(0, found);
                release();
            }
            return 0;
        }
    }
    release();
    return 0;
}
