#include "common.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"

extern void Sound__StopAllExcept(s32 volume);
extern void Sound__DisableAmbientTrack(void);
extern s32 Music_GetTrack(void);
extern void Music_SetTrack(s32 track);
extern void Music_RestoreVolume(void);
extern void Sound__SetForce2D(s32 on);
extern void Controller_DisableMenuSounds(void);

/* Opens the menu gSoundOptionsMenuDesc, switching the music (restored by SoundMenu_GoBack). */
s32 PauseMenu_SoundOptions(s32 arg0, s32 arg1, s32 arg2) {
    Sound__StopAllExcept(300);
    Sound__DisableAmbientTrack();
    sSoundMenuSavedTrack = Music_GetTrack();
    Music_SetTrack(sSoundMenuTrack);
    Music_RestoreVolume();
    Sound__SetForce2D(1);
    sSoundMenuActive = 1;
    Controller_DisableMenuSounds();
    MenuPool_OpenChildMenu(arg2, arg1, &gSoundOptionsMenuDesc);
    return 1;
}
