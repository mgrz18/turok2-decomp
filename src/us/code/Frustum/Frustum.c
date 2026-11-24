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

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80295FF4_de_8.c, func_80296878_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void Plane_FromPoints(void *arg0, void *arg1, void *arg2, void *arg3) {
    extern void CVector3__Normalize(CVector3 *v);
    extern void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);
    extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
    PlaneEquation *plane = arg0;
    CVector3 *point = arg1;
    u8 sp10[12];
    u8 sp20[12];
    CVector3__Subtract((CVector3 *)sp10, arg2, arg1);
    CVector3__Subtract((CVector3 *)sp20, arg3, arg2);
    CVector3__Cross(arg0, (CVector3 *)sp20, (CVector3 *)sp10);
    CVector3__Normalize(arg0);
    plane->distance = (plane->normal.x * point->x)
                                  + (plane->normal.y * point->y)
                                  + (plane->normal.z * point->z);
}

typedef struct {
    /* 0x0 */ f32 nx;
    /* 0x4 */ f32 ny;
    /* 0x8 */ f32 nz;
    /* 0xC */ f32 d;
} PlaneComps;

/* De-faked in round 21 (j4): the draft returned through identical if/else arms; returning
 * the early case first gives the ROM's layout. */

/* Where the segment a->b crosses the plane, as a fraction of the way (1 when parallel). */
f32 Plane_SegmentFraction(PlaneComps *plane, CVector3 *a, CVector3 *b) {
    f32 da = plane->nx * a->x + plane->ny * a->y + plane->nz * a->z;
    f32 db = plane->nx * b->x + plane->ny * b->y + plane->nz * b->z;

    if (db == da) {
        return 1.0f;
    }
    return (plane->d - da) / (db - da);
}

s32 Frustum_ContainsPoint(f32 (*planes)[4], f32 *point) {
    s32 i;
    f32 *plane;
    for (i = 0; i < 6; i++) {
        plane = planes[i];
        if (!(plane[0] * point[0] + plane[1] * point[1] + plane[2] * point[2] <= plane[3])) {
            return 0;
        }
    }
    return 1;
}

s32 Frustum_TestPoints(Vector4f *planes, s32 n, CVector3 *points) {
    s32 i;
    s32 j;
    s32 inside;
    for (i = 0; i < 6; i++) {
        inside = 0;
        for (j = 0; j < n; j++) {
            if (planes[i].x * points[j].x + planes[i].y * points[j].y + planes[i].z * points[j].z <= planes[i].w) {
                inside = 1;
                break;
            }
        }
        if (!inside) {
            return 0;
        }
    }
    return 1;
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00297AD4.c, func_00297AD4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

s32 Frustum_TestPointsNoFar(B21Plane *pl, s32 n, CVector3 *pts) {
    s32 i;
    s32 j;
    s32 found;
    B21Plane *p;
    for (i = 0; i < 6; i++) {
        if (i == 2) {
            continue;
        }
        p = &pl[i];
        found = 0;
        for (j = 0; j < n; j++) {
            if (p->x * pts[j].x + p->y * pts[j].y + p->z * pts[j].z <= p->w) {
                found = 1;
                break;
            }
        }
        if (!found) {
            return 0;
        }
    }
    return 1;
}

s32 Frustum_TestBox(f32 (*arg0)[4], f32 *arg1) {
    s32 i;
    s32 inside;
    f32 *plane;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f1;
    f32 temp_f1_2;
    f32 x;
    f32 y;
    f32 z;
    f32 sum;
    for (i = 0; i < 6; i++) {
        plane = arg0[i];
        temp_f0 = plane[0];
        temp_f0_2 = arg1[3] * temp_f0;
        x = arg1[0] * temp_f0;
        if (!(x <= temp_f0_2)) {
            x = temp_f0_2;
        }
        temp_f0_3 = plane[1];
        y = arg1[1] * temp_f0_3;
        temp_f1 = arg1[4] * temp_f0_3;
        sum = x;
        if (!(y <= temp_f1)) {
            y = temp_f1;
        }
        sum += y;
        temp_f0_4 = plane[2];
        z = arg1[2] * temp_f0_4;
        temp_f1_2 = arg1[5] * temp_f0_4;
        if (!(z <= temp_f1_2)) {
            z = temp_f1_2;
        }
        sum += z;
        inside = sum <= plane[3];
        if (!inside) {
            return 0;
        }
    }
    return 1;
}
s32 Plane_TestBox(f32 *arg0, f32 *arg1);

s32 Plane_TestBox(f32 *arg0, f32 *arg1) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f1;
    f32 temp_f1_2;
    f32 x;
    f32 y;
    f32 z;
    f32 sum;
    temp_f0 = arg0[0];
    x = arg1[0] * temp_f0;
    temp_f0_2 = arg1[3] * temp_f0;
    if (!(x <= temp_f0_2)) {
        x = temp_f0_2;
    }
    temp_f0_3 = arg0[1];
    y = arg1[1] * temp_f0_3;
    temp_f1 = arg1[4] * temp_f0_3;
    sum = x;
    if (!(y <= temp_f1)) {
        y = temp_f1;
    }
    sum += y;
    temp_f0_4 = arg0[2];
    z = arg1[2] * temp_f0_4;
    temp_f1_2 = arg1[5] * temp_f0_4;
    if (!(z <= temp_f1_2)) {
        z = temp_f1_2;
    }
    sum += z;
    if (sum <= arg0[3]) {
        return 1;
    }
    return 0;
}
