#include "common.h"
#include "globals/game.h"
#include "globals/particle.h"
#include "structs/swoosh.h"

s32 SwooshSystem_UpdateSwooshes();                          /* extern */
s32 SwooshSystem_UpdateTimers(CSwooshSystem *);             /* extern */

void SwooshSystem_Update(CSwooshSystem *arg0) {
    sSwooshSpawned = 0;
    if (gEngineApp_m_Settings.paused == 0) {
#ifdef TARGET_PORT
        SwooshSystem_UpdateSwooshes(arg0);  /* the N64 code leaves arg0 in $a0 for it */
#else
        SwooshSystem_UpdateSwooshes();
#endif
        SwooshSystem_UpdateTimers(arg0);
    }
}
