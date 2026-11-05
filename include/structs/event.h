#ifndef STRUCTS_EVENT_H
#define STRUCTS_EVENT_H

/* The particle event table: D_800AD080 holds, per event kind, a handler and
 * its flags (8 bytes a kind; func_00236EA0 / func_00237EE4 call the
 * handlers, whose last arguments each file types its own way). The code
 * reads the flags through the symbol of the first ones, D_800AD084, so
 * here they are seen from there: a kind's flags, then the next kind's
 * handler. Flag 2: the particle is the event's source (not what it hit);
 * 4: the event is scaled by the particle (func_0022C488) or needs the
 * object's model (func_0021ABC8); 1: the impact ends the particle.
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"

typedef struct CEventFlags {
    /* 0x0 */ u32 m_Flags;
    /* 0x4 */ void *_pNextHandler;          /* the next kind's handler (D_800AD080) */
} CEventFlags; /* size 0x8 */

extern CEventFlags D_800AD084[];        /* = &D_800AD080[0].flags */

#endif
