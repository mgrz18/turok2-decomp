#include "common.h"

/* A key event fired by an animation or a script: its handler gets (object, AI, event). */
typedef struct CKeyEvent {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ u16 param; /* sound id, ... */
} CKeyEvent;

s32 func_00275624(s32);

void func_0026E3EC(s32 arg0, s32 arg1, CKeyEvent *arg2) {
    func_00275624((s16) arg2->param);
}
