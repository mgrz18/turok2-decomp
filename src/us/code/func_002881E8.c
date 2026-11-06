#include "common.h"

s32 CEngineApp__Boot(s32 *);                   /* extern */
extern s32 gEngineApp;

void Engine_Boot(void) {
    CEngineApp__Boot(&gEngineApp);
}
