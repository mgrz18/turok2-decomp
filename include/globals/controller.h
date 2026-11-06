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

#endif
