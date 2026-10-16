/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802846C8_de_17.c, func_80285D30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct IntrusiveList;
struct IntrusiveList {
    void *head;
    void *tail;
    s32 prevOffset;
    s32 nextOffset;
    s32 count;
};
typedef struct IntrusiveList IntrusiveList;
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
struct Clip;
struct Clip {
    s16 mode;
    s16 frames;
};
struct Clip;
typedef struct Clip Clip;
struct NodeCode8028469C;
struct NodeCode8028469C {
    struct NodeCode8028469C *previous;
    struct NodeCode8028469C *next;
    Clip *key;
    Vec3 position;
    f32 value0;
    f32 value1;
    u32 amplitudeHandle;
    char pad24[8];
    u32 lifetimeHandle;
    char pad30[8];
    struct NodeCode8028469C **owner;
};
struct NodeCode8028469C;
typedef struct NodeCode8028469C NodeCode8028469C;
extern s32 CList__AddTail(IntrusiveList *list, void *node);
struct NodeCode8028469C;
struct PoolRecAt28V1;
struct PoolRecAt28V1 {
    IntrusiveList free;
    IntrusiveList active;
    s32 baseLevel;
};
struct PoolRecAt28V1;
typedef struct PoolRecAt28V1 PoolRecAt28V1;
void func_0026EDA8(void *arg0) {
    extern void func_0029E010(s32 arg0);
    extern s32 func_0029DFF0(void);
    extern void CList__Remove(void *, s32);
    void *node;
    void *next;
    s32 saved;
    saved = func_0029DFF0();
    node = ((PoolRecAt28V1 *)arg0)->active.head;
    if (node != 0) {
        do {
            next = ((NodeCode8028469C *)node)->next;
            CList__Remove(&((PoolRecAt28V1 *)arg0)->active, node);
            CList__AddTail(arg0, node);
            node = next;
        } while (node != 0);
    }
    ((PoolRecAt28V1 *)arg0)->baseLevel = 0;
    func_0029E010(saved);
}
