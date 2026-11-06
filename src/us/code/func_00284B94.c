#include "common.h"
#include "structs/controller.h"

/* Controller pak / rumble service, run once per frame while enabled. */

extern u8 D_800B6CF0;           /* service enabled */
extern s32 D_800B6CF4;          /* rescan countdown */
extern u8 D_800F5A50[];
extern u8 D_800F6750;           /* rescan request */
extern u8 D_800F6758[];        /* OSMesgQueue: pak access lock */
extern u8 D_800F6798[];
extern s32 D_8011AC88;

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

    if (D_800B6CF0 == 0) {
        return;
    }
    RumblePool_FreeFinished(D_800F5A50);
    if (osRecvMesg(D_800F6758, NULL, 0) == 0) {
        Controller_ReadAll();
        for (i = 0, pad = D_800F5EC0; i < 4; i++, pad++) {
            Controller_UpdateRumble(pad);
        }
        switch (D_800F6750) {
        case 0:
            if (Controller_AnyPakWatched() != 0) {
                D_800F6750 = 1;
                osContStartQuery(D_800F6798);
                break;
            }
            osContStartReadData(D_800F6798);
            break;
        case 1:
        case 2:
        case 3:
            D_800F6750 = 0;
            osContStartReadData(D_800F6798);
            break;
        }
        osSendMesg(D_800F6758, NULL, 0);
    }
    if (D_8011AC88 == 8) {
        if (--D_800B6CF4 <= 0) {
            D_800B6CF4 = 60;
            D_800F6750 = 1;
        }
    }
}
