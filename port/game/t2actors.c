/* Animated actors on the host: switches (docs/port/RENDER.md, actors and particles).
 *
 * T2Port_AnimEventsEnabled: the animation events (CGameObjectInstance__FireAnimEvents)
 * and hot points (GameObject_EmitHotPoints): footsteps, splashes, muzzle flashes, weapon
 * fire. They end in CParticleSystem__Spawn, which runs on host types since round 7 (v1,
 * RENDER.md section 6), so they are ON by default; T2_ANIM_EVENTS=0 turns them off
 * (to compare with the round-6 behaviour). */
#include <stdlib.h>

int T2Port_AnimEventsEnabled(void) {
    static int sState = -1;

    if (sState < 0) {
        const char *e = getenv("T2_ANIM_EVENTS");
        sState = (e != NULL && e[0] == 0) ? 0 : 1;
    }
    return sState;
}
