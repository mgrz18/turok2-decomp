/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A7480.c, func_002A7480.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py.
 * On structs/swoosh.h's CSwooshSystem and Swoosh. */

#include "common.h"
#include "structs/swoosh.h"

void Trail_PlaceNodes(Swoosh *e);
void Trail_ComputeFrames(void *arg0, sptr arg1);
void Trail_DrawTextured(void *pool, Swoosh *trail, s32 unused);
void Trail_DrawUntextured(CSwooshSystem *pool, Swoosh *actor, s32 unused);
/* Static inline in the Rage Wars source file (dropped by the importer). */
static inline void upd(CSwooshSystem *m, Swoosh *e, s32 c) {
    if (e->flags & 8) {
        Trail_PlaceNodes(e);
    }
    if (e->flags & 4) {
        Trail_ComputeFrames(e, c);
    }
    if (e->nodes.count >= 2) {
        if (e->layer >= 0) {
            Trail_DrawTextured(m, e, c);
        } else {
            Trail_DrawUntextured(m, e, c);
        }
    }
}
/* Draws the live swooshes: the untextured layers (< 0) first, then the textured ones (> 0). */
void SwooshSystem_DrawTrails(CSwooshSystem *m, s32 c) {
    Swoosh *e;
    for (e = m->m_SwooshPool.used.head; e != ((void *)0); e = e->next) {
        if (e->layer < 0) {
            upd(m, e, c);
        }
    }
    for (e = m->m_SwooshPool.used.head; e != ((void *)0); e = e->next) {
        if (e->layer > 0) {
            upd(m, e, c);
        }
    }
}
