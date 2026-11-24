#include "common.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"

void Sound__StopAllExcept(s32 volume);
extern void Sound__DisableAmbientTrack(void);
extern s32 Music_GetTrack(void);
void Music_SetTrack(s32 track);
void Music_RestoreVolume(void);
void Sound__SetForce2D(s32 on);
void Controller_DisableMenuSounds(void);

/* Opens the menu gSoundOptionsMenuDesc_2, switching the music (restored by SoundMenu_GoBack_2). */
s32 PauseMenu_SoundOptions_2(sptr arg0, sptr arg1, sptr arg2) {
    Sound__StopAllExcept(300);
    Sound__DisableAmbientTrack();
    sSoundMenuSavedTrack = Music_GetTrack();
    Music_SetTrack(sSoundMenuTrack);
    Music_RestoreVolume();
    Sound__SetForce2D(1);
    sSoundMenuActive = 1;
    Controller_DisableMenuSounds();
    MenuPool_OpenChildMenu_2(arg2, arg1, &gSoundOptionsMenuDesc_2);
    return 1;
}
