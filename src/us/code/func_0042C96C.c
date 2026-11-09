/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0044E2B8.c, func_0044E3E4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "protos/list.h"
#ifdef TARGET_PORT
/* The port: the scene's delayed events typed (structs/world.h): fire them all and free them. */
#include "common.h"
#include "structs/world.h"

extern void LevelEvent_Fire(s32);
void func_0042C96C(CScene *scene) {
    CDelayedEvent *d = scene->m_DelayedEvents.m_pHead;
    CDelayedEvent *next;

    while (d != NULL) {
        next = d->m_pNext;
        LevelEvent_Fire((s32)(sptr)d->m_pEvent);
        CList__Remove(&scene->m_DelayedEvents, d);
        CList__AddHead(&scene->m_FreeDelayedEvents, d);
        d = next;
    }
}
#else
struct Entry_func_8044D794_de;
typedef struct Entry_func_8044D794_de Entry_func_8044D794_de;
struct Owner_func_8044D794_de;
typedef struct Owner_func_8044D794_de Owner_func_8044D794_de;
struct Entry_func_8044D794_de;
struct Entry_func_8044D794_de {
    char pad0[4];
    struct Entry_func_8044D794_de *next;
    s32 handle;
};
struct Entry_func_8044D794_de;
struct Owner_func_8044D794_de;
struct Owner_func_8044D794_de {
    char pad0[0x6C];
    void *unk6C;
    char pad70[0xC];
    void *unk7C;
    char pad80[0x1100];
    struct Entry_func_8044D794_de *active;
    char pad1184[0x10]; /* active at 0x1180, spare at 0x1194 */
    struct Entry_func_8044D794_de *spare;
};
extern void LevelEvent_Fire(s32);
void func_0042C96C(Owner_func_8044D794_de *arg0) {
    Entry_func_8044D794_de *entry;
    Entry_func_8044D794_de *next;
    entry = arg0->active;
    while (entry != 0) {
        next = entry->next;
        LevelEvent_Fire(entry->handle);
        CList__Remove(&arg0->active, entry);
        CList__AddHead(&arg0->spare, entry);
        entry = next;
    }
}
#endif
