#include "common.h"
#include "structs/emitter.h"
#include "protos/list.h"

/* Gives an emitter back: off the active list, onto the free list. */
void CEmitterPool__ReleaseEmitter(CEmitterPool *pool, CEmitter *emitter) {
    CList__Remove(&pool->m_ActiveList, emitter);
    CList__AddHead(&pool->m_FreeList, emitter);
}
