#include "common.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"

void Music_SetTrack(s32 track);

void Sound__StopAllExcept(s32 volume);
void Sound__SetForce2D(s32 on);
s32 Music_GetTrack();                                /* extern */
void Controller_DisableMenuSounds(void);

s32 TitleMenu_SoundOptions(sptr arg0, sptr arg1, sptr arg2) {
    Sound__StopAllExcept(-1);
    sSoundMenuSavedTrack = Music_GetTrack();
    Music_SetTrack(sSoundMenuTrack);
    Sound__SetForce2D(1);
    sSoundMenuActive = 0;
    Controller_DisableMenuSounds();
    MenuPool_OpenChildMenu(arg2, arg1, &gSoundOptionsMenuDesc);
    return 1;
}
