#include "common.h"

extern void Music_SetTrack(s32);

void EngineMode07_Title_Init(void) {
    Music_SetTrack(0x10);
}
