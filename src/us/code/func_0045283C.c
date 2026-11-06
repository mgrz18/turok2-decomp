#include "common.h"
#include "globals/menu.h"

s32 Music_SetTrack(s32);
s32 MenuPool_OpenChildMenu_2(s32, s32, s32);

s32 Sound__StopAllExcept(s32);                     /* extern */
s32 Sound__SetForce2D(s32);                     /* extern */
s32 Music_GetTrack();                                /* extern */
s32 Controller_DisableMenuSounds();                            /* extern */
extern s32 gSoundOptionsMenuDesc_2;

s32 TitleMenu_SoundOptions_2(s32 arg0, s32 arg1, s32 arg2) {
    Sound__StopAllExcept(-1);
    sSoundMenuSavedTrack = Music_GetTrack();
    Music_SetTrack(sSoundMenuTrack);
    Sound__SetForce2D(1);
    sSoundMenuActive = 0;
    Controller_DisableMenuSounds();
    MenuPool_OpenChildMenu_2(arg2, arg1, (s32) &gSoundOptionsMenuDesc_2);
    return 1;
}
