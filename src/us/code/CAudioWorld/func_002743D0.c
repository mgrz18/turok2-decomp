#include "common.h"
#include "structs/vector.h"
#include "globals/audio.h"

/* De-faked in round 21 (j4): the draft returned through identical if/else arms; returning
 * the early case first gives the ROM's layout. */

/* Distance falloff: 1 - d^2 / sSoundHearingRangeSq inside, 0 outside. */
f32 Sound_DistanceFalloff(CVector3 *a, CVector3 *b) {
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    f32 dz = a->z - b->z;
    f32 d = dx * dx + dy * dy + dz * dz;

    if (sSoundHearingRangeSq <= d) {
        return 0.0f;
    }
    return 1.0f - d / sSoundHearingRangeSq;
}
