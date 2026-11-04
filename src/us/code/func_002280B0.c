#include "common.h"
#include "structs/game_object.h"

s32 CList__AddTail(s32 *, void *);           /* extern */
s32 CList__Remove(s32 *, void *);           /* extern */

extern s32 D_8010FEA0;
extern s32 D_8010FEB4;

void func_002280B0(CListedInstance *arg0) {
    s32 *temp_v1;

    temp_v1 = arg0->m_pOwnerCount;
    arg0->m_Instance.m_AI.m_Health = 0;
    if (temp_v1 != NULL) {
        *temp_v1 -= 1;
    }
    CList__Remove(&D_8010FEB4, arg0);
    CList__AddTail(&D_8010FEA0, arg0);
}
