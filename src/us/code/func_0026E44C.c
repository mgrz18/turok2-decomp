#include "common.h"

s32 func_00275950(u16);                         /* extern */

/* Sound effect descriptor (SndInfo in rw/junk_types.h). */
typedef struct SoundEffectInfo {
    /* 0x00 */ s16 group;
    /* 0x02 */ s16 sample;
    /* 0x04 */ s8 priority;
    /* 0x05 */ s8 chance;
    /* 0x06 */ u16 flags;  /* low 14 bits flags, top 2 pan */
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s8 pitch;
    /* 0x0F */ s8 unkF;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 randomRange0;
    /* 0x14 */ s16 randomRange1;
} SoundEffectInfo;

void func_0026E44C(s32 arg0, s32 arg1, SoundEffectInfo *arg2) {
    func_00275950(arg2->flags);
}
