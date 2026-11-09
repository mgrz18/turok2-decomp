#include "common.h"
#include "structs/camera.h"
#include "protos/list.h"


/* A camera's fading text goes back to the pool's free list. */
void CCameraPool__ReleaseText(CCameraPool *pool, CCamera *camera, struct CCameraText *text) {
    CList__Remove(&camera->m_ActiveTextList, text);
    CList__AddHead(&pool->m_FreeTextList, text);
}
