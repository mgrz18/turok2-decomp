#include "common.h"
#include "globals/audio.h"

s32 SndVoice_Update(s32);

/* 0xCC-byte slot; 16 of them follow the owner pointer. */
typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ s32 id;          /* -1 when the slot is free */
    /* 0x0C */ u8 padC[0xCC - 0xC];
} Slot0027403C;

typedef struct {
    /* 0x000 */ u8 pad0[0x102];
    /* 0x102 */ s16 skipIndex;
} Owner0027403C;

void SndVoicePool_Update(void **arg0) {
    s32 var_s0;
    s32 var_s2;
    void *var_s1;

    var_s2 = 0;
    var_s1 = ((__typeof__(arg0))((s8 *)arg0 + 4));
    var_s0 = 0;
    do {
        if ((((Slot0027403C *)var_s1)->id != -1) && (((Owner0027403C *)*arg0)->skipIndex != var_s0)) {
            SndVoice_Update((sptr) var_s1);
            var_s2 += 1;
        }
        var_s0 += 1;
        var_s1 += 0xCC;
    } while (var_s0 < 0x10);
    sSndVoicesActive = var_s2;
    if (sSndVoicesPeak < var_s2) {
        sSndVoicesPeak = var_s2;
    }
}
