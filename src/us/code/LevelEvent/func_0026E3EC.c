#include "common.h"

/* A key event fired by an animation or a script: its handler gets (object, AI, event). */
typedef struct CKeyEvent {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ u16 param; /* sound id, ... */
} CKeyEvent;

s32 Sound__Play2D(s32);

void Event_PlaySound(s32 arg0, s32 arg1, CKeyEvent *arg2) {
    Sound__Play2D((s16) arg2->param);
}
