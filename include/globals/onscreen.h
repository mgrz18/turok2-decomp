#ifndef GLOBALS_ONSCREEN_H
#define GLOBALS_ONSCREEN_H

/* The engine's on-screen text and boss bars, gEngineApp + 0x19440 (COnScreen,
 * structs/player.h; CEngineApp still has bytes there). */

#include "structs/player.h"

extern COnScreen gEngineApp_m_OnScreen;

/* Fields with symbols of their own */
extern f32 gEngineApp_m_OnScreen_m_FontXScale;   /* .m_FontXScale */
extern s32 gEngineApp_m_OnScreen_m_ShadowXOff[2]; /* .m_ShadowXOff, .m_ShadowYOff: every reader takes the pair as an array (the array view is what keeps the ROM's code) */
extern s32 gEngineApp_m_OnScreen_m_TopR;         /* .m_TopR */

#endif
