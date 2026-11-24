#include "common.h"
#include "structs/game.h"

extern void CEngineApp__Boot(void);

void Engine_Boot(void) {
    ((void (*)(void *)) CEngineApp__Boot)(&gEngineApp);
}
