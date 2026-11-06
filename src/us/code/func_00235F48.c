#include "common.h"

s32 SwooshSystem_UpdateSwooshes();                            /* extern */
s32 SwooshSystem_UpdateTimers(s32);                         /* extern */
extern s32 sSwooshSpawned;
extern s32 gEngineApp_m_Settings_paused;

void SwooshSystem_Update(s32 arg0) {
    sSwooshSpawned = 0;
    if (gEngineApp_m_Settings_paused == 0) {
        SwooshSystem_UpdateSwooshes();
        SwooshSystem_UpdateTimers(arg0);
    }
}
