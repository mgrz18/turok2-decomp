#include "common.h"

s32 Music_SetTrack(s32);
s32 MenuPool_OpenChildMenu_2(s32, s32, s32);

s32 Sound__StopAllExcept(s32);                     /* extern */
s32 Sound__SetForce2D(s32);                     /* extern */
s32 Music_GetTrack();                                /* extern */
s32 Music_RestoreVolume();                            /* extern */
s32 Sound__DisableAmbientTrack();                            /* extern */
s32 Controller_DisableMenuSounds();                            /* extern */
extern s32 D_004393D4;
extern s32 D_800C1F70;
extern s32 D_800C1F74;
extern s32 D_800C1F78;

s32 PauseMenu_SoundOptions_2(s32 arg0, s32 arg1, s32 arg2) {
    Sound__StopAllExcept(0x12C);
    Sound__DisableAmbientTrack();
    D_800C1F70 = Music_GetTrack();
    Music_SetTrack(D_800C1F78);
    Music_RestoreVolume();
    Sound__SetForce2D(1);
    D_800C1F74 = 1;
    Controller_DisableMenuSounds();
    MenuPool_OpenChildMenu_2(arg2, arg1, (s32) &D_004393D4);
    return 1;
}
