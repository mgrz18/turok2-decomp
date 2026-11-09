#include "common.h"
#include "structs/particle.h"
#include "structs/swoosh.h"

/* The swoosh record: the time it fades out in. */
typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ f32 fadeTime;
} SwooshDef;

/* Detaches every swoosh of owner arg1 (flags & 1: counted in a game object's m_ActiveSwooshes,
 * & 2: in a particle's), letting each fade out. */
void Swoosh_DetachOwner(CSwooshSystem *arg0, void *arg1) {
    f32 temp_f1;
    u8 temp_v0;
    u8 temp_v0_2;
    void *temp_a0;
    void *temp_a0_2;
    Swoosh *var_v1;

    var_v1 = arg0->m_SwooshPool.used.head;
    if (var_v1 != NULL) {
        do {
            temp_a0 = var_v1->owner;
            if (temp_a0 == arg1) {
                if (temp_a0 != NULL) {
                    if (var_v1->flags & 1) {
                        temp_v0 = ((CGameObjectInstance *) temp_a0)->m_ActiveSwooshes;
                        if (temp_v0 != 0) {
                            ((CGameObjectInstance *) temp_a0)->m_ActiveSwooshes = (u8) (temp_v0 - 1);
                        }
                    }
                    if (var_v1->flags & 2) {
                        temp_a0_2 = var_v1->owner;
                        temp_v0_2 = ((CParticle *) temp_a0_2)->m_ActiveSwooshes;
                        if (temp_v0_2 != 0) {
                            ((CParticle *) temp_a0_2)->m_ActiveSwooshes = (u8) (temp_v0_2 - 1);
                        }
                    }
                }
                var_v1->owner = NULL;
                temp_f1 = ((SwooshDef *) var_v1->info)->fadeTime;
                if (var_v1->time < temp_f1) {
                    var_v1->time = temp_f1;
                }
            }
            var_v1 = var_v1->next;
        } while (var_v1 != NULL);
    }
}
