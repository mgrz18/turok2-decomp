#include "common.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"

s32 Music_SetTrack(s32);
s32 MenuPool_OpenChildMenu_2(s32, s32, s32);

s32 Sound__StopAllExcept(s32);                     /* extern */
s32 Sound__SetForce2D(s32);                     /* extern */
s32 Music_GetTrack();                                /* extern */
s32 Music_RestoreVolume();                            /* extern */
s32 Sound__DisableAmbientTrack();                            /* extern */
s32 Controller_DisableMenuSounds();                            /* extern */

s32 PauseMenu_SoundOptions_2(s32 arg0, s32 arg1, s32 arg2) {
    Sound__StopAllExcept(0x12C);
    Sound__DisableAmbientTrack();
    sSoundMenuSavedTrack = Music_GetTrack();
    Music_SetTrack(sSoundMenuTrack);
    Music_RestoreVolume();
    Sound__SetForce2D(1);
    sSoundMenuActive = 1;
    Controller_DisableMenuSounds();
    MenuPool_OpenChildMenu_2(arg2, arg1, (s32) &gSoundOptionsMenuDesc_2);
    return 1;
}
