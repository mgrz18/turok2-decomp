#include "common.h"
#include "structs/sound.h"

s32 Sound__SetAmbientTrack(u16);                         /* extern */

void func_0026E44C(s32 arg0, s32 arg1, SoundEffectInfo *arg2) {
    Sound__SetAmbientTrack(arg2->flags);
}
