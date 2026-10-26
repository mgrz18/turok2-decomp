/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/CODE_80245D20.CPP, func_80245D30_de, with Turok 2's layouts. */
#include "common.h"

typedef struct { f32 x, y, z; } Vec3f;

extern s32 func_002685F0(void *actor);
extern f32 func_0020EAA0(Vec3f *v);                                 /* yaw of v */
extern void func_0020EF60(Vec3f *out, Vec3f *in, f32 s);            /* out = in * s */
extern void func_0020EF8C(Vec3f *out, f32 t, Vec3f *a, Vec3f *b);   /* out = lerp(a, b, t) */
extern void func_00210EF0(f32 *angle);                              /* wraps an angle */

extern f32 D_800B25F0[2];   /* current: blend rate, turn rate */
extern f32 D_800B6D28;      /* frame time scale */

/* A surface that carries what stands on it (water current, conveyor). */
typedef struct {
    /* 0x00 */ u8 pad0[0x52];
    /* 0x52 */ u16 flags;
    /* 0x54 */ u8 pad54[0x5D - 0x54];
    /* 0x5D */ u8 strength;
    /* 0x5E */ u8 pad5E;
    /* 0x5F */ s8 x;
    /* 0x60 */ s8 y;
    /* 0x61 */ s8 z;
} Material;

typedef struct {
    /* 0x000 */ u8 kind;
    /* 0x001 */ u8 pad1[0x1C - 0x1];
    /* 0x01C */ f32 vertical;
    /* 0x020 */ u8 pad20[0x24 - 0x20];
    /* 0x024 */ Vec3f current;
    /* 0x030 */ u8 pad30[0x50 - 0x30];
    /* 0x050 */ f32 yaw;
    /* 0x054 */ u8 pad54[0x140 - 0x54];
    /* 0x140 */ s32 flags;
} Actor;

static __inline__ s32 currentEnabled(Actor *actor) {
    if (actor->flags & 0x280) return 1;
    if (actor->flags & 0x400) return func_002685F0(actor);
    return 0;
}

/* Applies a surface current to an actor: pushes pos, turns the actor with it
 * (flags 0x600 spin, 0x2000 face the flow) and handles the vertical part. */
void func_00269050(Actor *actor, Material *mat, Vec3f *pos, s32 active) {
    Actor *rider;
    Vec3f flow;
    Vec3f flat;
    f32 cur;
    f32 want;
    f32 spin;
    f32 strength;
    f32 wrapped;
    f32 target;
    f32 from;
    f32 blend;
    s32 enabled;
    s32 blocked;
    u16 flags;
    Vec3f *current;

    if (actor->kind == 1) {
        rider = actor;
        enabled = currentEnabled(actor);
        blocked = rider->flags & 0xC0000;
    } else {
        enabled = 0;
        blocked = 0;
    }
    if (blocked != 0 || (enabled == 0 && !(mat->flags & 0x8000))) {
        actor->current.x = 0.0f;
        actor->current.y = 0.0f;
        actor->current.z = 0.0f;
        return;
    }
    if ((mat->flags & 0x100) && active != 0) {
        flow.x = mat->x * 0.007874016f;
        flow.y = mat->y * 0.007874016f;
        flow.z = mat->z * 0.007874016f;
        strength = mat->strength;
        spin = (strength + 1.0f) * 0.008726647f;
        func_0020EF60(&flow, &flow, strength * 3.0f);
        flags = mat->flags;
        if (flags & 0x600) {
            if (flags & 0x400) {
                spin = -spin;
            }
            if (actor->kind == 1) {
                actor->yaw += spin;
            }
        }
        if ((mat->flags & 0x2000) && actor->kind == 1) {
            rider = actor;
            flat = flow;
            flat.y = 0.0f;
            want = func_0020EAA0(&flat);
            cur = actor->yaw;
            if (want != cur) {
                func_00210EF0(&cur);
                func_00210EF0(&want);
                if (cur > want) {
                    wrapped = want + 6.2831855f;
                    if ((wrapped - cur) < (cur - want)) want = wrapped;
                } else {
                    wrapped = want - 6.2831855f;
                    if ((cur - wrapped) < (want - cur)) want = wrapped;
                }
                target = want;
                from = cur;
                if (from < target) {
                    cur = from + mat->strength * D_800B25F0[1] * D_800B6D28 * 0.017453294f;
                    if (target < cur) cur = target;
                } else {
                    cur = from - mat->strength * D_800B25F0[1] * D_800B6D28 * 0.017453294f;
                    if (cur < target) cur = target;
                }
                func_00210EF0(&cur);
            }
            rider->yaw = cur;
        }
    } else {
        flow.x = 0.0f;
        flow.y = 0.0f;
        flow.z = 0.0f;
    }
    blend = mat->strength * D_800B25F0[0] * D_800B6D28;
    blend = ((blend < 0.0f ? 0.0f : blend) > 1.0f ? 1.0f : (blend < 0.0f ? 0.0f : blend));
    current = &actor->current;
    func_0020EF8C(current, blend, current, &flow);
    pos->x += actor->current.x;
    pos->z += actor->current.z;
    if (mat->flags & 0x100) {
        if (flow.y != 0.0f) {
            actor->vertical = 0.0f;
        } else {
            actor->current.y += (flow.y - actor->current.y) * 0.2f;
        }
        actor->vertical += 2.0f * (actor->current.y * 10.24f) + 4.0f;
        return;
    }
    actor->current.y += (flow.y - actor->current.y) * 0.2f;
}
