#ifndef GLOBALS_CONTROLLER_H
#define GLOBALS_CONTROLLER_H

/* The controllers' state, 0x800B6CF0-0x800B6CF8. */

#include "common.h"

extern u8 gControllersInitialised;  /* Cont_InitAll has run */
extern u8 gValidControllerBits;     /* osContInit's bit pattern ("ValidControllers") */
extern u8 gNumControllers;          /* controllers Cont_InitAll found */
extern s32 sControllerRescanTimer;  /* Controller_Service counts it down from 60, then rescans */
extern s32 gMenuSoundsEnabled;      /* the Controller_Menu* functions play the menu sounds (1) */

#endif
