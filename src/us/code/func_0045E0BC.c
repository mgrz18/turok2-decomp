/* Second-module copy of MissionMenu_SelectOk (first module; this one at link address 0x004250BC), calling this module's own functions at their link addresses. */
#include "common.h"

void Music_RestoreVolume(void);
void LoopSounds_Resume(s32);
s32 LoopSounds_Get(void);
void Controller_ResetAllRumble(void);
void Controller_SetRumbleOptionAll(s32);
void MenuItem_SelectSubmenu(s32, s32, s32);
extern s32 D_80130A34;

s32 MissionMenu_SelectOk_2(s32 arg0, s32 arg1, s32 arg2) {
    s32 ret;

    Music_RestoreVolume();
    LoopSounds_Resume(LoopSounds_Get());
    if (D_80130A34 == 0) {
        MenuItem_SelectSubmenu(arg0, arg1, arg2);
        ret = 1;
    } else {
        Controller_ResetAllRumble();
        Controller_SetRumbleOptionAll(1);
        ret = 1;
    }
    return ret;
}
