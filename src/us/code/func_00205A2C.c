/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80250938_de_11.c, func_80253F8C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
struct OSMesgQueue;
typedef struct OSMesgQueue OSMesgQueue;
struct OSMesgQueue {
    void **mtqueue;
    void **fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    void **msg;
};
extern int D_800D8DCC;
struct OSMesgQueue;
struct HashNode;
struct HashNode {
    s32 key;
    void *value;
    s32 unk08;
    struct HashNode *next;
};
struct OSMesgQueue;
struct HashNode;
typedef struct HashNode HashNode;
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
struct OSMesgQueue;
extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
extern u32 func_0029DFF0(void);
extern void func_0029E010(u32 arg0);
extern void func_0029E010(u32);
extern s32 D_800D81E0;
extern s32 D_800D8DF0;
extern s32 D_800D81C0;
extern s32 D_800D81C4;
extern void func_00200818(void *, s32);
void func_00205A2C(s32 arg0, s32 arg1) {
    extern s32 osSendMesg(OSMesgQueue *, s32, s32);
    extern OSMesgQueue D_800D8DB0;
    s32 temp_v1;
    s32 temp_v1_2;
    u32 temp_a0;
    u32 temp_v0;
    HashNode *node;
    void *value;
    void **out;
    temp_a0 = func_0029DFF0();
    temp_v1 = D_800D8DCC + 1;
    D_800D8DCC = temp_v1;
    if (temp_v1 != 1) {
        func_0029E010(temp_a0);
        osRecvMesg(&D_800D8DB0, 0, 1);
    } else {
        func_0029E010(temp_a0);
    }
    out = &value;
    node = (HashNode *)((s32)D_800D81C4 +
        ((((arg1 << 5) ^ ((u32)arg1 >> 1) ^ ((u32)arg1 >> 9) ^
           ((u32)arg1 >> 17)) & D_800D81C0) * 0x10));
    if (node->key != arg1) {
        goto not_initial;
    }
    value = node->value;
    goto found;
loop_found:
    *out = node->value;
    goto found;
not_initial:
    value = 0;
    if (node == 0) {
        goto found;
    }
loop:
    if (node->key == arg1) {
        goto loop_found;
    }
    node = node->next;
    if (node != 0) {
        goto loop;
    }
found:
    if (value != 0) {
        void *object = *(void **)value;
        ((CachedResource *)object)->stamp = D_800D8DF0;
        func_00200818(&D_800D81E0, object);
    }
    temp_v0 = func_0029DFF0();
    temp_v1_2 = D_800D8DCC - 1;
    D_800D8DCC = temp_v1_2;
    if (temp_v1_2 != 0) {
        func_0029E010(temp_v0);
        osSendMesg(&D_800D8DB0, 0, 1);
        return;
    }
    func_0029E010(temp_v0);
}
