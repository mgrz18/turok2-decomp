#include "common.h"
#include "structs/collision.h"

/* Writes a sweep's hit back where the test that took it asked: the object's
 * position and region, the kind and the data (see SweepHit). */
void SweepHit_Store(SweepHit *out, CInstanceHeader *src) {
    if (out->posOut != NULL) {
        *out->posOut = src->m_vPos;
    }
    if (out->regionOut != NULL) {
        *out->regionOut = src->m_pCurrentRegion;
    }
    if (out->kindOut != NULL) {
        *out->kindOut = out->kind;
    }
    if (out->dataOut != NULL) {
        *out->dataOut = out->data;
    }
}
