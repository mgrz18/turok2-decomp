#include "common.h"
#include "protos/game.h"

s32 Music_RestoreVolume();                            /* extern */
s32 Sound__EnableAmbientTrack();                            /* extern */
s32 LoopSounds_Resume(sptr);                         /* extern */

s32 PauseMenu_Resume_2(void) {
    LoopSounds_Resume(LoopSounds_Get());
    Music_RestoreVolume();
    Sound__EnableAmbientTrack();
    return 1;
}
