/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/8044ACCC_26.CPP, func_8044C108_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
typedef struct IntrusiveList IntrusiveList;
struct IntrusiveList {
    void *head;
    void *tail;
    s32 prevOffset;
    s32 nextOffset;
    s32 count;
};
struct IntrusiveList;
struct IntPair;
struct IntPair {
    int field_0;
    int field_4;
};
struct IntPair;
struct Bucket;
typedef struct Bucket Bucket;
struct Bucket;
struct Bucket {
    struct Bucket *prev;
    struct Bucket *next;
    struct Record14 *object;
    f32 remaining;
};
struct Face;
struct Face {
    char pad0[0xE];
    u8 flags;
    u8 hidden;
    char pad10[3];
    u8 group;
};
struct Face;
typedef struct Face Face;
struct Face;
struct WorldRecAt1B40C;
struct WorldRecAt1B40C {
    char pad0[0x6C];
    void *tree;                /* 0x6C (Rage Wars 0x80) */
    char pad70[0x1168 - 0x70];
    s32 nfaces;                /* 0x1168 (Rage Wars 0x11C0: 0x58 earlier) */
    s32 nedges;
    void *verts;
    void *edgeData;
    struct Face *faces;
    struct Face *edges;
    IntrusiveList listA;       /* 0x1180 */
    IntrusiveList listB;       /* 0x1194 */
    Bucket buckets[0x30];      /* 0x11A8 */
    char pad14A8[0x18FA8 - 0x14A8];
    s32 group;                 /* 0x18FA8 (Rage Wars 0x1B40C) */
};
struct WorldRecAt1B40C;
typedef struct WorldRecAt1B40C WorldRecAt1B40C;
extern void CList__Construct(IntrusiveList *list, s32 prevOffset, s32 nextOffset);
extern s32 CList__AddTail(IntrusiveList *list, void *node);
typedef struct ResidentDataChunk {
    s32 width;
    s32 count;
    u8 data[1];
} ResidentDataChunk;
void *func_002017D4(void *table, s32 index);
extern s32 func_00200B00(void ***handle, s32 arg1);
extern void func_00201818(void *node, s32 arg1);
static __inline__ s32 groupHas(WorldRecAt1B40C *world, s32 n) {
    s32 index;
    void *g;
    u8 *bits;
    s32 mask;
    index = world->group;
    g = func_002017D4(func_002017D4(func_002017D4(world->tree, 0), index), 2);
    func_002017D4(g, 0);
    func_00201818(g, 1);
    bits = func_002017D4(g, 1);
    mask = 1 << (n & 7);
    return bits[n / 8] & mask;
}
void func_00464304(WorldRecAt1B40C *world, void ***handle) {
    void *node;
    struct IntPair *l0;
    struct IntPair *l1;
    struct IntPair *l2;
    struct IntPair *l3;
    s32 k;
    s32 i;
    s32 n;
    s32 count;
    Face *face;
    if (func_00200B00(handle, 0) != 0) {
        node = **handle;
        l0 = func_002017D4(node, 0);
        l1 = func_002017D4(node, 1);
        l2 = func_002017D4(node, 2);
        l3 = func_002017D4(node, 3);
        count = l0->field_4;
        world->verts = &((ResidentDataChunk *)(l0))->data[0];
        world->faces = (Face *)&((ResidentDataChunk *)(l1))->data[0];
        world->nfaces = count;
        count = l2->field_4;
        world->edgeData = &((ResidentDataChunk *)(l2))->data[0];
        world->edges = (Face *)&((ResidentDataChunk *)(l3))->data[0];
        world->nedges = count;
        CList__Construct(&world->listA, 0, 4);
        CList__Construct(&world->listB, 0, 4);
        for (k = 0; k < 0x30; k++) {
            CList__AddTail(&world->listB, &world->buckets[k]);
        }
        n = world->nfaces;
        for (i = 0; i < n; i++) {
            face = &world->faces[i];
            if (face->hidden != 0) {
                face->flags &= 0xFE;
            }
            if (groupHas(world, face->group)) {
                face->flags |= 0xF0;
            }
        }
        n = world->nedges;
        for (i = 0; i < n; i++) {
            face = &world->edges[i];
            if (face->hidden != 0) {
                face->flags &= 0xFE;
            }
            if (groupHas(world, face->group)) {
                face->flags |= 0xF0;
            }
        }
    }
}
