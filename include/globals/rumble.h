#ifndef GLOBALS_RUMBLE_H
#define GLOBALS_RUMBLE_H

/* The world's rumble effects, 0x800F5A50-0x800F5C2F: the pool and its 8
 * nodes (RumblePool_Init(&gRumblePool, gRumbleEntries, 8)). */

#include "common.h"
#include "structs/rumble.h"

extern RumblePool gRumblePool;
extern RumbleEffect gRumbleEntries[8];

#endif
