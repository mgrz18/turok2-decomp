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

/* The 8 bytes an impact event passes its handler by value after the position
 * (ImpactEvent_Call, ImpactEvent_Send): each handler reads them its own way.
 * As two words (the doors' and actions' events pass {0, ...}): */
typedef struct CImpactEventArg {
    /* 0x0 */ s32 a;
    /* 0x4 */ s32 b;
} CImpactEventArg; /* size 0x8 */

/* As a blast (ImpactEvent_ApplyBlast and the explosions, Blast_GetPush): the radius, the
 * damage, then the push (strength, x, y, z). */
#ifdef TARGET_PORT
/* The host (round 10 y3, docs/port/FIDELITY.md 5): an impact event's 8-byte value arrives as two
 * host-order words (the event values' convention: CEventValue in structs/particle.h, the
 * animation events' s32[2] from the archive), word 0 the N64's first big-endian word. On the
 * little-endian hosts the halves and bytes of each word sit in reverse order, so the fields
 * that read them by position are declared reversed. (With the N64 order, every particle hit
 * swapped radius and damage: demo 1's first enemy shot took 1 health instead of 8.) */
typedef struct CImpactBlast {
    u16 damage;
    u16 radius;
    s8 z;
    s8 y;
    s8 x;
    s8 strength;
} __attribute__((aligned(4))) CImpactBlast;
#else
typedef struct CImpactBlast {
    /* 0x0 */ u16 radius;
    /* 0x2 */ u16 damage;
    /* 0x4 */ s8 strength;
    /* 0x5 */ s8 x;
    /* 0x6 */ s8 y;
    /* 0x7 */ s8 z;
} __attribute__((aligned(4))) CImpactBlast; /* size 0x8 */
#endif

/* As two unsigned words the handler takes halves and bytes from (the camera shakes and
 * flashes). On the host (round 7 v2) the value reaches the handler as two host-order
 * words (the archive swaps the animation events' values as two u32: struct IntPair),
 * word 0 the N64's first big-endian word. */
typedef struct CImpactEventWords {
    /* 0x0 */ u32 w0;
    /* 0x4 */ u32 w1;
} CImpactEventWords; /* size 0x8 */

#endif
