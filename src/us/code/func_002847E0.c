/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00263760.c, func_00263760.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/controller.h"
#include "globals/pak.h"
#include "globals/controller.h"
void osContGetReadData(OSContPad *pad);
s32 osContReset(OSMesgQueue *mq, OSContStatus *data);
void osContGetQuery(OSContStatus *data);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
typedef struct M04Pad {
    u16 button;
    s8 x;
    s8 y;
    u8 err;
    u8 pad;
} M04Pad;
extern M04Pad gContPadData[];
void Controller_UpdateConnection(ControllerProfile *c);
void Controller_ProcessInput(ControllerProfile *c, s8 x, s8 y, u16 b);
void Controller_ReadAll(void) {
    s32 i;
    switch ((u8)gPakQueueState) {
    case 0:
        osRecvMesg(&gSiMessageQueue, &gSiMesgBuf, 1);
        osContGetReadData((OSContPad *)gContPadData);
        gMergedController.port = -1;
        gMergedController.connected = 0;
        gMergedController.inputStickX = 0;
        gMergedController.inputStickY = 0;
        gMergedController.stickX = 0;
        gMergedController.stickY = 0;
        gMergedController.buttons = 0;
        gMergedController.previousButtons = 0;
        gMergedController.buttonsSingle = 0;
        gMergedController.buttonsSingleNoHold = 0;
        gMergedController.buttonsDouble = 0;
        gMergedController.buttonsRepeat = 0;
        gMergedController.inputButtons = 0;
        gMergedController.inputPreviousButtons = 0;
        gMergedController.inputButtonsSingle = 0;
        gMergedController.inputButtonsSingleNoHold = 0;
        gMergedController.inputButtonsDouble = 0;
        gMergedController.inputButtonsRepeat = 0;
        for (i = 0; i < 4; i++) {
            Controller_ProcessInput(&gControllers[i], gContPadData[i].x, gContPadData[i].y, gContPadData[i].button);
            gMergedController.connected |= gControllers[i].connected;
            gMergedController.inputStickX += gControllers[i].inputStickX;
            gMergedController.inputStickY += gControllers[i].inputStickY;
            gMergedController.stickX += gControllers[i].stickX;
            gMergedController.stickY += gControllers[i].stickY;
            gMergedController.buttons |= gControllers[i].buttons;
            gMergedController.buttonsSingle |= gControllers[i].buttonsSingle;
            gMergedController.buttonsSingleNoHold |= gControllers[i].buttonsSingleNoHold;
            gMergedController.buttonsDouble |= gControllers[i].buttonsDouble;
            gMergedController.buttonsRepeat |= gControllers[i].buttonsRepeat;
            gMergedController.inputButtons |= gControllers[i].inputButtons;
            gMergedController.inputPreviousButtons |= gControllers[i].inputPreviousButtons;
            gMergedController.inputButtonsSingle |= gControllers[i].inputButtonsSingle;
            gMergedController.inputButtonsSingleNoHold |= gControllers[i].inputButtonsSingleNoHold;
            gMergedController.inputButtonsDouble |= gControllers[i].inputButtonsDouble;
            gMergedController.inputButtonsRepeat |= gControllers[i].inputButtonsRepeat;
        }
        gMergedController.inputStickX = (s8)((f32)(s8)gMergedController.inputStickX * 0.25f);
        gMergedController.inputStickY = (s8)((f32)(s8)gMergedController.inputStickY * 0.25f);
        gMergedController.stickX = (s8)((f32)(s8)gMergedController.stickX * 0.25f);
        gMergedController.stickY = (s8)((f32)(s8)gMergedController.stickY * 0.25f);
        break;
    case 1:
        osRecvMesg(&gSiMessageQueue, &gSiMesgBuf, 1);
        osContGetQuery(gContStatus);
        for (i = 0; i < 4; i++) {
            Controller_UpdateConnection(&gControllers[i]);
        }
        break;
    case 3:
        osContReset(&gSiMessageQueue, gContStatus);
    case 2:
        break;
    }
}
