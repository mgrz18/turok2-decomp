/* Boundary fixed in versions/cuts.us.txt: splat starts this function at 0x26F078 with two stray words
 * (`addiu $a0,$s1,0x6A88; jal DList_AddTail`); the real function is
 * Frustum_FromCorners (versions/undefined_funcs_auto.us.txt). */
#include "common.h"
#include "structs/vector.h"

typedef struct {
    /* 0x0 */ CVector3 n;
    /* 0xC */ f32 d;
} Plane;

extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a - b */
extern void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a x b */
extern void CVector3__Normalize(CVector3 *v);                         /* normalise */

/* The plane through a, b and c, facing by the winding a -> b -> c. */
static __inline__ void make_plane(Plane *p, CVector3 *a, CVector3 *b, CVector3 *c) {
    CVector3 e0;
    CVector3 e1;

    CVector3__Subtract(&e0, b, a);
    CVector3__Subtract(&e1, c, b);
    CVector3__Cross(&p->n, &e1, &e0);
    CVector3__Normalize(&p->n);
    p->d = p->n.x * a->x + p->n.y * a->y + p->n.z * a->z;
}

/* The six planes of a view volume from the eye and the four corners of its far rectangle. */
void Frustum_FromCorners(Plane *planes, CVector3 *eye, CVector3 *c0, CVector3 *c1, CVector3 *c2, CVector3 *c3) {
    make_plane(&planes[2], c0, c1, c3);
    make_plane(&planes[0], eye, c0, c2);
    make_plane(&planes[1], eye, c3, c1);
    make_plane(&planes[4], eye, c1, c0);
    make_plane(&planes[3], eye, c2, c3);
    planes[5].n.x = -planes[2].n.x;
    planes[5].n.y = -planes[2].n.y;
    planes[5].n.z = -planes[2].n.z;
    planes[5].d = planes[5].n.x * eye->x + planes[5].n.y * eye->y + planes[5].n.z * eye->z;
}
