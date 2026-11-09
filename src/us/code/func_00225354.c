#include "common.h"
#include "structs/camera.h"
#include "structs/event.h"
#include "protos/list.h"

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

s32 LevelEvent_Fire(s32);

f32 HalfToFloat(u16);                             /* extern */

void CScene__QueueDelayedEvent(VoiceOwner *arg0, LevelEvent *arg1) {
    f32 temp_f0;
    Voice *var_s0;

    var_s0 = arg0->freeVoices.m_pHead;
    if (var_s0 != 0) {
        CList__Remove(&arg0->freeVoices, var_s0);
        CList__AddHead(&arg0->activeVoices, var_s0);
        var_s0->def = (sptr) arg1;
    } else {
        var_s0 = arg0->activeVoices.m_pTail;
        LevelEvent_Fire(var_s0->def);
        var_s0->def = (sptr) arg1;
    }
    temp_f0 = HalfToFloat(arg1->delay);
    var_s0->length = temp_f0;
    if (arg1->flags & 2) {
        var_s0->length = (f32) (temp_f0 * 15.0f);
    }
}
