#include "common.h"

s32 CEngineApp__Boot(s32 *);                   /* extern */
extern s32 D_800F6CB0;

void Engine_Boot(void) {
    CEngineApp__Boot(&D_800F6CB0);
}
