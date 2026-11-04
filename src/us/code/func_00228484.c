#include "common.h"
#include "structs/camera.h"
#include "structs/game_object.h"

s32 CList__AddTail(s32, void *);                 /* extern */
s32 CList__Remove(s32, void *);                 /* extern */

/* Retires an effect: drops the owner count and moves it back to the free list. */
void func_00228484(CInstancePool *world, CListedInstance *arg1) {
    s32 *temp_v1;

    temp_v1 = arg1->m_pOwnerCount;
    arg1->m_Instance.m_AI.m_Health = 0;
    if (temp_v1 != NULL) {
        *temp_v1 -= 1;
    }
    CList__Remove((s32) &world->m_ActiveList, arg1);
    CList__AddTail((s32) &world->m_FreeList, arg1);
}
