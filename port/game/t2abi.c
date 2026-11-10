/* ABI adapters: calls whose caller and callee disagree on the types in a way MIPS o32 does not
 * see and x86-64 / arm64 do (docs/port/ABI.md; found by tools/port/abicheck.py).
 *
 * On MIPS the caller's view and the definition's view of a call meet in the same argument
 * words: a CVector3 by value is three words in a1..a3 (or on the stack), and so are three
 * floats after an integer first argument, or three s32 holding the floats' bits; a float after
 * an integer first argument travels in an integer register as its bits. On the host those are
 * different registers (SSE/FP vs general) or a struct vs scalars, so the callee reads garbage.
 *
 * Each caller file keeps its own declaration and, under TARGET_PORT only, #defines the callee to
 * one of the adapters below (a preprocessor redirect: the N64 build is untouched). An adapter
 * takes the caller's view and calls the definition with what the MIPS argument words meant:
 * words are reinterpreted bit for bit (memcpy), never converted numerically. */

#include "common.h"
#include "structs/vector.h"

#include <string.h>

static f32 bits_f(s32 w) {
    f32 f;
    memcpy(&f, &w, 4);
    return f;
}

static s32 f_bits(f32 f) {
    s32 w;
    memcpy(&w, &f, 4);
    return w;
}

static CVector3 v3_words(s32 x, s32 y, s32 z) {
    CVector3 v;
    v.x = bits_f(x);
    v.y = bits_f(y);
    v.z = bits_f(z);
    return v;
}

static CVector3 v3_floats(f32 x, f32 y, f32 z) {
    CVector3 v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

/* ---- struct results: the callers pass MIPS' hidden result pointer first ---------------- */

extern CQuaternion CGameObjectInstance__GetRotation(void *actor);
void CGameObjectInstance__GetRotation_sret(CQuaternion *out, void *actor) {
    *out = CGameObjectInstance__GetRotation(actor);
}

extern CQuaternion GameObject_GetGroundRotation(void *obj);
void GameObject_GetGroundRotation_sret(CQuaternion *out, void *obj) {
    *out = GameObject_GetGroundRotation(obj);
}

extern CQuaternion GameObject_GetAimRotation(void *actor, CVector3 point);
void GameObject_GetAimRotation_sret(CQuaternion *out, void *actor, CVector3 point) {
    *out = GameObject_GetAimRotation(actor, point);
}

/* ---- a CVector3 by value that the caller passes as three floats or three words ---------- */

extern f32 AI_AngleToPoint(void *actor, CVector3 point);
f32 AI_AngleToPoint_f3(void *actor, f32 x, f32 y, f32 z) {
    return AI_AngleToPoint(actor, v3_floats(x, y, z));
}
f32 AI_AngleToPoint_i3(s32 actor, s32 x, s32 y, s32 z) {
    return AI_AngleToPoint((void *) (sptr) actor, v3_words(x, y, z));
}

extern f32 AI_FindClearHeading(void *self, CVector3 pos, void *target, f32 scale);
f32 AI_FindClearHeading_i3(void *self, s32 x, s32 y, s32 z, void *target, f32 scale) {
    return AI_FindClearHeading(self, v3_words(x, y, z), target, scale);
}

extern s32 Collision_MoveInstance(void *obj, CVector3 pos, void *def);
void Collision_MoveInstance_f3(void *obj, f32 x, f32 y, f32 z, void *def) {
    Collision_MoveInstance(obj, v3_floats(x, y, z), def);
}

extern void GameObject_DropPickupAt(void *actor, CVector3 pos, s32 kind, f32 arg);
void GameObject_DropPickupAt_i3(void *actor, s32 x, s32 y, s32 z, s32 kind, f32 arg) {
    GameObject_DropPickupAt(actor, v3_words(x, y, z), kind, arg);
}

extern void GameObject_TraceMove(void *actor, CVector3 pos, CVector3 *outPos, void **outRegion,
                                 s32 flagA, s32 flagB);
/* func_0024699C: (b, x, y, z, outPos, outRegion, 0, 0) with the pointers as s32 words */
void GameObject_TraceMove_f3(void *actor, f32 x, f32 y, f32 z, s32 outPos, s32 outRegion, s32 flagA,
                             s32 flagB) {
    GameObject_TraceMove(actor, v3_floats(x, y, z), (CVector3 *) (sptr) outPos,
                         (void **) (sptr) outRegion, flagA, flagB);
}
/* func_00268560: the position as { f32 a; s32 b; f32 c } (TripleRecAt8): an integer and an SSE
 * eightbyte on x86-64 */
typedef struct {
    f32 a;
    s32 b;
    f32 c;
} T2AbiFIF;
void GameObject_TraceMove_fif(void *actor, T2AbiFIF t, void *outPos, s32 *outRegion, s32 flagA,
                              s32 flagB) {
    CVector3 v;
    memcpy(&v, &t, sizeof(v));
    GameObject_TraceMove(actor, v, outPos, (void **) outRegion, flagA, flagB);
}

extern s32 Quad_ContainsPoint(CVector3 *corners, CVector3 point);
s32 Quad_ContainsPoint_f3(void *quad, f32 x, f32 y, f32 z) {
    return Quad_ContainsPoint(quad, v3_floats(x, y, z));
}

extern f32 Rumble_GetLevelAt(void *list, CVector3 pos);
f32 Rumble_GetLevelAt_i3(void *list, s32 x, s32 y, s32 z) {
    return Rumble_GetLevelAt(list, v3_words(x, y, z));
}

extern s32 Sound__Play(s16 id, CVector3 pos, sptr follow, s32 owner);
void Sound__Play_f3(s32 id, f32 x, f32 y, f32 z, s32 follow, s32 owner) {
    Sound__Play(id, v3_floats(x, y, z), follow, owner);
}
s32 Sound__Play_i3(s32 id, s32 x, s32 y, s32 z, s32 follow, s32 owner) {
    return Sound__Play(id, v3_words(x, y, z), follow, owner);
}

/* the vector as three u32 words in the definition, three floats in the caller */
extern s32 Actors_GetLightVolumeFalloff(void *obj, u32 x, u32 y, u32 z, CVector3 *dir, f32 *amount);
s32 Actors_GetLightVolumeFalloff_f3(void *obj, f32 x, f32 y, f32 z, f32 *dir, f32 *amount) {
    return Actors_GetLightVolumeFalloff(obj, f_bits(x), f_bits(y), f_bits(z), (CVector3 *) dir, amount);
}
s32 Actors_GetLightVolumeFalloff_v(void *obj, CVector3 v, void *dir, f32 *amount) {
    return Actors_GetLightVolumeFalloff(obj, f_bits(v.x), f_bits(v.y), f_bits(v.z), dir, amount);
}

extern void DLights_GetAtPoint(void *lights, u32 x, u32 y, u32 z, CVector3 *dir, f32 *amount);
void DLights_GetAtPoint_f3(void *lights, f32 x, f32 y, f32 z, CVector3 *dir, f32 *amount) {
    DLights_GetAtPoint(lights, f_bits(x), f_bits(y), f_bits(z), dir, amount);
}

/* ---- other structs by value -------------------------------------------------------------- */

typedef struct {
    u32 value;
    f32 start;
    f32 delta;
} T2AbiDecodeRange; /* Func802608ECResult */
extern f32 Bits_ReadFloat(u32 *stream, T2AbiDecodeRange range);
f32 Bits_ReadFloat_3(u32 *stream, s32 bits, f32 base, f32 range) {
    T2AbiDecodeRange r;
    r.value = bits;
    r.start = base;
    r.delta = range;
    return Bits_ReadFloat(stream, r);
}

typedef struct {
    s32 unk0;
    s32 unk4;
} T2AbiEventParams;
typedef struct {
    CVector3 pos;
    s32 type;
} T2AbiAnimEvent;
/* ImpactEvent_LevelSpecial: (a0, a1, a2, CVector3 pos at word 3, EventParams at word 6) */
extern void MotherBoss_AnimEvent(s32 arg0, void *self, s32 arg2, T2AbiAnimEvent ev);
void MotherBoss_AnimEvent_ev(s32 arg0, s32 arg1, s32 arg2, CVector3 pos, T2AbiEventParams params) {
    T2AbiAnimEvent ev;
    ev.pos = pos;
    ev.type = params.unk0;
    MotherBoss_AnimEvent(arg0, (void *) (sptr) arg1, arg2, ev);
}
extern void Primagen_HandleAnimEvent(s32 arg0, void *obj, s32 arg2, CVector3 pos, s32 event);
void Primagen_HandleAnimEvent_ev(s32 arg0, s32 arg1, s32 arg2, CVector3 pos, T2AbiEventParams params) {
    Primagen_HandleAnimEvent(arg0, (void *) (sptr) arg1, arg2, pos, params.unk0);
}

/* ---- a float parameter the caller passes as an integer word (its bits), or the reverse ---- */

extern void AI_SetTargetOverride(s32 obj, void *ai, s32 owner, s32 time);
void AI_SetTargetOverride_f(void *obj, void *ai, void *owner, f32 time) {
    AI_SetTargetOverride((s32) (sptr) obj, ai, (s32) (sptr) owner, f_bits(time));
}

extern void AI_TurnClamped(void *obj, sptr ai, f32 angle, f32 rate);
void AI_TurnClamped_i(sptr obj, sptr ai, f32 angle, s32 rate) {
    AI_TurnClamped((void *) obj, ai, angle, bits_f(rate));
}

extern void AI_TurnHeadToTarget(void *actor, void *ai, f32 pitchOffset);
void AI_TurnHeadToTarget_i(sptr actor, void *ai, s32 pitchOffset) {
    AI_TurnHeadToTarget((void *) actor, ai, bits_f(pitchOffset));
}

extern void Automap_SetPlayer(void *obj, sptr region, s32 yaw, CVector3 pos);
void Automap_SetPlayer_f(void *obj, sptr region, f32 yaw, CVector3 pos) {
    Automap_SetPlayer(obj, region, f_bits(yaw), pos);
}

extern void *CInstancePool__SpawnAnimInstance(void *scene, s32 variant, s32 owner, CVector3 position,
                                              f32 scale, CVector3 direction, CVector3 color, s32 *ref);
void CInstancePool__SpawnAnimInstance_i(void *scene, s32 variant, s32 owner, CVector3 position,
                                        s32 scale, CVector3 direction, CVector3 color, sptr ref) {
    CInstancePool__SpawnAnimInstance(scene, variant, owner, position, bits_f(scale), direction, color,
                                     (s32 *) ref);
}

extern void *CLevelActorPool__Spawn(void *pool, s32 *refCount, s32 id, CVector3 vel, CVector3 pos,
                                    s32 arg9, f32 lifetime);
void *CLevelActorPool__Spawn_i(void *pool, void *refCount, s32 id, CVector3 vel, CVector3 pos, s32 arg9,
                               s32 lifetime) {
    return CLevelActorPool__Spawn(pool, refCount, id, vel, pos, arg9, bits_f(lifetime));
}

extern void CMtxF__RotateX(void *m, f32 a);
extern void CMtxF__RotateZ(void *m, f32 a);
extern void CMtxF__SetRotateY(void *m, f32 a);
extern void CMtxF__SetRotateZ(void *m, f32 a);
s32 CMtxF__RotateX_i(s32 m, s32 a) {
    CMtxF__RotateX((void *) (sptr) m, bits_f(a));
    return 0;
}
s32 CMtxF__RotateZ_i(s32 m, s32 a) {
    CMtxF__RotateZ((void *) (sptr) m, bits_f(a));
    return 0;
}
void CMtxF__SetRotateY_i(void *m, s32 a) {
    CMtxF__SetRotateY(m, bits_f(a));
}
void CMtxF__SetRotateZ_i(void *m, s32 a) {
    CMtxF__SetRotateZ(m, bits_f(a));
}

extern void CQuaternion__Slerp(void *out, f32 amount, void *a, void *b);
void CQuaternion__Slerp_i(void *out, s32 amount, void *a, void *b) {
    CQuaternion__Slerp(out, bits_f(amount), a, b);
}

extern void CVector3__Lerp(void *out, f32 t, void *a, void *b);
void CVector3__Lerp_i(void *out, s32 t, void *a, void *b) {
    CVector3__Lerp(out, bits_f(t), a, b);
}

extern s32 Sweep_HitCylinder(void *sweep, CVector3 *centre, f32 radius, s32 checkHeight, f32 maxY,
                             f32 minY, s32 frontOnly, s32 arg7, f32 arg8, s32 fromInside);
s32 Sweep_HitCylinder_i(void *sweep, CVector3 *centre, f32 radius, s32 checkHeight, f32 maxY, f32 minY,
                        s32 frontOnly, s32 arg7, s32 arg8, s32 fromInside) {
    return Sweep_HitCylinder(sweep, centre, radius, checkHeight, maxY, minY, frontOnly, arg7,
                             bits_f(arg8), fromInside);
}
