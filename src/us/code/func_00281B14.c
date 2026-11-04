#include "common.h"
#include "structs/camera.h"

void CList__AddHead(CList *list, void *node);                /* extern */
void CList__Remove(CList *list, void *node);                 /* extern */

/* A camera's fading text goes back to the pool's free list. */
void func_00281B14(CCameraPool *pool, CCamera *camera, struct CCameraText *text) {
    CList__Remove(&camera->m_ActiveTextList, text);
    CList__AddHead(&pool->m_FreeTextList, text);
}
