/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A7480.c, func_002A7480.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py.
 * On structs/swoosh.h's CSwooshSystem and Swoosh. */

#include "common.h"
#include "structs/swoosh.h"

void Trail_PlaceNodes(Swoosh *);
void Trail_ComputeFrames(Swoosh *, sptr);
void Trail_DrawTextured(CSwooshSystem *, Swoosh *, s32);
void Trail_DrawUntextured(CSwooshSystem *, Swoosh *, s32);
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
