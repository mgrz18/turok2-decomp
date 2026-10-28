#include "common.h"
#include "structs/camera.h"

#define MIN(a, b) ((a) > (b) ? (b) : (a))

/* One entry of the emitter definition table D_8010FF14, 0x2C bytes. */
typedef struct {
    /* 0x00 */ s16 id;
    /* 0x02 */ u8 pad2[6];
    /* 0x08 */ f32 spinRate;
    /* 0x0C */ u8 padC[4];
    /* 0x10 */ f32 phaseRate;
    /* 0x14 */ f32 baseSpeed;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ f32 hostScale;
    /* 0x20 */ f32 throttleRate;
    /* 0x24 */ u8 pad24[5];
    /* 0x29 */ u8 flags;
    /* 0x2A */ u8 pad2A[2];
} EmitterDef; /* size 0x2C */

typedef struct Emitter {
    /* 0x000 */ struct Emitter *last;
    /* 0x004 */ struct Emitter *next;
    /* 0x008 */ u8 pad8[4];
    /* 0x00C */ EmitterDef *def;
    /* 0x010 */ u16 mask;
    /* 0x012 */ u8 pad12[2];
    /* 0x014 */ s32 resource;
    /* 0x018 */ u8 pad18[4];
    /* 0x01C */ f32 angle0;
    /* 0x020 */ f32 angle1;
    /* 0x024 */ u8 pad24[0x1E4];
    /* 0x208 */ f32 throttle;
    /* 0x20C */ f32 phase;
    /* 0x210 */ f32 speed;
    /* 0x214 */ f32 phaseScale;
    /* 0x218 */ f32 offset0;
    /* 0x21C */ f32 trim;
    /* 0x220 */ f32 offset2;
} Emitter;

typedef struct {
    /* 0x000 */ u8 pad0[0x8A0];
    /* 0x8A0 */ CList freeList;
    /* 0x8B4 */ CList activeList;
    /* 0x8C8 */ u16 mask;
} EmitterOwner;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 height;
    /* 0x24 */ u8 pad24[0x20];
    /* 0x44 */ s32 flags;
    /* 0x48 */ u8 pad48[0xE];
    /* 0x56 */ u16 emitterMask;
} RegionInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 stalled;
    /* 0x28 */ u8 pad28[0x10];
    /* 0x38 */ f32 x;
    /* 0x3C */ u8 pad3C[4];
    /* 0x40 */ f32 z;
    /* 0x44 */ u8 pad44[0x14];
    /* 0x58 */ s32 region;
    /* 0x5C */ u8 pad5C[0xBC];
    /* 0x118 */ f32 speed;
} Host;

typedef struct { f32 x, y, z; } Vec3f;

extern u8 D_800F7078[];
extern EmitterDef *D_8010FF14;  /* emitter definitions */
extern s32 D_8010FF18;          /* how many */
extern f32 D_800B6D28;          /* the frame's time step */
extern f32 D_800B6D34;

RegionInfo *func_0022425C(void *world, s32 region);
f32 func_0026BE60(s32 region, f32 x, f32 z);
Vec3f func_00280E40(Host *host);
void func_00207914(s32 *resource, s32 id);
void CList__Remove(CList *list, void *element);
void CList__AddHead(CList *list, void *element);
void CList__AddTail(CList *list, void *element);

/* func_002836E4, inlined: takes a free emitter. */
static __inline__ Emitter *TakeEmitter(EmitterOwner *owner) {
    Emitter *e = owner->freeList.m_pHead;

    if (e != NULL) {
        CList__Remove(&owner->freeList, e);
        CList__AddTail(&owner->activeList, e);
    }
    return e;
}

/* func_00283730, inlined: gives an emitter back. */
static __inline__ void FreeEmitter(EmitterOwner *owner, Emitter *e) {
    CList__Remove(&owner->activeList, e);
    CList__AddHead(&owner->freeList, e);
}

/* func_002834CC, inlined. */
static __inline__ void InitEmitter(Emitter *e, EmitterDef *def, s16 mask, f32 scale,
                                   f32 offset0, f32 offset1, f32 offset2) {
    e->def = def;
    e->mask = mask;
    func_00207914(&e->resource, def->id);
    e->angle0 = 0;
    e->angle1 = 0;
    e->throttle = 0;
    e->phase = 0;
    e->phaseScale = scale;
    e->offset0 = offset0;
    e->trim = offset1;
    e->offset2 = offset2;
}

/* func_00283550, inlined: returns 1 once the emitter has wound down. */
static __inline__ s32 UpdateEmitter(Emitter *e, s32 mask, f32 fallback, s32 boosting, f32 boost, Host *host) {
    EmitterDef *def = e->def;
    f32 x;
    f32 step;
    f32 t;
    f32 wrap;

    if (e->mask & mask) {
        f32 up = e->throttle;
        up += def->throttleRate * D_800B6D28;
        if (1.0f < up) {
            up = 1.0f;
        }
        e->throttle = up;
    } else {
        f32 dn;
        if (e->throttle == 0.0f) {
            return 1;
        }
        dn = e->throttle - def->throttleRate * D_800B6D28;
        dn = (dn < 0.0f) ? 0.0f : dn;
        e->throttle = dn;
    }
    e->phase = e->phase + D_800B6D28 * def->phaseRate * e->phaseScale;
    step = D_800B6D28 * D_800B6D34 * def->spinRate;
    t = e->angle0 + step * 1.536f;
    wrap = 20480.0f;
    e->angle0 = t;
    if (wrap < t) {
        e->angle0 = t - wrap;
    }
    t = e->angle1 + step * 3.072f;
    e->angle1 = t;
    if (wrap < t) {
        e->angle1 = t - wrap;
    }
    x = def->baseSpeed + e->trim;
    if (boosting != 0 && host->stalled == 0) {
        x += boost;
    } else {
        u8 b = def->flags;
        if (b & 1) {
            x += host->speed * def->hostScale;
        } else if (b & 2) {
            x += fallback;
        }
    }
    e->speed = x;
    return 0;
}

/* Starts the emitters the host's region asks for (one per bit of its mask,
 * from the table at D_8010FF14) and updates the running ones, giving back
 * those that have wound down. Under a region flagged 1, the emitters get a
 * boost while the camera is below the region's height. */
void func_00282E4C(EmitterOwner *owner, Host *host) {
    RegionInfo *info;
    EmitterDef *def;
    Emitter *e;
    Emitter *next;
    s32 region;
    s32 n;
    s32 bit;
    s32 mask;
    s32 below;
    f32 ground;
    f32 boost;
    f32 depth;

    region = host->region;
    if (region == 0) {
        return;
    }
    info = func_0022425C(D_800F7078, region);
    if (info == NULL) {
        return;
    }
    boost = 0.0f;
    ground = func_0026BE60(region, host->x, host->z);
    below = 0;
    if (info->flags & 1) {
        depth = MIN((info->height - func_00280E40(host).y) * 0.5f, 20.48f);
        below = func_00280E40(host).y < info->height;
        boost = info->height - depth;
    }
    def = D_8010FF14;
    mask = info->emitterMask;
    bit = 1;
    n = D_8010FF18;
    while (--n != -1) {
        if ((mask & bit) && def->id != 0 && !(owner->mask & bit)) {
            e = TakeEmitter(owner);
            if (e != NULL) {
                InitEmitter(e, def, bit, 1.0f, 1.0f, 0.0f, 1.0f);
                owner->mask |= bit;
            }
        }
        def++;
        bit <<= 1;
    }
    e = owner->activeList.m_pHead;
    while (e != NULL) {
        next = e->next;
        if (UpdateEmitter(e, mask, ground, below, boost, host)) {
            owner->mask &= ~e->mask;
            FreeEmitter(owner, e);
        }
        e = next;
    }
}
