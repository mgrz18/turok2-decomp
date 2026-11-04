#include "common.h"

/* A node of a list sorted by time: next at 0, its time at 0x10. */
typedef struct TimedNode {
    /* 0x00 */ struct TimedNode *next;
    /* 0x04 */ u8 pad4[0xC];
    /* 0x10 */ s32 time;
} TimedNode;

/* The list: its head at 0 and the current time at 0x20. */
typedef struct {
    /* 0x00 */ TimedNode *head;
    /* 0x04 */ u8 pad4[0x1C];
    /* 0x20 */ s32 now;
} TimedList;

s32 func_0029B020(s32 *, s32 *, s32); /* extern */
extern s32 D_800AA4B0;
extern s32 D_800AA4B4;

s32 func_00294C70(TimedList *arg0, TimedNode **arg1) {
    s32 var_s0;
    TimedNode *var_a0;

    var_s0 = 0x7FFFFFFF;
    if (arg0->head == NULL) {
        func_0029B020(&D_800AA4B0, &D_800AA4B4, 0x133);
    }
    *arg1 = NULL;
    var_a0 = arg0->head;
    if (var_a0 != NULL) {
        do {
            if ((var_a0->time - arg0->now) < var_s0) {
                *arg1 = var_a0;
                var_s0 = var_a0->time - arg0->now;
            }
            var_a0 = var_a0->next;
        } while (var_a0 != NULL);
    }
    return (*arg1)->time;
}
