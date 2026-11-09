#include "common.h"

s32 Credits_Draw(sptr);                         /* extern */
s32 Credits_Update(sptr);                         /* extern */
s32 CEngineApp__DrawWorld();                            /* extern */

void EngineMode15_Ending_Draw(sptr arg0) {
    CEngineApp__DrawWorld();
    Credits_Draw(arg0);
    Credits_Update(arg0);
}
