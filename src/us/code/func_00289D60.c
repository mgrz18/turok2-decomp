#include "common.h"

void Credits_Draw(sptr);                         /* extern */
void Credits_Update(sptr);                         /* extern */
void CEngineApp__DrawWorld();                            /* extern */

void EngineMode16_Ending_Draw(sptr arg0) {
    CEngineApp__DrawWorld();
    Credits_Draw(arg0);
    Credits_Update(arg0);
}
