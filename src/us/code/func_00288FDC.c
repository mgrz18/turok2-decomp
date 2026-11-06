#include "common.h"
#include "structs/game.h"
#include "globals/game.h"

/* A play-time clock: frames, seconds, minutes, hours. */

/* Only the field read here. */

extern f32 D_800A9DF4;                              /* units per next unit (presumably 60) */
extern f32 gFrameIncrement;                              /* frame delta */

void CEngineApp__UpdatePlayTime(CEngineApp *arg0) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f1;

    if (arg0->m_Settings.paused == 0) {
        temp_f1 = gEngineApp_m_Settings.match.frames + gFrameIncrement;
        gEngineApp_m_Settings.match.frames = temp_f1;
        if (temp_f1 >= 15.0f) {
            temp_f0 = gEngineApp_m_Settings.match.seconds + 1.0f;
            gEngineApp_m_Settings.match.frames = (f32) (temp_f1 - 15.0f);
            gEngineApp_m_Settings.match.seconds = temp_f0;
            if (D_800A9DF4 <= temp_f0) {
                temp_f0_2 = gEngineApp_m_Settings.match.minutes + 1.0f;
                gEngineApp_m_Settings.match.seconds = (f32) (temp_f0 - D_800A9DF4);
                gEngineApp_m_Settings.match.minutes = temp_f0_2;
                if (D_800A9DF4 <= temp_f0_2) {
                    gEngineApp_m_Settings.match.minutes = (f32) (temp_f0_2 - D_800A9DF4);
                    gEngineApp_m_Settings.match.hours = (f32) (gEngineApp_m_Settings.match.hours + 1.0f);
                }
            }
        }
    }
}
