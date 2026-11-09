#include "common.h"
#include "structs/particle.h"
#include "structs/swoosh.h"

/* The swoosh record: the time it fades out in. */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ f32 y;
} SwooshPoint;

s32 Swoosh_Update(void *, void *);                  /* extern */
s32 NodePool_Free(void *, void *);              /* extern */

/* Updates every live swoosh; frees those that ended (detached from their owner first:
 * flags & 1 counted in a game object's m_ActiveSwooshes, & 2 in a particle's). */
void SwooshSystem_UpdateSwooshes(CSwooshSystem *arg0) {
    f32 temp_f1;
    u8 temp_v0;
    u8 temp_v0_2;
    Swoosh *temp_s1;
    void *temp_v1;
    void *temp_v1_2;
    Swoosh *var_s0;

    var_s0 = arg0->m_SwooshPool.used.head;
    if (var_s0 != NULL) {
        do {
            temp_s1 = var_s0->next;
            if (Swoosh_Update(arg0, var_s0) == 0) {
                temp_v1 = var_s0->owner;
                if (temp_v1 != NULL) {
                    if (var_s0->flags & 1) {
                        temp_v0 = ((CGameObjectInstance *) temp_v1)->m_ActiveSwooshes;
                        if (temp_v0 != 0) {
                            ((CGameObjectInstance *) temp_v1)->m_ActiveSwooshes = temp_v0 - 1;
                        }
                    }
                    if (var_s0->flags & 2) {
                        temp_v1_2 = var_s0->owner;
                        temp_v0_2 = ((CParticle *) temp_v1_2)->m_ActiveSwooshes;
                        if (temp_v0_2 != 0) {
                            ((CParticle *) temp_v1_2)->m_ActiveSwooshes = temp_v0_2 - 1;
                        }
                    }
                }
                var_s0->owner = NULL;
                temp_f1 = ((SwooshPoint *) var_s0->info)->y;
                if (var_s0->time < temp_f1) {
                    var_s0->time = temp_f1;
                }
                NodePool_Free(&arg0->m_SwooshPool, var_s0);
            }
            var_s0 = temp_s1;
        } while (var_s0 != NULL);
    }
}
