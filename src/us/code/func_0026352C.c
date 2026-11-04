#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 (*func)(s32, void *);
} CAIModeInfo;

s32 func_00220408(s32 *, s32);                  /* extern */
extern s32 D_800F7078;

void func_0026352C(s32 arg0, CAI *ai) {
    s32 (*temp_v0_2)(s32, void *);
    CAIModeInfo *temp_v0;

    if ((func_00220408(&D_800F7078, arg0) ^ 1) == 0) {
        temp_v0 = (CAIModeInfo *) ai->m_pModeCurrent;
        if (temp_v0 != NULL) {
            temp_v0_2 = temp_v0->func;
            if (temp_v0_2 != NULL) {
                temp_v0_2(arg0, ai);
            }
        }
    }
}
