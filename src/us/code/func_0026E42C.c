#include "common.h"

/* An event record (see func_0026DFB0.c); only its sound here. */
typedef struct {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ s16 sound;
} Event;

void func_00275624(s16);                            /* extern */

void func_0026E42C(Event *arg0) {
    func_00275624(arg0->sound);
}
