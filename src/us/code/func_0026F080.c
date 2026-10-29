/* Boundary fixed in versions/cuts.us.txt: splat starts this function at 0x26F078 with two stray words
 * (`addiu $a0,$s1,0x6A88; jal func_0026D288`); the real function is
 * func_0026F080 (versions/undefined_funcs_auto.us.txt). */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x0 */ Vec3f n;
    /* 0xC */ f32 d;
} Plane;

extern void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a - b */
extern void func_0020EFDC(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a x b */
extern void func_0020F040(Vec3f *v);                         /* normalise */

/* The plane through a, b and c, facing by the winding a -> b -> c. */
static __inline__ void make_plane(Plane *p, Vec3f *a, Vec3f *b, Vec3f *c) {
    Vec3f e0;
    Vec3f e1;

    func_0020EF2C(&e0, b, a);
    func_0020EF2C(&e1, c, b);
    func_0020EFDC(&p->n, &e1, &e0);
    func_0020F040(&p->n);
    p->d = p->n.x * a->x + p->n.y * a->y + p->n.z * a->z;
}

/* The six planes of a view volume from the eye and the four corners of its far rectangle. */
void func_0026F080(Plane *planes, Vec3f *eye, Vec3f *c0, Vec3f *c1, Vec3f *c2, Vec3f *c3) {
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
