#include "common.h"

/* A key event fired by an animation or a script: its handler gets (object, AI, event). */
typedef struct CKeyEvent {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ u16 param; /* sound id, ... */
} CKeyEvent;

s32 Sound__SetAmbientTrack(u16);                         /* extern */

void func_0026E46C(CKeyEvent *arg0) {
    Sound__SetAmbientTrack(arg0->param);
}
