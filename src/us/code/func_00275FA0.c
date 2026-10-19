/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_6.c, func_80259260_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00273EB8(void *arg0, int arg1);
extern s16 func_00276820(void *);
extern s16 func_0027682C(void *);
extern s8 func_00276874(void *);
extern void func_00273EC0(void *, void *, s32);
extern s8 func_00276838(void *);
extern u8 func_00276868(void *);
extern s16 func_00276898(void *);
s32 func_00275FA0(ManagerRecAtD8V1 *manager, void *desc, Vec3Words *position, s32 volume, s32 extra) {
    extern void func_00271514(s32 *);
    extern void func_002714B0(s32 *);
    NodeRecAtC8 *node;
    NodeRecAtC8 *first;
    NodeRecAtC8 *at;
    s32 *scene;
    s32 priority;
    if (manager->free.next != (NodeRecAtC8 *)&manager->free) {
        func_002714B0(manager->scene);
        first = manager->free.next;
        first->prev->next = first->next;
        first->next->prev = first->prev;
        scene = manager->scene;
        node = first;
        node->handle = scene[0x108 / 4];
        scene[0x108 / 4] = node->handle + 1;
        node->volume = volume;
        node->voice = -1;
        node->volume16 = volume;
        node->group = func_00276820(desc);
        node->priority = func_00276838(desc);
        node->sample = func_0027682C(desc);
        node->pan = func_00276868(desc);
        node->flags = func_00276898(desc);
        node->pitch = func_00276874(desc);
        node->gainB = 0x7D00;
        node->gainA = 0x7D00;
        node->time = 0;
        node->frame = manager->scene[0x104 / 4];
        node->position = *position;
        node->pad58 = 0;
        node->padB4 = 0;
        func_00273EB8(node->channel, ((struct ContextRecAt2BC0 *)manager->scene)->field2B8C);
        node->mix = manager->scene[0x2BAC / 4];
        func_00273EC0(node->channel, desc, extra);
        at = manager->active.next;
        priority = node->priority;
        while (at != (NodeRecAtC8 *)&manager->active) {
            if (at->priority < priority) {
                break;
            }
            at = at->next;
        }
        node->prev = at->prev;
        node->next = at;
        at->prev->next = node;
        at->prev = node;
        func_00271514(manager->scene);
        return node->handle;
    }
    return -1;
}
