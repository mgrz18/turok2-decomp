#include "common.h"
#include "structs/game.h"

void Music_SetTrack(s32 track);
s32 CEngineApp__QuitToLegal(sptr);

s32 Music_Stop();                            /* extern */
void Music_RestoreVolume(void);

s32 QuitMenu_Yes(void) {
    Music_SetTrack(-1);
    Music_RestoreVolume();
    Music_Stop();
    CEngineApp__QuitToLegal((sptr) &gEngineApp);
    return 1;
}
