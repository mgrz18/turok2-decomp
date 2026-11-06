#include "common.h"
#include "globals/game.h"
#include "globals/particle.h"

s32 SwooshSystem_UpdateSwooshes();                            /* extern */
s32 SwooshSystem_UpdateTimers(s32);                         /* extern */

void SwooshSystem_Update(s32 arg0) {
    sSwooshSpawned = 0;
    if (gEngineApp_m_Settings.paused == 0) {
        SwooshSystem_UpdateSwooshes();
        SwooshSystem_UpdateTimers(arg0);
    }
}
