#include "common.h"
#include "structs/controller.h"
#include "globals/game.h"
#include "globals/pak.h"
#include "globals/controller.h"
#include "globals/rumble.h"
#include "protos/ultra.h"

/* Controller pak / rumble service, run once per frame while enabled. */


extern void RumblePool_FreeFinished(RumblePool *pool);
extern void Controller_ReadAll(void);
extern void Controller_UpdateRumble(ControllerProfile *);
extern s32 Controller_AnyPakWatched(void);
extern void osContStartQuery(void *);

void Controller_Service(void) {
    ControllerProfile *pad;
    s32 i;

    if (gControllersInitialised == 0) {
        return;
    }
    RumblePool_FreeFinished(&gRumblePool);
    if (osRecvMesg(&gPakAccessQueue, NULL, 0) == 0) {
        Controller_ReadAll();
        for (i = 0, pad = gControllers; i < 4; i++, pad++) {
            Controller_UpdateRumble(pad);
        }
        switch ((u8)gPakQueueState) {
        case 0:
            if (Controller_AnyPakWatched() != 0) {
                gPakQueueState = 1;
                osContStartQuery(&gSiMessageQueue);
                break;
            }
            osContStartReadData(&gSiMessageQueue);
            break;
        case 1:
        case 2:
        case 3:
            gPakQueueState = 0;
            osContStartReadData(&gSiMessageQueue);
            break;
        }
        osSendMesg(&gPakAccessQueue, NULL, 0);
    }
    if (gEngineApp_m_Settings.mode == 8) {
        if (--sControllerRescanTimer <= 0) {
            sControllerRescanTimer = 60;
            gPakQueueState = 1;
        }
    }
}
