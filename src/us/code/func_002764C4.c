/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_7.c, func_80259784_de.
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
struct ManagerRecAtD8V2;
typedef struct ManagerRecAtD8V2 ManagerRecAtD8V2;
struct Link_func_80259784_de;
typedef struct Link_func_80259784_de Link_func_80259784_de;
struct NodeRecAt4C;
struct NodeRecAt4C {
    struct NodeRecAt4C *prev;
    struct NodeRecAt4C *next;
    char pad8[0x14];
    s32 timer;
    char pad20[0x20];
    s16 priority;
    char pad42[6];
    s32 owner;
    Vec3 position;
};
struct Link_func_80259784_de;
struct NodeRecAt4C;
struct Link_func_80259784_de {
    struct NodeRecAt4C *prev;
    struct NodeRecAt4C *next;
};
struct Entry_func_80259784_de;
typedef struct Entry_func_80259784_de Entry_func_80259784_de;
struct Entry_func_80259784_de;
struct Entry_func_80259784_de {
    char pad0[0xAC];
    s32 busy;
    char padB0[0x1C];
};
struct SceneCode802591C0;
struct SceneCode802591C0 {
    char pad0[0x1DBC];
    Entry_func_80259784_de entries[1];
    char pad1E88[0x2A84 - 0x1E88];
    s32 selection;
    s32 previousSelection;
    char pad2A8C[0x28];
    s16 activeSelection;
    s16 pendingSelection;
};
struct ManagerRecAtD8V2;
struct SceneCode802591C0;
struct ManagerRecAtD8V2 {
    struct SceneCode802591C0 *scene;
    Link_func_80259784_de active;
    char padC[0xCC];
    Link_func_80259784_de free;
};
struct NodeRecAt4C;
typedef struct NodeRecAt4C NodeRecAt4C;
extern void *func_0029E460(void *destination, const void *source, int count);
void func_002764C4(ManagerRecAtD8V2 *manager, s32 index, s32 owner, Vec3 *position) {
    Entry_func_80259784_de *entry;
    NodeRecAt4C *node;
    NodeRecAt4C *at;
    NodeRecAt4C *first;
    s32 priority;
    entry = &manager->scene->entries[index];
    if (entry->busy != 0) {
        return;
    }
    first = manager->free.next;
    if (first != (NodeRecAt4C *)&manager->free) {
        first->prev->next = first->next;
        first->next->prev = first->prev;
        node = first;
        func_0029E460(node->pad8, entry, 0xCC);
        node->owner = owner;
        node->position.x = position->x;
        node->position.y = position->y;
        node->position.z = position->z;
        node->timer = 0;
        at = manager->active.next;
        priority = node->priority;
        while (at != (NodeRecAt4C *)&manager->active) {
            if (at->priority < priority) {
                break;
            }
            at = at->next;
        }
        node->prev = at->prev;
        node->next = at;
        at->prev->next = node;
        at->prev = node;
    }
    manager->scene->activeSelection = 0;
    manager->scene->pendingSelection = -1;
    manager->scene->previousSelection = -1;
    manager->scene->selection = -1;
}
