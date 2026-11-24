#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"
#include "globals/collision.h"


void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);           /* out = a - b */
void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);           /* out = a + b */
void CVector3__Scale(CVector3 *out, CVector3 *v, f32 k);                 /* out = v * k */
void CVector3__Lerp(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);    /* out = a + (b - a) * t */
void CVector3__Bounce(CVector3 *out, CVector3 *v, CVector3 *n, f32 bounce); /* bounce off n */
void CVector3__RemoveComponent(CVector3 *out, CVector3 *v, CVector3 *n);           /* slide along n */
void CVector3__ProjectKeepLength(CVector3 *out, CVector3 *v, CVector3 *n);
f32 func_00298470(f32);                                                /* sqrtf */

#define DOT(a, b) ((a)->x * (b)->x + (a)->y * (b)->y + (a)->z * (b)->z)

/* Tests the sweep against the plane through point with this normal; on a
 * nearer hit records it and answers it the way kind says (1 stop, 2 slide,
 * 3 bounce, 4 and 5 other slides, 6 slide or bounce, 7 pass through).
 * Returns 1 when the hit was taken. */
s32 Sweep_HitPlane(Sweep *sweep, CVector3 *normal, CVector3 *point, s32 frontOnly, s32 kind, f32 friction,
                  s32 allowBehind) {
    CVector3 toPoint;
    CVector3 dir;
    CVector3 rest;
    CVector3 bounced;
    CVector3 move;
    CVector3 a;
    CVector3 b;
    CVector3 push;
    CVector3 vel;
    f32 denom;
    f32 t;
    f32 len;
    f32 xx;
    f32 zz;

    if (kind == 0) {
        return 0;
    }
    if (sweep->lastKind == 1 && sweep->lastContact.x == normal->x && sweep->lastContact.y == normal->y
        && sweep->lastContact.z == normal->z) {
        return 0;
    }
    denom = DOT(normal, &sweep->delta);
    if (denom == 0.0f) {
        return 0;
    }
    if (frontOnly && denom > 0.0f) {
        return 0;
    }
    CVector3__Subtract(&toPoint, point, &sweep->inst->m_vPos);
    t = DOT(normal, &toPoint) / denom;
    if (sweep->hit.fraction <= t) {
        return 0;
    }
    if (t < 0.0f && !allowBehind) {
        return 0;
    }
    if (t < -0.2f) {
        t = -0.2f;
    }
    gCollisionResultsP->m_vNormal = *normal;
    sweep->hit.fraction = t;
    CVector3__Lerp(&sweep->hit.pos, t, &sweep->inst->m_vPos, &sweep->end);
    if (kind == 6) {
        kind = 3;
        if (sweep->onGround) {
            kind = 2;
        }
    }
    if (kind == 3 && sweep->def->m_BounceReturnEnergy < 1.0f) {
        xx = sweep->delta.x * sweep->delta.x;
        zz = sweep->delta.z * sweep->delta.z;
        if (xx + sweep->delta.y * sweep->delta.y + zz < 16.777218f) {
            kind = 2;
            if (xx + zz < 1.0485761f) {
                kind = 1;
            }
        }
    }
    switch (kind) {
        case 7:
            sweep->kind = 0;
            sweep->hit.type = 1;
            if (sweep->physics) {
                sweep->hit.vel = sweep->obj->m_vVelocity;
            }
            sweep->hit.slideTo = sweep->end;
            return 1;
        case 1:
            sweep->kind = 0;
            sweep->hit.type = 2;
            if (sweep->physics) {
                sweep->hit.vel.x = sweep->hit.vel.y = sweep->hit.vel.z = 0.0f;
            }
            break;
        case 3:
            sweep->kind = 0;
            if (sweep->physics || sweep->more) {
                len = func_00298470(normal->x * normal->x + normal->y * normal->y + normal->z * normal->z);
                if (len == 0.0f) {
                    return 0;
                }
                CVector3__Scale(&dir, normal, 1.0f / len);
            }
            if (sweep->physics) {
                CVector3__Bounce(&vel, &sweep->obj->m_vVelocity, &dir, sweep->def->m_BounceReturnEnergy);
                sweep->hit.vel = vel;
                CVector3__Scale(&sweep->hit.vel, &sweep->hit.vel, 1.0f - friction);
            }
            if (sweep->more) {
                sweep->hit.type = 3;
                CVector3__Scale(&rest, &sweep->delta, 1.0f - t);
                CVector3__Bounce(&bounced, &rest, &dir, sweep->def->m_BounceReturnEnergy);
                CVector3__Scale(&bounced, &bounced, 1.0f - friction);
                CVector3__Add(&sweep->hit.slideTo, &sweep->hit.pos, &bounced);
            } else {
                sweep->hit.type = 2;
            }
            break;
        case 2:
            sweep->kind = 1;
            sweep->contact = *normal;
            if (sweep->physics || sweep->more) {
                len = func_00298470(normal->x * normal->x + normal->y * normal->y + normal->z * normal->z);
                if (len == 0.0f) {
                    return 0;
                }
                CVector3__Scale(&dir, normal, 1.0f / len);
            }
            if (sweep->physics) {
                CVector3__RemoveComponent(&vel, &sweep->obj->m_vVelocity, &dir);
                sweep->hit.vel = vel;
                CVector3__Scale(&sweep->hit.vel, &sweep->hit.vel, 1.0f - friction);
            }
            if (sweep->more) {
                sweep->hit.type = 3;
                CVector3__Scale(&rest, &sweep->delta, 1.0f - t);
                CVector3__RemoveComponent(&move, &rest, &dir);
                CVector3__Scale(&move, &move, 1.0f - friction);
                CVector3__Add(&sweep->hit.slideTo, &sweep->hit.pos, &move);
                CVector3__Scale(&push, &dir, 0.2048f);
                CVector3__Add(&sweep->hit.slideTo, &sweep->hit.slideTo, &push);
            } else {
                sweep->hit.type = 2;
            }
            break;
        case 4:
            sweep->kind = 1;
            sweep->contact = *normal;
            if (sweep->physics || sweep->more) {
                len = func_00298470(normal->x * normal->x + normal->y * normal->y + normal->z * normal->z);
                if (len == 0.0f) {
                    return 0;
                }
                CVector3__Scale(&dir, normal, 1.0f / len);
            }
            if (sweep->physics) {
                CVector3__ProjectKeepLength(&vel, &sweep->obj->m_vVelocity, &dir);
                sweep->hit.vel = vel;
            }
            if (sweep->more) {
                sweep->hit.type = 3;
                CVector3__Scale(&rest, &sweep->delta, 1.0f - t);
                CVector3__ProjectKeepLength(&move, &rest, &dir);
                CVector3__Add(&sweep->hit.slideTo, &sweep->hit.pos, &move);
                CVector3__Scale(&push, &dir, 0.2048f);
                CVector3__Add(&sweep->hit.slideTo, &sweep->hit.slideTo, &push);
            } else {
                sweep->hit.type = 2;
            }
            break;
        case 5:
            sweep->kind = 1;
            sweep->contact = *normal;
            if (sweep->physics || sweep->more) {
                len = func_00298470(normal->x * normal->x + normal->y * normal->y + normal->z * normal->z);
                if (len == 0.0f) {
                    return 0;
                }
                CVector3__Scale(&dir, normal, 1.0f / len);
            }
            if (sweep->physics) {
                CVector3__RemoveComponent(&a, &sweep->obj->m_vVelocity, &dir);
                CVector3__ProjectKeepLength(&b, &sweep->obj->m_vVelocity, &dir);
                CVector3__Lerp(&sweep->hit.vel, 0.25f, &a, &b);
                CVector3__Scale(&sweep->hit.vel, &sweep->hit.vel, 1.0f - friction);
            }
            if (sweep->more) {
                sweep->hit.type = 3;
                CVector3__Scale(&rest, &sweep->delta, 1.0f - t);
                CVector3__RemoveComponent(&a, &rest, &dir);
                CVector3__ProjectKeepLength(&b, &rest, &dir);
                CVector3__Lerp(&move, 0.25f, &a, &b);
                CVector3__Scale(&move, &move, 1.0f - friction);
                CVector3__Add(&sweep->hit.slideTo, &sweep->hit.pos, &move);
                CVector3__Scale(&push, &dir, 0.2048f);
                CVector3__Add(&sweep->hit.slideTo, &sweep->hit.slideTo, &push);
            } else {
                sweep->hit.type = 2;
            }
            break;
        case 6:
            break;
    }
    return 1;
}
