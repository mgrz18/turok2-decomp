#include "common.h"

s32 Music_SetTrack(s32);
s32 MenuItem_SelectSubmenu(s32, s32, s32);

s32 Sound__StopAllExcept(s32);                     /* extern */
s32 Sound__SetForce2D(s32);                     /* extern */
s32 Music_DuckVolume();                            /* extern */
s32 Controller_EnableMenuSounds();                            /* extern */
extern s32 D_800C1F70;
extern s32 D_800C1F74;

void SoundMenu_GoBack_2(s32 arg0, s32 arg1, s32 arg2) {
    Sound__StopAllExcept(-1);
    Sound__SetForce2D(0);
    if (D_800C1F74 != 0) {
        Music_DuckVolume();
    }
    Music_SetTrack(D_800C1F70);
    Controller_EnableMenuSounds();
    MenuItem_SelectSubmenu(arg0, arg1, arg2);
}
