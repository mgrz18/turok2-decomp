#include "common.h"
#include "structs/vector.h"

typedef struct Plane {
    u8 pad0[0x30];
    CVector3 normal;        /* 0x30 */
} Plane;

extern void func_0020EF2C(CVector3 *out, CVector3 *a, Plane *b);

s32 func_0023C2B0(Plane *plane, CVector3 *a, CVector3 *b) {
    CVector3 d;
    s32 sideA;
    s32 sideB;

    func_0020EF2C(&d, a, plane);
    sideA = (plane->normal.x * d.x + plane->normal.y * d.y + plane->normal.z * d.z >= 0.0f) ? 0 : 1;
    func_0020EF2C(&d, b, plane);
    sideB = (plane->normal.x * d.x + plane->normal.y * d.y + plane->normal.z * d.z >= 0.0f) ? 0 : 1;
    return sideA ^ sideB;
}
