/* Animated actors on the host: switches (docs/port/RENDER.md, actors).
 *
 * T2Port_AnimEventsEnabled: the animation events (CGameObjectInstance__FireAnimEvents)
 * and hot points (GameObject_EmitHotPoints) end in CParticleSystem__Spawn
 * (func_0022D2FC.c), whose views are still at N64 offsets and corrupt the heap/stack on
 * the host. They stay off until it is ported; T2_ANIM_EVENTS=1 turns them on to work
 * on that port. */
#include <stdlib.h>

int T2Port_AnimEventsEnabled(void) {
    static int sState = -1;

    if (sState < 0) {
        const char *e = getenv("T2_ANIM_EVENTS");
        sState = (e != NULL && e[0] == '1') ? 1 : 0;
    }
    return sState;
}
