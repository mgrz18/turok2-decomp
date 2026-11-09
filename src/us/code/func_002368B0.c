#include "common.h"
#include "structs/particle.h"
#include "structs/swoosh.h"

/* The swoosh record: the time it fades out in. */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ f32 y;
} SwooshPoint;

/* Detaches swoosh arg0 from its owner (flags & 1: counted in a game object's m_ActiveSwooshes,
 * & 2: in a particle's) and lets it fade out. */
void func_002368B0(Swoosh *arg0) {
    f32 temp_f1;
    u8 temp_v0;
    u8 temp_v0_2;
    void *temp_v1;
    void *temp_v1_2;

    temp_v1 = arg0->owner;
    if (temp_v1 != NULL) {
        if (arg0->flags & 1) {
            temp_v0 = ((CGameObjectInstance *) temp_v1)->m_ActiveSwooshes;
            if (temp_v0 != 0) {
                ((CGameObjectInstance *) temp_v1)->m_ActiveSwooshes = temp_v0 - 1;
            }
        }
        if (arg0->flags & 2) {
            temp_v1_2 = arg0->owner;
            temp_v0_2 = ((CParticle *) temp_v1_2)->m_ActiveSwooshes;
            if (temp_v0_2 != 0) {
                ((CParticle *) temp_v1_2)->m_ActiveSwooshes = temp_v0_2 - 1;
            }
        }
    }
    arg0->owner = NULL;
    temp_f1 = ((SwooshPoint *) arg0->info)->y;
    if (arg0->time < temp_f1) {
        arg0->time = temp_f1;
    }
}
