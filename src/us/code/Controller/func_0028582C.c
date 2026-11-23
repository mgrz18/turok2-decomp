#include "common.h"
#include "structs/controller.h"

/* Once a frame: copies what the reads gathered (the input* fields) into
 * the fields the game reads, and clears the gathered presses. */
void Controller_Latch(ControllerProfile *arg0) {
    arg0->stickX = arg0->inputStickX;
    arg0->stickY = arg0->inputStickY;
    arg0->buttons = arg0->inputButtons;
    arg0->buttonsSingle = arg0->inputButtonsSingle;
    arg0->buttonsSingleNoHold = arg0->inputButtonsSingleNoHold;
    arg0->buttonsDouble = arg0->inputButtonsDouble;
    arg0->buttonsRepeat = arg0->inputButtonsRepeat;
    arg0->inputButtonsSingle = 0;
    arg0->inputButtonsSingleNoHold = 0;
    arg0->inputButtonsDouble = 0;
    arg0->inputButtonsRepeat = 0;
}
