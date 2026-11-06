#include "common.h"

s32 Credits_Draw(s32);                         /* extern */
s32 Credits_Update(s32);                         /* extern */
s32 CEngineApp__DrawWorld();                            /* extern */

void EngineMode15_Ending_Draw(s32 arg0) {
    CEngineApp__DrawWorld();
    Credits_Draw(arg0);
    Credits_Update(arg0);
}
