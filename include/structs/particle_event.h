#ifndef STRUCTS_PARTICLE_EVENT_H
#define STRUCTS_PARTICLE_EVENT_H

/* The particle event table: gImpactEventHandlers holds, per event kind, a handler and
 * its flags (8 bytes a kind; ImpactEvent_Send / ImpactEvent_Call call the
 * handlers, whose last arguments each file types its own way). The ROM reaches
 * the flags through the symbol gImpactEventHandlers_m_Flags (= &gImpactEventHandlers[0].flags);
 * the C spells it gImpactEventHandlers[kind].flags (same code). Flag 2: the particle is the event's source (not what it hit);
 * 4: the event is scaled by the particle (CParticle__HitTarget) or needs the
 * object's model (CGameObjectInstance__FireAnimEvents); 1: the impact ends the particle.
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"

/* A row of gImpactEventHandlers (globals/particle.h): the handler, whose
 * arguments each caller types its own way (it casts), and the kind's flags. */
typedef struct CImpactEventHandler {
    /* 0x0 */ void (*handler)(void);
    /* 0x4 */ u32 flags;
} CImpactEventHandler; /* size 0x8 */

#endif
