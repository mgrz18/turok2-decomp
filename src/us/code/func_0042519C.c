#include "common.h"
#include "globals/menu.h"
#include "protos/game.h"

void Music_RestoreVolume(void);
void LoopSounds_Resume(sptr);
void Controller_ResetAllRumble(void);
void Controller_SetRumbleOptionAll(s32);
void MenuItem_SelectSubmenu(s32, sptr, s32);

s32 MissionMenu_SelectOk(s32 arg0, s32 arg1, s32 arg2) {
    s32 ret;

    Music_RestoreVolume();
    LoopSounds_Resume(LoopSounds_Get());
    if (sMissionFailed == 0) {
        MenuItem_SelectSubmenu(arg0, arg1, arg2);
        ret = 1;
    } else {
        Controller_ResetAllRumble();
        Controller_SetRumbleOptionAll(1);
        ret = 1;
    }
    return ret;
}
