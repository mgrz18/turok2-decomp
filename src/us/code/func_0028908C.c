#include "common.h"

s32 CScene__Update(s32);                         /* extern */
s32 COnScreen__DrawBossBars(s32);                         /* extern */
s32 func_00281354(s32);                         /* extern */
s32 CEngineApp__DrawFade(s32);                         /* extern */
s32 Automap_Update(s32);                         /* extern */
s32 Automap_IsShown(s32);                             /* extern */

void EngineMode13_Game_Draw(s32 arg0) {
    s32 temp_s0;

    temp_s0 = arg0 + 0x23DF0;
    Automap_Update(temp_s0);
    CScene__Update(arg0 + 0x3C8);
    func_00281354(arg0 + 0x22C00);
    COnScreen__DrawBossBars(arg0 + 0x19440);
    if (Automap_IsShown(temp_s0) == 0) {
        CEngineApp__DrawFade(arg0);
    }
}
