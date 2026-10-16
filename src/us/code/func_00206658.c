/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de.c, func_80254E30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct IntrusiveList;
struct IntrusiveList {
    void *head;
    void *tail;
    s32 prevOffset;
    s32 nextOffset;
    s32 count;
};
typedef struct IntrusiveList IntrusiveList;
struct CachedResource;
typedef struct CachedResource CachedResource;
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
struct ResourceLoadRequest;
struct CachedResource;
struct ResourceLoadRequest;
typedef struct ResourceLoadRequest ResourceLoadRequest;
struct ResourceLoadRequest;
void *func_00206658(void) {
    extern void CList__Remove(void *, s32);
    extern s32 CList__AddHead(void *, s32);
    extern s32 D_800D8D68;
    void **p = &D_800D8D68;
    void *temp_s0 = *p;
    if (temp_s0 != 0) {
        CList__Remove(p, (s32) temp_s0);
        ((ResourceLoadRequest *)temp_s0)->flags = 1;
        CList__AddHead(((char *)p + sizeof(IntrusiveList)), (s32) temp_s0);
    }
    return temp_s0;
}
