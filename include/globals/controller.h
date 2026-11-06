#ifndef GLOBALS_CONTROLLER_H
#define GLOBALS_CONTROLLER_H

/* The serial interface queue and the controller driver's statics
 * (gControllers, gMergedController, gContStatus are structs/controller.h's). */

#include "common.h"
#include "ultra_types.h"

extern OSMesgQueue gSiMessageQueue;  /* the serial interface events */
extern OSMesg gSiMesgBuf;            /* its one-message buffer */
extern s16 gFirstControllerPort;     /* the lowest port with a controller, -1 none */

/* gMergedController.buttonsSingle (structs/controller.h), kept where the code reaches it by its own symbol. */
extern u32 gMergedController_buttonsSingle;

/* The controllers' state, 0x800B6CF0-0x800B6CF8. */

extern u8 gControllersInitialised;  /* Cont_InitAll has run */
extern u8 gValidControllerBits;     /* osContInit's bit pattern ("ValidControllers") */
extern u8 gNumControllers;          /* controllers Cont_InitAll found */
extern s32 sControllerRescanTimer;  /* Controller_Service counts it down from 60, then rescans */
extern s32 gMenuSoundsEnabled;      /* the Controller_Menu* functions play the menu sounds (1) */
extern OSContPad gContPadData[4];     /* osContGetReadData's buffer */
extern u32 gControllers_buttons;     /* gControllers[0].buttons */

#endif
