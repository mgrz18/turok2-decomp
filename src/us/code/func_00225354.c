#include "common.h"
#include "structs/camera.h"
#include "structs/event.h"

typedef struct {
    /* 0x00 */ u8 links[8];
    /* 0x08 */ s32 def;
    /* 0x0C */ f32 length;
} Voice;

typedef struct {
    /* 0x0000 */ u8 pad0[0x1180];
    /* 0x1180 */ CList activeVoices;
    /* 0x1194 */ CList freeVoices;
} VoiceOwner;

s32 CList__AddHead(s32, s32);
s32 CList__Remove(s32, s32);
s32 func_0026DFB0(s32);

f32 func_0026D210(u16);                             /* extern */

void func_00225354(VoiceOwner *arg0, LevelEvent *arg1) {
    f32 temp_f0;
    Voice *var_s0;

    var_s0 = arg0->freeVoices.m_pHead;
    if (var_s0 != 0) {
        CList__Remove((s32) &arg0->freeVoices, (s32) var_s0);
        CList__AddHead((s32) &arg0->activeVoices, (s32) var_s0);
        var_s0->def = (s32) arg1;
    } else {
        var_s0 = arg0->activeVoices.m_pTail;
        func_0026DFB0(var_s0->def);
        var_s0->def = (s32) arg1;
    }
    temp_f0 = func_0026D210(arg1->delay);
    var_s0->length = temp_f0;
    if (arg1->flags & 2) {
        var_s0->length = (f32) (temp_f0 * 15.0f);
    }
}
