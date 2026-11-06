#include "common.h"

void Credits_Draw(s32);                         /* extern */
void Credits_Update(s32);                         /* extern */
void CEngineApp__DrawWorld();                            /* extern */

void EngineMode16_Ending_Draw(s32 arg0) {
    CEngineApp__DrawWorld();
    Credits_Draw(arg0);
    Credits_Update(arg0);
}
