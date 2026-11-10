/* The particle templates on the host (docs/port/RENDER.md section 6).
 *
 * The particle-effects unit is an ISet of three blocks: block 0 a pool of sub-records, block 1
 * the sorted kinds (u32), block 2 the CROMParticleEffect templates (USet, 0x3C each) whose nine
 * pointer fields are offsets into block 0. On the N64 CScene__DecompressParticles relocates
 * them in place. On the host the pointers are 8 bytes, so the 0x3C records cannot hold them:
 * T2Port_ParticlesBuild builds a host copy of the templates (sizeof(CROMParticleEffect)) whose
 * pointers point at host-order copies of the sub-records, and CParticleSystem__Spawn indexes
 * that copy instead of block 2 (CParticle.m_pEffect then points into it).
 *
 * Block 0 stays big-endian in the archive (swap_spec.json "block0": "byte"): its records
 * overlap (the tool that packed them shares byte strings between records of different types),
 * so no in-place swap is right for every reader. Each record is decoded here by the slot that
 * points at it. Blocks 1 and 2 are swapped by the archive's plan as usual. */

#include "common.h"
#include "structs/particle.h"
#include "protos/iset.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern s32 CScene__GetObject(struct CScene *, s32);

/* block 2 as the archive leaves it: host-order fields at the N64 offsets */
typedef struct {
    u8 head[0x14];      /* m_dwFlags .. unk_11: scalars at the same offsets on both */
    s32 tag;            /* 0x14: the model's object index, -1: none */
    u32 offs[9];        /* 0x18..0x38: offsets into block 0 */
} T2ROMParticleEffect; /* 0x3C */

typedef struct T2ParticleHost {
    void *data;                    /* the unit (CParticleSystem.m_pParticleData) */
    CROMParticleEffect *effects;
    s32 count;
    u8 *pool;
    struct T2ParticleHost *next;   /* older copies */
} T2ParticleHost;

/* Every copy built, newest first. They are never freed: particles spawned from an older copy
 * may outlive the load that replaced it (the unit is decompressed again on scene loads, often
 * at the same address), and a freed copy was read by CParticle__HitFloor in the new-game
 * cinemas. A copy is ~150 KB; one per scene load. */
static T2ParticleHost *sHost;

static u16 be16(const u8 *p) { return (u16) ((p[0] << 8) | p[1]); }
static u32 be32(const u8 *p) { return ((u32) p[0] << 24) | ((u32) p[1] << 16) | ((u32) p[2] << 8) | p[3]; }
static f32 bef32(const u8 *p) { u32 w = be32(p); f32 f; memcpy(&f, &w, 4); return f; }

static void ranges(CPackedRange *out, const u8 *p, s32 n) {
    s32 i;
    for (i = 0; i < n; i++) {
        out[i].m_Base = be16(p + i * 4);
        out[i].m_Range = be16(p + i * 4 + 2);
    }
}

/* the host size of each slot's record (one allocation per template) */
#define SZ_IMPACT    sizeof(CROMParticleImpact)
#define SZ_OFFSET    (3 * sizeof(CPackedRange))
#define SZ_ROTATION  (6 * sizeof(CPackedRange))
#define SZ_DISTANCE  sizeof(CROMParticleDistance)
#define SZ_DIRECTION (3 * sizeof(CPackedRange))
#define SZ_OSC       sizeof(CROMParticleOscillation)
#define SZ_MOTION    sizeof(CROMParticleMotion)
#define SZ_COLORS    sizeof(CROMParticleColors)
#define SZ_PARAMS    sizeof(CROMParticleParams)
#define SZ_ALL (SZ_IMPACT + SZ_OFFSET + SZ_ROTATION + SZ_DISTANCE + SZ_DIRECTION + SZ_OSC + SZ_MOTION + \
                SZ_COLORS + SZ_PARAMS + 9 * 8)

static u8 *take(u8 **cur, size_t n) {
    u8 *p = *cur;
    *cur += (n + 7) & ~(size_t) 7;
    return p;
}

void T2Port_ParticlesBuild(void *scene, void *data) {
    const u8 *base = (const u8 *) CISet__GetBlock(data, 0);
    u8 *b2 = (u8 *) CISet__GetBlock(data, 2);
    s32 count = ((s32 *) b2)[1];
    T2ROMParticleEffect *rom = (T2ROMParticleEffect *) (b2 + 8);
    T2ParticleHost *h;
    u8 *cur;
    s32 i, k;

    if (((s32 *) b2)[0] != 0x3C) {
        fprintf(stderr, "[t2particles] template size %d, expected 0x3C\n", ((s32 *) b2)[0]);
        return;
    }
    h = (T2ParticleHost *) calloc(1, sizeof(T2ParticleHost));
    h->next = sHost;
    sHost = h;
    h->data = data;
    h->count = count;
    h->effects = (CROMParticleEffect *) calloc(count > 0 ? count : 1, sizeof(CROMParticleEffect));
    h->pool = (u8 *) calloc(count > 0 ? count : 1, SZ_ALL);
    cur = h->pool;
    for (i = 0; i < count; i++) {
        CROMParticleEffect *e = &h->effects[i];
        T2ROMParticleEffect *r = &rom[i];
        const u8 *p;

        memcpy(e, r->head, sizeof(r->head));
        e->m_pModel = r->tag == -1 ? NULL : (void *) (sptr) CScene__GetObject(scene, r->tag);

        /* 0x18 impact */
        {
            CROMParticleImpact *im = (CROMParticleImpact *) take(&cur, SZ_IMPACT);
            p = base + r->offs[0];
            for (k = 0; k < 14; k++) {
                im->m_ImpactEventValue[k].upper = (s16) be16(p + k * 8);
                im->m_ImpactEventValue[k].lower = (s16) be16(p + k * 8 + 2);
                im->m_ImpactEventValue[k].extra = (s32) be32(p + k * 8 + 4);
                im->m_ImpactParticleType[k] = be16(p + 0x70 + k * 2);
                im->m_ImpactEventType[k] = be16(p + 0x8C + k * 2);
                im->m_ImpactSoundType[k] = be16(p + 0xA8 + k * 2);
            }
            e->m_pImpact = im;
        }
        e->m_pOffset = (CPackedRange *) take(&cur, SZ_OFFSET);
        ranges(e->m_pOffset, base + r->offs[1], 3);
        e->m_pRotation = (CPackedRange *) take(&cur, SZ_ROTATION);
        ranges(e->m_pRotation, base + r->offs[2], 6);
        {
            CROMParticleDistance *d = (CROMParticleDistance *) take(&cur, SZ_DISTANCE);
            p = base + r->offs[3];
            ranges(d->m_Size, p, 6);
            d->m_ShadowScale = bef32(p + 0x18);
            d->m_NearScale = bef32(p + 0x1C);
            d->m_FarScale = bef32(p + 0x20);
            e->m_pDistanceRange = d;
        }
        e->m_pDirection = (CPackedRange *) take(&cur, SZ_DIRECTION);
        ranges(e->m_pDirection, base + r->offs[4], 3);
        {
            CROMParticleOscillation *o = (CROMParticleOscillation *) take(&cur, SZ_OSC);
            p = base + r->offs[5];
            ranges(o->m_Wobble, p, 6);
            o->m_Ramp = (s8) p[0x18];
            memcpy(o->unk_19, p + 0x19, 3);
            e->m_pOscillation = o;
        }
        {
            CROMParticleMotion *m = (CROMParticleMotion *) take(&cur, SZ_MOTION);
            p = base + r->offs[6];
            ranges(&m->m_Gravity, p, 1);
            ranges(&m->m_Speed, p + 4, 1);
            for (k = 0; k < 4; k++) {
                m->m_Render[k] = be16(p + 8 + k * 2);
            }
            m->m_Accel = be16(p + 0x10);
            m->m_MaxSpeed = be16(p + 0x12);
            e->m_pMotion = m;
        }
        e->m_pColors = (CROMParticleColors *) take(&cur, SZ_COLORS);
        memcpy(e->m_pColors, base + r->offs[7], sizeof(CROMParticleColors));
        {
            CROMParticleParams *pp = (CROMParticleParams *) take(&cur, SZ_PARAMS);
            p = base + r->offs[8];
            memcpy(pp, p, sizeof(CROMParticleParams));
            pp->m_LifeBase = (s16) be16(p);
            pp->m_LifeRange = (s16) be16(p + 2);
            e->m_pParams = pp;
        }
    }
}

/* The host templates of a particle unit (NULL if it was never built), and their count. */
CROMParticleEffect *T2Port_ParticleEffects(void *data, s32 *count) {
    T2ParticleHost *h;
    static long sCalls = 0;
    static long sEvery = -1;

    /* debug: T2_PARTICLE_LOG=N prints every N spawn calls (CParticleSystem__Spawn asks once per call) */
    if (sEvery < 0) {
        const char *e = getenv("T2_PARTICLE_LOG");
        sEvery = e != NULL ? atol(e) : 0;
    }
    if (sEvery > 0 && ++sCalls % sEvery == 0) {
        fprintf(stderr, "[t2particles] %ld spawn calls\n", sCalls);
    }

    for (h = sHost; h != NULL; h = h->next) {
        if (h->data == data && data != NULL) {
            if (count != NULL) {
                *count = h->count;
            }
            return h->effects;
        }
    }
    if (count != NULL) {
        *count = 0;
    }
    return NULL;
}
