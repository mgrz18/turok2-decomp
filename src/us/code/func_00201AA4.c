#include "common.h"

/* A mutex: a one-message queue (0x18 bytes) plus its message slot. */
typedef struct {
    /* 0x00 */ u8 queue[0x18];
    /* 0x18 */ s32 msg;
    /* 0x1C */ s32 owner;
} CMutex;

extern void func_0029B030(void *, void *, s32);     /* osCreateMesgQueue */

void CMutex__Construct(CMutex *arg0) {
    arg0->owner = 0;
    func_0029B030(arg0, &arg0->msg, 1);
}
