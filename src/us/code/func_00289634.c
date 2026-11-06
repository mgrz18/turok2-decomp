#include "common.h"

void Music_SetTrack(s32);                     /* extern */

void EngineMode07_Title_Init(void) {
    Music_SetTrack(0x10);
}
