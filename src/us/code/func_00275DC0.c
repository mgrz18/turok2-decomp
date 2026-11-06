#include "common.h"

s32 Sound__Stop(s32);                         /* extern */
extern s32 D_800B2A88;

typedef struct Node00275DC0 {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ struct Node00275DC0 *next;
    /* 0x8 */ s32 id;
} Node00275DC0;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ Node00275DC0 *head;
    /* 0x18 */ u8 pad18[0x28 - 0x18];
    /* 0x28 */ s32 done;
} List00275DC0;

void LoopSounds_Pause(List00275DC0 *arg0) {
    s32 temp_v0;
    Node00275DC0 *var_s0;

    var_s0 = arg0->head;
    if (var_s0 != NULL) {
        do {
            Sound__Stop(var_s0->id);
            temp_v0 = var_s0->id;
            var_s0 = var_s0->next;
            D_800B2A88 = temp_v0;
        } while (var_s0 != NULL);
    }
    arg0->done = 1;
}
