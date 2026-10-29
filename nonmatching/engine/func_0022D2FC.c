/* NON-MATCHING: 13 of 624 words differ: register allocation only, in the inlined depth scale (ROM keeps the depth argument in $f5 and the clamped factor in $f0; this build uses $f1 and $f2). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_4.CPP, Particle_BuildMatrix, with Turok 2's layouts. */
#include "common.h"

/* Builds a particle's display matrix: its size scaled for camera depth and
 * field of view, its wobble, its orientation (model particles) or roll
 * (sprites), its placement for the space it lives in, and an optional
 * decaying shake, then packs the result into the fixed-point matrix the
 * display list loads. */

typedef struct { f32 x, y, z; } Vec3f;
typedef f32 Mtx4f[4][4];

#define MIN_LT(a, b) ((a) < (b) ? (a) : (b))
#define MAX_GT(a, b) ((a) > (b) ? (a) : (b))

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 minimum;
    /* 0x20 */ f32 maximum;
} DistanceRange;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s8 ramp;
} Oscillation;

typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x8 - 0x4];
    /* 0x08 */ s8 animMode;
    /* 0x09 */ u8 pad9;
    /* 0x0A */ s8 clampToGround;
    /* 0x0B */ u8 padB[0x14 - 0xB];
    /* 0x14 */ s32 renderModel;
    /* 0x18 */ u8 pad18[0x24 - 0x18];
    /* 0x24 */ DistanceRange *distanceRange;
    /* 0x28 */ u8 pad28[0x2C - 0x28];
    /* 0x2C */ Oscillation *oscillation;
} ParticleDesc;

typedef struct {
    /* 0x00 */ u8 pad0[0x6C];
    /* 0x6C */ f32 fovY;
} Camera;

typedef struct {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ void *region;
    /* 0x014 */ u8 pad14[0x40 - 0x14];
    /* 0x040 */ u32 flags;
    /* 0x044 */ u8 pad44[0xF8 - 0x44];
    /* 0x0F8 */ ParticleDesc *desc;
    /* 0x0FC */ u8 padFC[0x11C - 0xFC];
    /* 0x11C */ f32 time;
    /* 0x120 */ u8 pad120[0x12C - 0x120];
    /* 0x12C */ Vec3f size;
    /* 0x138 */ u8 pad138[0x150 - 0x138];
    /* 0x150 */ Vec3f direction;
    /* 0x15C */ Vec3f rot;
    /* 0x168 */ u8 pad168[0x178 - 0x168];
    /* 0x178 */ f32 phaseA;
    /* 0x17C */ f32 phaseB;
    /* 0x180 */ u8 pad180[0x188 - 0x180];
    /* 0x188 */ f32 amplitudeA;
    /* 0x18C */ f32 amplitudeB;
    /* 0x190 */ u8 pad190[0x1AC - 0x190];
    /* 0x1AC */ s8 space;
    /* 0x1AD */ s8 shake;
    /* 0x1AE */ u8 pad1AE[0x1B4 - 0x1AE];
    /* 0x1B4 */ u8 opacity;
} Particle;

extern void func_0022D09C(Particle *p, Vec3f *outPosition, Vec3f *outHeading);
extern Mtx4f *func_0022CF40(Particle *p, Camera *camera);
extern void func_0020EFDC(Vec3f *out, Vec3f *a, Vec3f *b);       /* cross product */
extern void func_0020EF60(Vec3f *out, Vec3f *in, f32 s);         /* out = in * s */
extern void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);       /* out = a + b */
extern void func_0020F040(Vec3f *v);                             /* normalize */
extern f32 func_002982F0(f32 angle);                             /* sin */
extern void func_0020FE00(Mtx4f m, f32 x, f32 y, f32 z);         /* rotation */
extern void func_0020F79C(Mtx4f m);                              /* identity */
extern void func_0020C618(Mtx4f out, Mtx4f a, Mtx4f b);          /* out = a * b */
extern void func_00210630(Mtx4f m, f32 angle);                   /* roll */
extern void func_00210284(Mtx4f m, f32 x, f32 y, f32 z);
extern void func_00210318(Mtx4f m, f32 x, f32 y, f32 z);         /* translate */
extern f32 func_0026BE60(void *region, f32 x, f32 z);            /* ground height */
extern f32 func_00269C3C(void *region, f32 x, f32 z);            /* ceiling height */
extern void func_0020F510(Vec3f *v, f32 limit);
extern void func_0021034C(Mtx4f m, f32 x, f32 y, f32 z);         /* scale */
extern void func_00210C3C(Mtx4f m);
extern void func_0020FF80(Mtx4f dst, Mtx4f src);                 /* copy */
extern void func_0020FC24(Mtx4f m, f32 x, f32 y, f32 z);         /* translation */
extern void func_00210968(Mtx4f m, f32 angle);
extern void func_00210790(Mtx4f m, f32 angle);
extern void func_0020D274(Mtx4f m, void *out);                   /* pack */

static __inline__ f32 particleDepthScale(ParticleDesc *desc, f32 depth) {
    f32 farness;
    f32 near;
    f32 size;

    if (desc->flags & 4) {
        size = depth;
        near = size - 16.0f;
        farness = MIN_LT(1.0f, MAX_GT(0.0f, near) * (1.0f / 2032.0f));
        size = size * (desc->distanceRange->minimum
            + (desc->distanceRange->maximum - desc->distanceRange->minimum) * farness) * 0.001f;
    } else {
        size = 1.0f;
    }
    return size;
}

/* Written as an inline with an early return: the ROM's register choices for
 * the fade and the wobble's 0.0 come from that shape. */
static __inline__ f32 rampFade(f32 time, s32 ramp) {
    f32 length;

    length = (f32)ramp;
    if (time < length) {
        return time / length;
    }
    return 1.0f;
}

void func_0022D2FC(Particle *particle, void *out, Camera *camera, f32 depth) {
    Mtx4f rotation, basis, local, world, shakeMtx;
    Vec3f position;
    Vec3f up;
    Vec3f heading;
    Vec3f axisX;
    Vec3f axisY;
    Vec3f axisZ;
    Vec3f across;
    Vec3f side;
    Vec3f direction;
    Vec3f offset;
    ParticleDesc *desc;
    f32 *final;
    f32 scale;
    f32 lift;
    f32 depthScale;
    f32 ramp;
    f32 fade;
    f32 ground;
    f32 rest;
    f32 extent;
    f32 jolt;
    f32 joltSq;
    f32 angle;
    f32 tiltX;
    f32 tiltY;
    f32 fov;
    s32 shake;
    s32 model;

    desc = particle->desc;
    if (camera != 0) {
        depthScale = particleDepthScale(particle->desc, depth);
        fov = camera->fovY;
        scale = depthScale * (1.0f / 47.5f) * (fov + (47.5f - fov) * 0.25f);
    } else {
        scale = 1.0f;
    }
    if (desc->flags & 8) {
        lift = particle->size.y * particle->size.z * scale;
    } else {
        lift = 0.0f;
    }
    func_0022D09C(particle, &position, &heading);
    if (!(particle->flags & 0x30000)) {
        ramp = particle->time;
        if (ramp > 0.0f) {
            fade = rampFade(ramp, desc->oscillation->ramp);
            direction = particle->direction;
            if (particle->amplitudeA < 0.0f) {
                up.x = 0;
                up.z = 0;
                up.y = 1.0f;
                func_0020EFDC(&side, &up, &direction);
                func_0020EF60(&offset, &side, func_002982F0(particle->phaseA) * -particle->amplitudeA * fade);
                func_0020EEF8(&position, &position, &offset);
            }
            if (particle->amplitudeB < 0.0f) {
                up.x = 0;
                up.z = 0;
                up.y = 1.0f;
                func_0020EFDC(&across, &up, &direction);
                func_0020EFDC(&side, &across, &direction);
                func_0020EF60(&offset, &side, func_002982F0(particle->phaseB) * -particle->amplitudeB * fade);
                func_0020EEF8(&position, &position, &offset);
            }
        }
    }
    if (particle->desc->renderModel != 0) {
        if (particle->space == -7) {
            func_0020FE00(rotation, particle->rot.x + 3.141593f / 2, particle->rot.y, particle->rot.z);
            if ((heading.x == 0.0f) && (heading.y == 0.0f) && (heading.z == 0.0f)) {
                axisZ = particle->direction;
            } else {
                axisZ = heading;
            }
            func_0020F040(&axisZ);
            up.x = 0;
            up.z = 0;
            up.y = 1.0f;
            func_0020EFDC(&axisX, &up, &axisZ);
            func_0020EFDC(&axisY, &axisX, &axisZ);
            func_0020F79C(basis);
            func_0020F040(&axisX);
            func_0020F040(&axisY);
            func_0020F040(&axisZ);
            basis[0][0] = axisX.x;
            basis[0][1] = axisX.y;
            basis[0][2] = axisX.z;
            basis[1][0] = axisY.x;
            basis[1][1] = axisY.y;
            basis[1][2] = axisY.z;
            basis[2][0] = axisZ.x;
            basis[2][1] = axisZ.y;
            basis[2][2] = axisZ.z;
            func_0020C618(local, rotation, basis);
        } else {
            func_0020FE00(local, particle->rot.x, particle->rot.y + 3.141593f, particle->rot.z);
        }
    } else if ((particle->rot.x == 0.0f) && (particle->rot.y == 0.0f)) {
        func_00210630(local, particle->rot.z);
    } else {
        func_00210630(local, particle->rot.z);
        model = particle->desc->renderModel;
        tiltX = 0.0f;
        if (model == 0) {
            tiltX = particle->rot.x;
        }
        tiltX = -tiltX;
        tiltY = 0.0f;
        if (model == 0) {
            tiltY = particle->rot.y;
        }
        func_00210284(local, tiltX, -tiltY, 0.0f);
    }
    if (lift != 0.0f) {
        func_00210284(local, 0.0f, lift, 0.0f);
        func_00210318(local, 0.0f, -lift, 0.0f);
    }
    particle->flags &= ~0x800000;
    switch (particle->space) {
    case -8:
        position.x += particle->direction.x * 5.12f;
        position.y += particle->direction.y * 5.12f;
        position.z += particle->direction.z * 5.12f;
        break;
    case -5:
        if (desc->clampToGround != 0) {
            position.y = MAX_GT(position.y, func_0026BE60(particle->region, position.x, position.z) + 5.12f);
        }
        break;
    case -3:
        ground = func_0026BE60(particle->region, position.x, position.z);
        if (desc->flags & 0x400) {
            if (position.y < ground + 1.024f) {
                position.y = ground;
                particle->flags |= 0x800000;
            }
        } else {
            rest = ground + 5.12f;
            position.y = (position.y <= rest) ? rest : position.y;
        }
        break;
    case -2:
        if (desc->clampToGround != 0) {
            position.y = MIN_LT(position.y, func_00269C3C(particle->region, position.x, position.z) - 5.12f);
        }
        break;
    case -6:
    case -4:
    case -1:
        position.y += lift;
        break;
    case -7:
    case 0:
    case 1:
    case 2:
        break;
    }
    func_0020F510(&position, 20000.0f);
    if ((desc->flags & 0x80000) && (desc->animMode == 1)) {
        scale = scale * (1.0f / 255.0f) * (f32)particle->opacity;
    }
    if (particle->desc->renderModel != 0) {
        extent = scale * particle->size.z * (35.0f / 1024.0f);
        func_0021034C(local, particle->size.x * extent, particle->size.y * extent, extent * 10.24f);
        func_00210C3C(local);
    } else {
        extent = scale * particle->size.z;
        func_0021034C(local, particle->size.x * extent, particle->size.y * extent, extent * 10.24f);
    }
    {
        Mtx4f *parent = func_0022CF40(particle, camera);
        if (parent != 0) {
            func_0020C618(world, local, *parent);
        } else {
            func_0020FF80(world, local);
        }
    }
    func_00210318(world, position.x, position.y, position.z);
    if ((particle->flags & 0x20000) && (shake = particle->shake) != 0) {
        jolt = (f32)shake * (1.0f / 127.0f);
        joltSq = jolt * jolt;
        angle = joltSq * (3.141593f * 5 / 180);
        func_0020FC24(shakeMtx, 0.0f, 0.0f, -66.56f);
        func_00210968(shakeMtx, angle * (func_002982F0(joltSq * (6 * 3.141593f)) * 0.25f));
        func_00210790(shakeMtx, angle * func_002982F0(joltSq * (14 * 3.141593f)));
        func_00210318(shakeMtx, 0.0f, 0.0f, 66.56f);
        func_0020C618(rotation, shakeMtx, world);
        final = rotation[0];
    } else {
        final = world[0];
    }
    func_0020D274((void *)final, out);
}
