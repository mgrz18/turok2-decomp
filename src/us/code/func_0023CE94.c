#include "common.h"
#include "structs/vector.h"

typedef struct Sphere {
    CVector3 center;     /* 0x00 */
    f32 radius;      /* 0x0C */
    f32 radiusSq;    /* 0x10 */
} Sphere;

void Sphere_Set(Sphere *sphere, CVector3 *center, f32 radius) {
    sphere->center = *center;
    sphere->radius = radius;
    sphere->radiusSq = radius * radius;
}
