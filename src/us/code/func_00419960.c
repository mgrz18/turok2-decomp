#include "common.h"

s32 Music_SetTrack(s32);
s32 func_00430330(s32);

s32 Music_Stop();                            /* extern */
s32 Music_RestoreVolume();                            /* extern */
extern s32 D_800F6CB0;

s32 QuitMenu_Yes(void) {
    Music_SetTrack(-1);
    Music_RestoreVolume();
    Music_Stop();
    func_00430330((s32) &D_800F6CB0);
    return 1;
}
