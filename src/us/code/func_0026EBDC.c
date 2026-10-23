/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00285B34.c, func_80285B64_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef float f32;
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
struct Node_func_80285B64_de;
struct Node_func_80285B64_de {
    char pad0[8];
    Clip *key;
    Vec3 position;
    f32 value0;
    f32 value1;
    char pad20[0x18];
    struct Node_func_80285B64_de **owner;
};
struct Node_func_80285B64_de;
typedef struct Node_func_80285B64_de Node_func_80285B64_de;
struct Node_func_80285B64_de;
struct Pool_func_80285B64_de;
struct Pool_func_80285B64_de {
    struct Node_func_80285B64_de *free;
    char pad4[0x10];
    struct Node_func_80285B64_de *active;
};
struct Pool_func_80285B64_de;
typedef struct Pool_func_80285B64_de Pool_func_80285B64_de;
extern char D_800F7078;
extern s32 func_0029DFF0(void);
extern void func_0029E010(s32 mask);
extern void CList__Remove(Pool_func_80285B64_de *pool, Node_func_80285B64_de *node);
extern void CList__AddTail(Node_func_80285B64_de **list, Node_func_80285B64_de *node);
extern s32 func_00225D94(void *table, s32 id);
extern void func_0026EE30(void *slot, s32 handle);
static __inline__ void bind(Node_func_80285B64_de *node, s32 offset, s32 id) {
    func_0026EE30((char *)node + offset, func_00225D94(&D_800F7078, id));
}
static __inline__ void init(Node_func_80285B64_de *node, Clip *key, Vec3 position, f32 value0, f32 value1, Node_func_80285B64_de **owner) {
    node->key = key;
    node->position = position;
    node->value0 = value0;
    node->value1 = value1;
    node->owner = owner;
    if (owner != 0) {
        *owner = node;
    }
    bind(node, 0x20, key->mode);
    bind(node, 0x2C, key->frames);
}
Node_func_80285B64_de *func_0026EBDC(Pool_func_80285B64_de *pool, Clip *key, Vec3 position, f32 value0, f32 value1, Node_func_80285B64_de **owner) {
    s32 mask;
    Node_func_80285B64_de *node;
    mask = func_0029DFF0();
    node = pool->free;
    if (node != 0) {
        CList__Remove(pool, node);
        CList__AddTail(&pool->active, node);
        init(node, key, position, value0, value1, owner);
    }
    func_0029E010(mask);
    return node;
}
