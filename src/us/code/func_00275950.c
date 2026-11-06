#include "common.h"

s32 Sound__DisableAmbientTrack();                            /* extern */
s32 Sound__EnableAmbientTrack();                            /* extern */
extern s32 D_800F2B34;

void Sound__SetAmbientTrack(s32 arg0) {
    D_800F2B34 = arg0;
    if (arg0 != 0) {
        Sound__EnableAmbientTrack();
        return;
    }
    Sound__DisableAmbientTrack();
}
