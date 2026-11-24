#include "common.h"
#include "globals/menu.h"

void Music_SetTrack(s32 track);
s32 MenuItem_SelectSubmenu(sptr, sptr, sptr);

void Sound__StopAllExcept(s32 volume);
void Sound__SetForce2D(s32 on);
s32 Music_DuckVolume();                            /* extern */
s32 Controller_EnableMenuSounds();                            /* extern */

void SoundMenu_GoBack(sptr arg0, sptr arg1, sptr arg2) {
    Sound__StopAllExcept(-1);
    Sound__SetForce2D(0);
    if (sSoundMenuActive != 0) {
        Music_DuckVolume();
    }
    Music_SetTrack(sSoundMenuSavedTrack);
    Controller_EnableMenuSounds();
    MenuItem_SelectSubmenu(arg0, arg1, arg2);
}
