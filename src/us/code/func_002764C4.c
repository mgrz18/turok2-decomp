/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_7.c, func_80259784_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

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
