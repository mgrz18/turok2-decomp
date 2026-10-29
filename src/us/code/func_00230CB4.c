/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/CODE_80281A70.CPP, func_80281CA0_de, with Turok 2's layouts. */
#include "common.h"

/* Steps the particle system once: marks every particle on the three active
 * lists, then (outside a pause) advances the ones inside the visible bounds,
 * fires the impact events of the expired ones and frees them, and lets each
 * collected type 0x22 particle react to the others and each type 0x57
 * particle expire the non-0x57 ones within range. */

typedef struct { f32 x, y, z; } Vec3f;
typedef struct { s32 a, b; } IntPair;

typedef struct {
    /* 0x00 */ Vec3f min;
    /* 0x0C */ Vec3f max;
} Bounds;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ f32 colorFade;
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 z;
    /* 0x16 */ s16 enabled;
} DLight;

typedef struct {
    /* 0x00 */ IntPair eventValue[14];
    /* 0x70 */ u16 particleType[14];
    /* 0x8C */ u16 eventType[14];
    /* 0xA8 */ u16 soundType[14];
} ParticleImpact;

typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x18 - 0x4];
    /* 0x18 */ ParticleImpact *impact;
} ParticleEffect;

typedef struct Particle {
    /* 0x000 */ u8 pad0[0x2];
    /* 0x002 */ u16 type;
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[0x40 - 0x10];
    /* 0x040 */ u32 flags;
    /* 0x044 */ u8 pad44[0xF8 - 0x44];
    /* 0x0F8 */ ParticleEffect *effect;
    /* 0x0FC */ u8 padFC[0x118 - 0xFC];
    /* 0x118 */ DLight *light;
    /* 0x11C */ f32 frame;
    /* 0x120 */ u8 pad120[0x128 - 0x120];
    /* 0x128 */ s16 frames;
    /* 0x12A */ u8 pad12A[0x174 - 0x12A];
    /* 0x174 */ f32 alpha;
    /* 0x178 */ u8 pad178[0x1AC - 0x178];
    /* 0x1AC */ s8 alignment;
    /* 0x1AD */ u8 pad1AD[0x1C8 - 0x1AD];
    /* 0x1C8 */ struct Particle *next;
} Particle;

typedef struct {
    /* 0x00 */ Particle *head;
    /* 0x04 */ u8 pad4[0x10];
} ParticleList;

typedef struct {
    /* 0x0000 */ u8 pad0[0xEC28];
    /* 0xEC28 */ ParticleList active[3];
} ParticleSystem;

extern f32 D_800B6D28;          /* frame time scale */
extern f32 D_800B6D30;
extern s32 D_8011ACA4;
extern s32 D_80110028;
extern Bounds D_800F5A10;       /* visible bounds */

extern void func_0022AFB4(Particle *p);
extern s32 func_002325F0(Particle *p);
extern void func_00236EA0(Particle *p, Particle *source, s32 type, s32 id, Vec3f pos, IntPair value);
extern void func_00229340(Particle *p, s32 type, s32 alignment, s32 count);
extern void func_00275544(s16 sound, Vec3f pos, s32 a, s32 b);
extern void func_0022F1C8(ParticleSystem *world, Particle *p);
extern void func_00230AB0(ParticleSystem *world, Particle *p, Particle **others, s32 count);
extern void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);    /* out = a - b */

void func_00230CB4(ParticleSystem *world) {
    Particle *colliders[128];
    Particle *stepped[128];
    IntPair value;
    Vec3f delta;
    Particle *p;
    Particle *node;
    Particle *focus;
    Particle **begin;
    ParticleEffect *effect;
    DLight *light;
    s32 count;
    s32 idx;
    s32 pairCount;
    s32 i;
    s32 j;
    s32 event;
    u32 particleType;
    u32 eventType;
    u32 soundType;
    f32 savedStep;

    count = 0;
    for (idx = 0; idx < 3; idx++) {
        for (node = (world->active + idx)->head; node != 0; node = node->next) {
            node->flags = (node->flags & 0xFFEFFFFF) | 0x02000000;
            stepped[count++] = node;
        }
    }
    if (D_8011ACA4 != 0 || D_80110028 != 0) {
        return;
    }
    savedStep = D_800B6D28;
    D_800B6D28 = savedStep * D_800B6D30;
    pairCount = 0;
    for (idx = 0; idx < count; idx++) {
        p = stepped[idx];
        if ((p->flags & 0x02000100) != 0x02000100) {
            continue;
        }
        p->flags &= 0xFFEFFF87;
        if (p->light) {
            p->light->enabled = 0;
        }
        if (p->frame < (f32)p->frames && p->frames != 1 && !(p->flags & 0x1000)) {
            if (D_800F5A10.max.x > p->pos.x && D_800F5A10.max.y > p->pos.y && D_800F5A10.max.z > p->pos.z &&
                D_800F5A10.min.x <= p->pos.x && D_800F5A10.min.y <= p->pos.y && D_800F5A10.min.z <= p->pos.z) {
                func_0022AFB4(p);
                light = p->light;
                if (light) {
                    light->enabled = 1;
                    light->x = p->pos.x;
                    light->y = p->pos.y;
                    light->z = p->pos.z;
                    light->colorFade = p->alpha * (1.0f / 255.0f);
                }
                p->frame += D_800B6D28;
            } else {
                func_0022F1C8(world, p);
            }
        } else {
            effect = p->effect;
            event = func_002325F0(p) ? 12 : 10;
            particleType = effect->impact->particleType[event];
            eventType = effect->impact->eventType[event];
            value = effect->impact->eventValue[event];
            soundType = effect->impact->soundType[event];
            if (eventType != 0xFFFF) {
                func_00236EA0(p, p, eventType, -1, p->pos, value);
            }
            if (particleType != 0xFFFF) {
                func_00229340(p, particleType, p->alignment, 1);
            }
            if (soundType != 0xFFFF) {
                func_00275544(soundType, p->pos, 0, -1);
            }
            if (!(p->flags & 0x100)) {
                continue;
            }
            func_0022F1C8(world, p);
        }
        if (p->flags & 0x100) {
            switch (p->type) {
            case 12:
            case 34:
            case 87:
                colliders[pairCount++] = p;
                break;
            }
        }
    }
    if (pairCount != 0) {
        begin = colliders;
        for (i = 0; i < pairCount; i++) {
            focus = begin[i];
            switch (focus->type) {
            case 34:
                func_00230AB0(world, focus, begin, pairCount);
                break;
            case 87:
                for (j = 0; j < pairCount; j++) {
                    p = begin[j];
                    if (p->type != 87) {
                        func_0020EF2C(&delta, &p->pos, &focus->pos);
                        if (delta.x * delta.x + delta.y * delta.y + delta.z * delta.z <= 65536.0f) {
                            p->frames = 1;
                        }
                    }
                }
                break;
            case 12:
                break;
            }
        }
    }
    D_800B6D28 = savedStep;
}
