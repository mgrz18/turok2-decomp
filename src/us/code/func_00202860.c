/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO_2/80255280.CPP, func_802552E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
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
extern s32 D_800AB0B0;
extern u32 D_800AB0B4;
struct EntryRecAtCV1;
typedef struct EntryRecAtCV1 EntryRecAtCV1;
struct EntryRecAtCV1;
struct EntryRecAtCV1 {
    u32 key;
    struct ResourceLoadRequest *value;
    s32 index;
    struct EntryRecAtCV1 *next;
};
extern s32 D_800D81C8;
extern EntryRecAtCV1 *D_800D81C4;
extern void CHashTable__Construct(s32 capacity);
extern s32 D_800D81C0;
extern char D_800D1CC0;
extern EntryRecAtCV1 *func_002024E0(void *, s32);
void CHashTable__Construct(s32 capacity)
{
    s32 bits = 0;
    s32 top = 0;
    s32 i;
    s32 half;
    u32 n;
    for (i = 0; i < 32; i++) {
        s32 bit = 1 << i;
        if (capacity & bit) {
            bits++;
            top = i;
        }
    }
    if (bits != 1) {
        top++;
    }
    half = 1 << top;
    D_800AB0B4 = half * 2;
    D_800AB0B0 = half;
    D_800D81C0 = D_800AB0B4 - 2;
    D_800D81C8 = D_800AB0B4 - 1;
    D_800D81C4 = func_002024E0(&D_800D1CC0, half << 5);
    for (n = 0; n < D_800AB0B4; n++) {
        D_800D81C4[n].value = 0;
        D_800D81C4[n].key = 0;
        D_800D81C4[n].next = 0;
        D_800D81C4[n].index = n;
    }
}
