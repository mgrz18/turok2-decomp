#include "common.h"
#include "structs/vector.h"

typedef struct {
    /* 0x0 */ f32 nx;
    /* 0x4 */ f32 ny;
    /* 0x8 */ f32 nz;
    /* 0xC */ f32 d;
} Plane;

/* De-faked in round 21 (j4): the draft returned through identical if/else arms; returning
 * the early case first gives the ROM's layout. */

/* Where the segment a->b crosses the plane, as a fraction of the way (1 when parallel). */
f32 Plane_SegmentFraction(Plane *plane, CVector3 *a, CVector3 *b) {
    f32 da = plane->nx * a->x + plane->ny * a->y + plane->nz * a->z;
    f32 db = plane->nx * b->x + plane->ny * b->y + plane->nz * b->z;

    if (db == da) {
        return 1.0f;
    }
    return (plane->d - da) / (db - da);
}
