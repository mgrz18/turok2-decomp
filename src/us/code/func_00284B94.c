#include "common.h"
#include "structs/controller.h"

/* Controller pak / rumble service, run once per frame while enabled. */

extern u8 gControllersInitialised;           /* service enabled */
extern s32 sControllerRescanTimer;          /* rescan countdown */
extern u8 gRumblePool[];
extern u8 gPakQueueState;           /* rescan request */
extern u8 gPakAccessQueue[];        /* OSMesgQueue: pak access lock */
extern u8 gSiMessageQueue[];
extern s32 gEngineApp_m_Settings_mode;

extern s32 osRecvMesg(void *queue, void *msg, s32 flags);
extern s32 osSendMesg(void *queue, void *msg, s32 flags);
extern void RumblePool_FreeFinished(void *);
extern void Controller_ReadAll(void);
extern void Controller_UpdateRumble(ControllerProfile *);
extern s32 Controller_AnyPakWatched(void);
extern void osContStartQuery(void *);
extern void osContStartReadData(void *);

void Controller_Service(void) {
    ControllerProfile *pad;
    s32 i;

    if (gControllersInitialised == 0) {
        return;
    }
    RumblePool_FreeFinished(gRumblePool);
    if (osRecvMesg(gPakAccessQueue, NULL, 0) == 0) {
        Controller_ReadAll();
        for (i = 0, pad = gControllers; i < 4; i++, pad++) {
            Controller_UpdateRumble(pad);
        }
        switch (gPakQueueState) {
        case 0:
            if (Controller_AnyPakWatched() != 0) {
                gPakQueueState = 1;
                osContStartQuery(gSiMessageQueue);
                break;
            }
            osContStartReadData(gSiMessageQueue);
            break;
        case 1:
        case 2:
        case 3:
            gPakQueueState = 0;
            osContStartReadData(gSiMessageQueue);
            break;
        }
        osSendMesg(gPakAccessQueue, NULL, 0);
    }
    if (gEngineApp_m_Settings_mode == 8) {
        if (--sControllerRescanTimer <= 0) {
            sControllerRescanTimer = 60;
            gPakQueueState = 1;
        }
    }
}
