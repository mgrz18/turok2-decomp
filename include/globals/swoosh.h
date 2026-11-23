#ifndef GLOBALS_SWOOSH_H
#define GLOBALS_SWOOSH_H

/* The swoosh/trail and decal system (CEngineApp + 0x19608, right after COnScreen):
 * gEngineApp.m_SwooshSystem on the host (structs/game.h); the N64 CEngineApp has no
 * member there, so the code reaches it by this symbol (versions/names/data_g6.us.txt). */

#include "structs/swoosh.h"

extern CSwooshSystem gEngineApp_m_SwooshSystem;

#endif
