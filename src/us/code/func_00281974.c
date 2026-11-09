#include "common.h"
#include "structs/camera.h"
#include "protos/list.h"

/* The block at arg0 + 0x40 of CCameraPool__ReleaseAllTexts: a list at 0xE28 whose elements go back to the list at arg0 + 0xF0C. */
typedef struct {
    /* 0x000 */ u8 pad0[0xE28];
    /* 0xE28 */ CList m_List;
} ListOwner;


void CCameraPool__ReleaseCameraTexts(s32 arg0, ListOwner *arg1) {
    s32 *temp_s0;
    s32 *var_s1;
    void *var_a0;

    var_s1 = arg1->m_List.m_pTail;
    if (var_s1 != NULL) {
        var_a0 = &arg1->m_List;
        do {
            temp_s0 = *var_s1;
            CList__Remove(var_a0, var_s1);
            CList__AddHead(arg0 + 0xF0C, var_s1);
            var_s1 = temp_s0;
            var_a0 = &arg1->m_List;
        } while (var_s1 != NULL);
    }
}
