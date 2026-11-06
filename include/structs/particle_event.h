#ifndef STRUCTS_PARTICLE_EVENT_H
#define STRUCTS_PARTICLE_EVENT_H

/* The particle event table: gImpactEventHandlers holds, per event kind, a handler and
 * its flags (8 bytes a kind; ImpactEvent_Send / ImpactEvent_Call call the
 * handlers, whose last arguments each file types its own way). The code
 * reads the flags through the symbol of the first ones, gImpactEventHandlers_m_Flags, so
 * here they are seen from there: a kind's flags, then the next kind's
 * handler. Flag 2: the particle is the event's source (not what it hit);
 * 4: the event is scaled by the particle (CParticle__HitTarget) or needs the
 * object's model (CGameObjectInstance__FireAnimEvents); 1: the impact ends the particle.
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"

typedef struct CEventFlags {
    /* 0x0 */ u32 m_Flags;
    /* 0x4 */ void *_pNextHandler;          /* the next kind's handler (gImpactEventHandlers) */
} CEventFlags; /* size 0x8 */

extern CEventFlags gImpactEventHandlers_m_Flags[];        /* = &gImpactEventHandlers[0].flags */

#endif
