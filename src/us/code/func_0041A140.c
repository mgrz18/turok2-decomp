#include "common.h"

extern void Sound__StopAllExcept(s32 volume);
extern void Sound__DisableAmbientTrack(void);
extern s32 Music_GetTrack(void);
extern void Music_SetTrack(s32 track);
extern void Music_RestoreVolume(void);
extern void Sound__SetForce2D(s32 on);
extern void Controller_DisableMenuSounds(void);
extern s32 MenuPool_OpenChildMenu(s32, s32, s32);
extern u8 D_004394B4[];   /* menu template */
extern s32 D_800C1F70;    /* saved music track */
extern s32 D_800C1F74;
extern s32 D_800C1F78;

/* Opens the menu D_004394B4, switching the music (restored by SoundMenu_GoBack). */
s32 PauseMenu_SoundOptions(s32 arg0, s32 arg1, s32 arg2) {
    Sound__StopAllExcept(300);
    Sound__DisableAmbientTrack();
    D_800C1F70 = Music_GetTrack();
    Music_SetTrack(D_800C1F78);
    Music_RestoreVolume();
    Sound__SetForce2D(1);
    D_800C1F74 = 1;
    Controller_DisableMenuSounds();
    MenuPool_OpenChildMenu(arg2, arg1, (s32) D_004394B4);
    return 1;
}
