/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00263760.c, func_00263760.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/controller.h"
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
extern f32 D_800A9CE0;
extern M04Pad D_800F5C60[];
extern u8 D_800F6750;
extern OSMesgQueue D_800F6798;
extern OSMesg D_800F6770;
void Controller_UpdateConnection(ControllerProfile *c);
void Controller_ProcessInput(ControllerProfile *c, s8 x, s8 y, u16 b);
void Controller_ReadAll(void) {
    s32 i;
    switch (D_800F6750) {
    case 0:
        osRecvMesg(&D_800F6798, &D_800F6770, 1);
        osContGetReadData((OSContPad *)D_800F5C60);
        D_800F5C78.port = -1;
        D_800F5C78.connected = 0;
        D_800F5C78.inputStickX = 0;
        D_800F5C78.inputStickY = 0;
        D_800F5C78.stickX = 0;
        D_800F5C78.stickY = 0;
        D_800F5C78.buttons = 0;
        D_800F5C78.previousButtons = 0;
        D_800F5C78.buttonsSingle = 0;
        D_800F5C78.buttonsSingleNoHold = 0;
        D_800F5C78.buttonsDouble = 0;
        D_800F5C78.buttonsRepeat = 0;
        D_800F5C78.inputButtons = 0;
        D_800F5C78.inputPreviousButtons = 0;
        D_800F5C78.inputButtonsSingle = 0;
        D_800F5C78.inputButtonsSingleNoHold = 0;
        D_800F5C78.inputButtonsDouble = 0;
        D_800F5C78.inputButtonsRepeat = 0;
        for (i = 0; i < 4; i++) {
            Controller_ProcessInput(&D_800F5EC0[i], D_800F5C60[i].x, D_800F5C60[i].y, D_800F5C60[i].button);
            D_800F5C78.connected |= D_800F5EC0[i].connected;
            D_800F5C78.inputStickX += D_800F5EC0[i].inputStickX;
            D_800F5C78.inputStickY += D_800F5EC0[i].inputStickY;
            D_800F5C78.stickX += D_800F5EC0[i].stickX;
            D_800F5C78.stickY += D_800F5EC0[i].stickY;
            D_800F5C78.buttons |= D_800F5EC0[i].buttons;
            D_800F5C78.buttonsSingle |= D_800F5EC0[i].buttonsSingle;
            D_800F5C78.buttonsSingleNoHold |= D_800F5EC0[i].buttonsSingleNoHold;
            D_800F5C78.buttonsDouble |= D_800F5EC0[i].buttonsDouble;
            D_800F5C78.buttonsRepeat |= D_800F5EC0[i].buttonsRepeat;
            D_800F5C78.inputButtons |= D_800F5EC0[i].inputButtons;
            D_800F5C78.inputPreviousButtons |= D_800F5EC0[i].inputPreviousButtons;
            D_800F5C78.inputButtonsSingle |= D_800F5EC0[i].inputButtonsSingle;
            D_800F5C78.inputButtonsSingleNoHold |= D_800F5EC0[i].inputButtonsSingleNoHold;
            D_800F5C78.inputButtonsDouble |= D_800F5EC0[i].inputButtonsDouble;
            D_800F5C78.inputButtonsRepeat |= D_800F5EC0[i].inputButtonsRepeat;
        }
        D_800F5C78.inputStickX = (s8)((f32)(s8)D_800F5C78.inputStickX * D_800A9CE0);
        D_800F5C78.inputStickY = (s8)((f32)(s8)D_800F5C78.inputStickY * D_800A9CE0);
        D_800F5C78.stickX = (s8)((f32)(s8)D_800F5C78.stickX * D_800A9CE0);
        D_800F5C78.stickY = (s8)((f32)(s8)D_800F5C78.stickY * D_800A9CE0);
        break;
    case 1:
        osRecvMesg(&D_800F6798, &D_800F6770, 1);
        osContGetQuery(D_800F6778);
        for (i = 0; i < 4; i++) {
            Controller_UpdateConnection(&D_800F5EC0[i]);
        }
        break;
    case 3:
        osContReset(&D_800F6798, D_800F6778);
    case 2:
        break;
    }
}
