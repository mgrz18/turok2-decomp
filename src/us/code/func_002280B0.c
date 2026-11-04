#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x000 */ CGameObjectInstance obj;
    /* 0x288 */ u8 links[8];
    /* 0x290 */ s32 *ownerCount;
} Effect;

s32 CList__AddTail(s32 *, void *);           /* extern */
s32 CList__Remove(s32 *, void *);           /* extern */

extern s32 D_8010FEA0;
extern s32 D_8010FEB4;

void func_002280B0(Effect *arg0) {
    s32 *temp_v1;

    temp_v1 = arg0->ownerCount;
    arg0->obj.m_AI.m_Health = 0;
    if (temp_v1 != NULL) {
        *temp_v1 -= 1;
    }
    CList__Remove(&D_8010FEB4, arg0);
    CList__AddTail(&D_8010FEA0, arg0);
}
