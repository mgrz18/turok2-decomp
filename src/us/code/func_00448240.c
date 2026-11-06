#include "common.h"
#include "rw/abu_types.h"
#include "gfx.h"
#include "structs/vector.h"
#include "structs/world.h"
#include "structs/game_object.h"
#include "structs/cinema.h"
#include "structs/collision.h"
#include "structs/camera.h"

/* The second VM module's copy of src/us/code/Cinema_SplineSecondDerivs.c, byte for byte,
 * at the same link address (named 0x448240-0x44C228, linked 0x40F240-0x413228):
 * the same source, with the functions under their second-module names. Its
 * .rodata goes NOLOAD over the first module's at 0x800C0950
 * (versions/shared_rodata.us.txt). */

/* The cinema system: one file from Cinema_SplineSecondDerivs_2 to Cinema_IsWaitCue_2 (0x40F240-0x413228).
 * Its strings ("Cinema temp alloc", "Cinema") are shared by functions all over
 * the block, which GCC does only within one file, and its .rodata is the ROM's
 * 0x198 bytes at 0x800C0950. The second VM module has a copy of the block at the
 * same link address (Cinema_SplineEval_2, Cinema_DrawLetterbox_2, ...) that reads this pool. */

/* A spline record: a point, the second derivatives of its coordinates over the
 * arc length, the arc length, the weight (time) and the second derivative of the
 * arc length over the weight. */
typedef struct {
    /* 0x00 */ CVector3 p;
    /* 0x0C */ CVector3 d2p;
    /* 0x18 */ f32 s;
    /* 0x1C */ f32 w;
    /* 0x20 */ f32 d2s;
} SplineNode; /* 0x24 bytes */

typedef struct {
    /* 0x00 */ s32 nodeSize;
    /* 0x04 */ s32 count;
    /* 0x08 */ SplineNode nodes[1];
} Spline;

/* A track header: its stride and record count, the records after it. */
typedef struct {
    /* 0x00 */ s32 stride;
    /* 0x04 */ s32 count;
} Track;

/* A level object a cinema cue refers to. */
typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ u16 id4;
    /* 0x06 */ u8 pad6[0xA - 0x6];
    /* 0x0A */ u16 idA;
    /* 0x0C */ u8 padC[0xE - 0xC];
    /* 0x0E */ u8 unkE;
    /* 0x0F */ u8 padF[0x11 - 0xF];
    /* 0x11 */ u8 kind;
    /* 0x12 */ u8 val;
} CueObject;

extern Cinema *D_800C1BB0;
extern s32 D_800C1BB8;
extern s32 D_800C1BC4;
extern f32 D_800C1BC8;
extern s32 D_800C2030;
extern s32 D_800C2034;
extern Gfx *D_800D8E24;
/* An array to GCC: a function that reads two of its fields keeps its address in one register. */
extern CScene gScene[];
extern s32 D_800F709C;  /* gScene.trackLookup */
extern s32 *D_800F70D8; /* gScene.trackBundle */
extern CGameObjectInstance *D_80119890; /* current player */

extern void *CISet__GetBlock(void *node, s32 index); /* child `index` of a resource node */
extern s32 CISet__GetROMAddressAndSize(s32 *bundle, s32 lookup, s32 index, s32 *size);
extern s32 **CCache__Request(s32 a, s32 data, s32 size, s32 tag, s32 e, void *prepare, const char *name, s32 h);
extern s32 **CCache__Allocate(s32 heap, s32 size, s32 tag, const char *name);
extern void CCache__Unlock(s32 unused, s32 **handle);
extern void CCache__Deallocate(s32 heap, s32 *block);
extern void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = a + b */
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);  /* out = b - a */
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale); /* out = in * scale */
extern f32 CVector3__GetYaw(CVector3 *v);                       /* heading */
extern f32 CVector3__GetPitch(CVector3 *v);                       /* pitch */

CVector3 Cinema_SplineEval_2(SplineNode *nodes, s32 count, f32 t);
void Cinema_RequestForObject_2(void *owner, CueObject *obj);
s32 Cinema_RequestById_2(s32 key);
s32 Cinema_GetClipCount_2(void);
Spline *Cinema_GetClipEyeSpline_2(s32 index);
void *Cinema_GetFadeTrack_2(void);

/* Allocates a temporary array; the block's first word points back at the handle. */
static __inline__ f32 *TempAlloc(s32 size) {
    s32 **handle;
    s32 *block;

    handle = CCache__Allocate(0, size + 0x10, 0x13, "Cinema temp alloc");
    if (handle != NULL) {
        block = *handle;
        *block = (s32) handle;
        return (f32 *) (block + 4);
    }
    return NULL;
}

static __inline__ void TempFree(f32 *data) {
    if (data != NULL) {
        CCache__Deallocate(0, ((s32 **) data)[-4]);
    }
}

/* Whether a cue's object is one the cinema waits for (a separate function in
 * Rage Wars). Inline but not static: GCC inlines it where it is called and
 * emits its own copy at the end of the file, Cinema_IsWaitCue_2. */
__inline__ s32 Cinema_IsWaitCue_2(CueObject *k) {
    s32 r = 0;
    switch (k->kind) {
    case 1:
        if (k->val == 5) r = 1;
        break;
    case 9:
        if (k->val == 4) r = 1;
        break;
    case 10:
        if (k->val == 3) r = 1;
        break;
    default:
        r = 0;
        break;
    }
    return r;
}

/* Computes the second derivatives of a natural cubic spline through n points (ys over xs) into out. */
void Cinema_SplineSecondDerivs_2(s32 n, f32 *ys, f32 *xs, f32 *out) {
    f32 *diagonal;
    f32 *rhs;
    f32 *width;
    s32 singular;
    s32 i;

    singular = 0;
    if (n == 1) {
        out[0] = 0;
        return;
    }
    diagonal = TempAlloc(n * 4);
    rhs = TempAlloc(n * 4);
    width = TempAlloc(n * 4);
    for (i = 1; i < n - 1; i++) {
        diagonal[i] = 2.0f * (xs[i + 1] - xs[i - 1]);
        if (diagonal[i] == 0.0f) {
            diagonal[i] = 1.0f;
            singular = 1;
        }
    }
    for (i = 0; i < n - 1; i++) {
        width[i] = xs[i + 1] - xs[i];
        if (width[i] == 0.0f) {
            width[i] = 1.0f;
            singular = 1;
        }
    }
    for (i = 1; i < n - 1; i++) {
        rhs[i] = ((ys[i + 1] - ys[i]) / width[i] - (ys[i] - ys[i - 1]) / width[i - 1]) * 6.0f;
    }
    for (i = 1; i < n - 2; i++) {
        rhs[i + 1] -= rhs[i] * width[i] / diagonal[i];
        diagonal[i + 1] -= width[i] * width[i] / diagonal[i];
    }
    for (i = 0; i < n; i++) {
        out[i] = 0;
    }
    if (!singular) {
        for (i = n - 2; i > 0; i--) {
            out[i] = (rhs[i] - width[i] * out[i + 1]) / diagonal[i];
        }
    }
    TempFree(diagonal);
    TempFree(rhs);
    TempFree(width);
}

extern f32 func_00298470(f32 x); /* sqrt */

/* Builds a spline through count points: arc lengths, then second derivatives of x, y and z over the arc length
 * and of the arc length over the weights. */
void Cinema_SplineBuild_2(s32 count, CVector3 *points, f32 *weights, Spline *spline) {
    CVector3 delta;
    f32 *ys;
    f32 *xs;
    f32 *d2;
    SplineNode *nodes;
    f32 length;
    s32 i;

    length = 0.0f;
    ys = TempAlloc(count * 4);
    xs = TempAlloc(count * 4);
    d2 = TempAlloc(count * 4);
    nodes = spline->nodes;
    spline->nodeSize = 0x24;
    spline->count = count;
    if (count == 1) {
        spline->nodes[0].p.x = points->x;
        nodes->p.y = points->y;
        nodes->p.z = points->z;
        nodes->d2p.x = 0.0f;
        nodes->d2p.y = 0.0f;
        nodes->d2p.z = 0.0f;
        nodes->d2s = 0.0f;
        nodes->w = 0.0f;
        return;
    }
    for (i = 0; i < count; i++) {
        nodes[i].p.x = points[i].x;
        nodes[i].p.y = points[i].y;
        nodes[i].p.z = points[i].z;
        nodes[i].w = weights[i];
    }
    for (i = 0; i < count; i++) {
        nodes[i].s = length;
        if (i == count - 1) {
            break;
        }
        CVector3__Subtract(&delta, &nodes[i].p, &nodes[i + 1].p);
        length += func_00298470(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
    }
    for (i = 0; i < count; i++) {
        ys[i] = nodes[i].p.x;
        xs[i] = nodes[i].s;
    }
    Cinema_SplineSecondDerivs_2(count, ys, xs, d2);
    for (i = 0; i < count; i++) {
        nodes[i].d2p.x = d2[i];
    }
    for (i = 0; i < count; i++) {
        ys[i] = nodes[i].p.y;
        xs[i] = nodes[i].s;
    }
    Cinema_SplineSecondDerivs_2(count, ys, xs, d2);
    for (i = 0; i < count; i++) {
        nodes[i].d2p.y = d2[i];
    }
    for (i = 0; i < count; i++) {
        ys[i] = nodes[i].p.z;
        xs[i] = nodes[i].s;
    }
    Cinema_SplineSecondDerivs_2(count, ys, xs, d2);
    for (i = 0; i < count; i++) {
        nodes[i].d2p.z = d2[i];
    }
    for (i = 0; i < count; i++) {
        ys[i] = nodes[i].s;
        xs[i] = nodes[i].w;
    }
    Cinema_SplineSecondDerivs_2(count, ys, xs, d2);
    for (i = 0; i < count; i++) {
        nodes[i].d2s = d2[i];
    }
    TempFree(ys);
    TempFree(xs);
    TempFree(d2);
}

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU_2/80400000_4.CPP, func_8040067C_de. */
typedef struct SplineSource { s32 stride,count; union { char data[0]; CVector3 points[0]; f32 weights[0]; }; } SplineSource;
typedef struct SplineTable { s32 count; } SplineTable;
typedef union SplineSourceCursor { SplineSource *header; CVector3 *points; f32 *weights; } SplineSourceCursor;
extern int CUSet__GetTotalSize(int *);
extern void Pack_Init(void *,s32);
extern char *Pack_Add(Pack *,s32,s32);
extern s32 ProPackDecompress(void **, s32);
extern struct Shape_typemap_165 *CCache__AllocateMemoryForDecompression(s32,void *,s32);
extern void CCache__DeallocAndReplaceCompressedData(s32,struct Shape_typemap_165 **,struct Shape_typemap_165 *);
static __inline__ char *packAppend(Pack *pack,void *prepare,s32 size) { return Pack_Add(pack,(s32)prepare,size); }
static __inline__ s32 splineRecordSize(s32 left,s32 right) { s32 a=left*9,b=right*9; return (b+a)*4+48; }
void Cinema_PrepareResource_2(s32 unused, struct Shape_typemap_165 **handle) {
    struct Shape_typemap_165 *output;
    void *source, *a, *b, *c, *part, *extra;
    Pack *pack, *dest;
    SplineTable *parts;
    Pack *table;
    SplineSourceCursor p,q,r,s;
    s32 count, i, size, sa, sb, sc, np, nr, byteP, byteR;
    void *dp, *dr;
    if (!ProPackDecompress((void **)handle, 1)) return;
    source = (*handle)->pack;
    parts = CISet__GetBlock(source, 0);
    a = CISet__GetBlock(source, 1);
    b = CISet__GetBlock(source, 2);
    c = CISet__GetBlock(source, 3);
    count = parts->count;
    size = (count * 4 + 15) & ~7;
    for (i = 0; i < parts->count; i++) {
        SplineSource *firstP,*firstR;
        void *firstPart;
        s32 byteP,byteR;
        firstPart = CISet__GetBlock(parts, i);
        firstP = CISet__GetBlock(firstPart, 0);
        firstR = CISet__GetBlock(firstPart, 2);
        size += splineRecordSize(firstP->count,firstR->count);
    }
    sc = CUSet__GetTotalSize(c);
    sa = CUSet__GetTotalSize(a);
    sb = CUSet__GetTotalSize(b);
    output = CCache__AllocateMemoryForDecompression(0, handle, size + sa + sb + sc + 24);
    if (output == 0) return;
    pack = output->pack;
    Pack_Init(pack, 4);
    table = (void *)packAppend(pack, 0, size);
    packAppend(pack, a, sa);
    packAppend(pack, b, sb);
    packAppend(pack, c, sc);
    Pack_Init(table, count);
    for (i = 0; i < count; i++) {
        s32 byteP,byteR;
        SplineSource *sizeP,*sizeR;
        part = CISet__GetBlock(parts, i);
        sizeP = CISet__GetBlock(part, 0);
        sizeR = CISet__GetBlock(part, 2);
        byteP = sizeP->count * 9;
        byteR = sizeR->count * 9;
        dest = (void *)packAppend(table, 0, (byteR + byteP)*4 + 48);
        Pack_Init(dest, 3);
        p.header = CISet__GetBlock(part, 0);
        q.header = CISet__GetBlock(part, 1);
        r.header = CISet__GetBlock(part, 2);
        s.header = CISet__GetBlock(part, 3);
        extra = CISet__GetBlock(part, 4);
        np = p.header->count;
        nr = r.header->count;
        { s32 pointBytes,otherBytes;
        pointBytes = np * 36 + 8;
        otherBytes = nr * 36 + 8;
        dp = packAppend(dest, 0, pointBytes);
        dr = packAppend(dest, 0, otherBytes);
        }
        p.points = p.header->points;
        q.weights = q.header->weights;
        r.points = r.header->points;
        s.weights = s.header->weights;
        packAppend(dest, extra, 8);
        Cinema_SplineBuild_2(np, p.points, q.weights, dp);
        Cinema_SplineBuild_2(nr, r.points, s.weights, dr);
    }
    CCache__DeallocAndReplaceCompressedData(0, handle, output);
}

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ f32 time;
} Cue; /* 0x14 bytes */

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 count;
    /* 0x08 */ Cue cues[1];
} CueList;

extern void Cinema_Reset(void);

static __inline__ void minTime(f32 *dest, f32 *src) {
    if (*dest > *src) {
        *dest = *src;
    }
}

static __inline__ void maxTime(f32 *dest, f32 *src) {
    if (*dest < *src) {
        *dest = *src;
    }
}

static __inline__ void setTimes(f32 *a, f32 *b, f32 *c, f32 *t) {
    *a = *t;
    *b = *t;
    *c = *t;
}

/* Loads cinema `index`: its track, its time range from the clips and cues, and its entry in the cinema table. */
s32 Cinema_Load_2(s32 index) {
    s32 size;
    s32 size2;
    s32 i;
    s32 count;
    s32 previous;
    s32 pending;
    s32 loaded;
    CGameObjectInstance *captured;
    CScene *resources;
    s32 **owner;
    s32 **table;
    CueList *cues;
    Cue *cue;
    Spline *clip;
    Cinema *context;
    Cinema *scanContext;
    f32 total;

    captured = D_80119890;
    pending = D_800C1BB0->resetPlayerNext;
    resources = gScene;
    if (resources->m_CinemasKey == 0 || resources->m_pCinemas == 0) {
    failed:
        return 0;
    } else {
        if (D_800C1BB0->active != 0) {
            Cinema_Reset();
        }
        loaded = CISet__GetROMAddressAndSize(resources->m_pCinemas, resources->m_CinemasKey, index + 1, &size);
        owner = CCache__Request(0, loaded, size, 0x13, 0, Cinema_PrepareResource_2, "Cinema", 1);
        if (owner == NULL) {
            goto failed;
        }
        D_800C1BB0->owner = owner;
        D_800C1BB0->resource = *owner;
        cues = Cinema_GetFadeTrack_2();
        cue = cues->cues;
        count = cues->count;
        total = 0.0f;
        for (i = 0; i < Cinema_GetClipCount_2(); i++) {
            clip = Cinema_GetClipEyeSpline_2(i);
            {
                s32 n = clip->count;
                SplineNode *base = clip->nodes;
                if (n > 0) {
                    total += base[n - 1].w;
                }
            }
        }
        D_800C1BB0->startTime = 0.0f;
        D_800C1BB0->endTime = total;
        D_800C1BB0->clipStart = 0.0f;
        D_800C1BB0->clipEnd = total;
        i = 0;
        if (count > 0) {
            scanContext = D_800C1BB0;
            do {
                Cue *c = &cue[i];
                minTime(&scanContext->clipStart, &c->time);
                maxTime(&scanContext->clipEnd, &c->time);
                i++;
            } while (i < count);
        }
        setTimes(&D_800C1BB0->time, &D_800C1BB0->prevTime, &D_800C1BB0->time24, &D_800C1BB0->clipStart);
        previous = D_800C1BB0->spec.id;
        loaded = CISet__GetROMAddressAndSize(D_800F70D8, D_800F709C, 0, &size2);
        if (loaded == 0) {
            table = NULL;
        } else {
            table = CCache__Request(0, loaded, size2, 0x13, 0, NULL, "Cinema", 1);
        }
        {
            CinemaTable *entries = (CinemaTable *) *table;
            CinemaEntry *base = entries->entries;
            D_800C1BB0->resetPlayer = pending;
            D_800C1BB0->active = 1;
            D_800C1BB0->spec = base[index];
        }
        D_800C1BB0->unk50 = 0;
        D_800C1BB0->started = 0;
        D_800C1BB0->ended = 0;
        D_800C1BB0->closed = 0;
        D_800C1BB0->scriptId = D_800C1BB0->spec.id;
        if ((D_800C1BB0->spec.flags & 1) && D_800C1BB0->target != NULL) {
            D_800C1BB0->position = D_800C1BB0->target->header.m_vPos;
            D_800C1BB0->yaw = D_800C1BB0->target->m_RotY + 1.5707964f;
        }
        if (D_800C1BB0->spec.flags & 2) {
            if (captured != NULL) {
                D_800C1BB0->position = captured->header.m_vPos;
                D_800C1BB0->yaw = captured->m_RotY;
            } else {
                D_800C1BB0->position.x = 0.0f;
                D_800C1BB0->position.y = 0.0f;
                D_800C1BB0->position.z = 0.0f;
                D_800C1BB0->yaw = 0.0f;
            }
        }
        CCache__Unlock(0, table);
        if (pending) {
            void (*first)(void);
            void (*second)(void);
            context = D_800C1BB0;
            first = context->nextOnClose;
            second = context->nextOnEnd;
            context->resetPlayer = 1;
            context->restored = 0;
            context->nextOnClose = NULL;
            context->nextOnEnd = NULL;
            context->saveStart = context->spec.flags & 8;
            context->onClose = first;
            context->onEnd = second;
            if (context->scriptId - 10U < 0x47U) {
                context->saveStart = 0;
            }
            if (D_800C1BB0->scriptId - 400U < 4U) {
                D_800C1BB0->saveStart = 0;
            }
        } else {
            if (D_800C1BB0->spec.id < 3) {
                D_800C1BB0->scriptId = previous;
            }
            D_800C1BB0->resetPlayer = 0;
            D_800C1BB0->time = D_800C1BB0->startTime;
        }
        if (D_800C1BB0->spec.id == 0x9F) {
            D_800C1BB0->saveStart = 0;
        }
        return 1;
    }
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00400E50.c. */
CVector3 Cinema_SplineEval_2(SplineNode *rec, s32 n, f32 t) {
    CVector3 res;
    SplineNode *p = 0;
    SplineNode *q = 0;
    s32 i;
    f32 d0;
    f32 d1;
    f32 d2;
    f32 m0;
    f32 m1;
    f32 A;
    f32 B;
    f32 u;
    f32 s;
    f32 h2;
    f32 w;
    f32 h;
    f32 pa;
    f32 qa;
    if (n == 0) {
        res.x = 0;
        res.y = 0;
        res.z = 0;
    } else if (n == 1 || t <= rec[0].w) {
        res.x = rec[0].p.x;
        res.y = rec[0].p.y;
        res.z = rec[0].p.z;
    } else if (rec[n - 1].w <= t) {
        res.x = rec[n - 1].p.x;
        res.y = rec[n - 1].p.y;
        res.z = rec[n - 1].p.z;
    } else {
        for (i = 0; i < n - 1; i++) {
            p = &rec[i];
            q = &rec[i + 1];
            if (p->w <= t && t < q->w) {
                break;
            }
        }
        d0 = 0.0f;
        d1 = 0.0f;
        d2 = 0.0f;
        if (i > 0) {
            d0 = rec[i].s - rec[i - 1].s;
        }
        if (i < n - 1) {
            d1 = rec[i + 1].s - rec[i].s;
        }
        if (i < n - 2) {
            d2 = rec[i + 2].s - rec[i + 1].s;
        }
        m0 = d1;
        if (!(m0 <= d0)) {
            m0 = d0;
        }
        m1 = d2;
        if (!(m1 <= d1)) {
            m1 = d1;
        }
        A = p->s;
        B = q->s;
        if (p->w != q->w) {
            u = (t - p->w) / (q->w - p->w);
        } else {
            u = 0.0f;
        }
        s = A * (1.0f - u) + B * u + (m0 - (B - A)) * (u - 2.0f * u * u + u * u * u) + (m1 - (B - A)) * (u * u * u - u * u);
        for (i = 0; i < n - 1; i++) {
            p = &rec[i];
            q = &rec[i + 1];
            if (p->s <= s && s < q->s) {
                break;
            }
        }
        pa = p->s;
        qa = q->s;
        h = qa - pa;
        w = 0.0f;
        if (h != 0.0f) {
            w = (s - pa) / h;
        }
        h2 = h * h;
        res.x = p->p.x * (1.0f - w) + q->p.x * w + (p->d2p.x * ((1.0f - w) * (1.0f - w) * (1.0f - w) - (1.0f - w)) + q->d2p.x * (w * w * w - w)) * h2 * 0.16666667f;
        res.y = p->p.y * (1.0f - w) + q->p.y * w + (p->d2p.y * ((1.0f - w) * (1.0f - w) * (1.0f - w) - (1.0f - w)) + q->d2p.y * (w * w * w - w)) * h2 * 0.16666667f;
        res.z = p->p.z * (1.0f - w) + q->p.z * w + (p->d2p.z * ((1.0f - w) * (1.0f - w) * (1.0f - w) - (1.0f - w)) + q->d2p.z * (w * w * w - w)) * h2 * 0.16666667f;
    }
    return res;
}

CVector3 Cinema_GetEyePos_2(f32 t) {
    s32 i;
    s32 idx;
    s32 count;
    SplineNode *keys;
    s32 *clip;
    f32 start;
    f32 end;
    start = 0.0f;
    count = 0;
    keys = 0;
    idx = 0;
    i = 0;
    end = start;
    while (i < *(s32 *)CISet__GetBlock(D_800C1BB0->resource, 0)) {
        idx = i;
        clip = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(D_800C1BB0->resource, 0), idx), 0);
        count = clip[1];
        keys = (SplineNode *)(clip + 2);
        start = end;
        end = start + keys[count - 1].w;
        if (idx == 0 && t <= start) {
            break;
        }
        if (idx == *(s32 *)CISet__GetBlock(D_800C1BB0->resource, 0) - 1 && end < t) {
            break;
        }
        i = idx + 1;
        if (start <= t && t <= end) {
            break;
        }
    }
    if (*(s32 *)CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(D_800C1BB0->resource, 0), idx), 2) != 0) {
        t = start;
    }
    return Cinema_SplineEval_2(keys, count, t - start);
}

CVector3 Cinema_GetTargetPos_2(f32 t) {
    s32 i;
    s32 idx;
    s32 count;
    SplineNode *keys;
    s32 *clip;
    f32 start;
    f32 end;
    start = 0.0f;
    count = 0;
    keys = 0;
    idx = 0;
    i = 0;
    end = start;
    while (i < *(s32 *)CISet__GetBlock(D_800C1BB0->resource, 0)) {
        idx = i;
        clip = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(D_800C1BB0->resource, 0), idx), 1);
        count = clip[1];
        keys = (SplineNode *)(clip + 2);
        start = end;
        end = start + keys[count - 1].w;
        if (idx == 0 && t <= start) {
            break;
        }
        if (idx == *(s32 *)CISet__GetBlock(D_800C1BB0->resource, 0) - 1 && end < t) {
            break;
        }
        i = idx + 1;
        if (start <= t && t <= end) {
            break;
        }
    }
    if (((s32 *)CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(D_800C1BB0->resource, 0), idx), 2))[1] != 0) {
        t = start;
    }
    return Cinema_SplineEval_2(keys, count, t - start);
}

CVector3 Cinema_GetFadeColor_2(f32 t) {
    s32 *track;
    PositionKey *key;
    s32 count;
    f32 u;
    CVector3 result;
    CVector3 a;
    CVector3 b;
    track = CISet__GetBlock(D_800C1BB0->resource, 1);
    count = track[1];
    key = (PositionKey *)(track + 2);
    if (count == 0) {
        result.x = 0.0f;
        result.y = 0.0f;
        result.z = 0.0f;
        return result;
    } else if (t <= key[0].time) {
        result.x = key[0].x;
        result.y = key[0].y;
        result.z = key[0].z;
        return result;
    } else if (key[count - 1].time <= t) {
        result.x = key[count - 1].x;
        result.y = key[count - 1].y;
        result.z = key[count - 1].z;
        return result;
    } else {
        while (key->time < t) {
            key++;
        }
        u = (t - key[-1].time) / (key->time - key[-1].time);
        a.x = key[-1].x;
        a.y = key[-1].y;
        a.z = key[-1].z;
        b.x = key->x;
        b.y = key->y;
        b.z = key->z;
        CVector3__Scale(&a, &a, (1.0f) - u);
        CVector3__Scale(&b, &b, u);
        CVector3__Add(&result, &a, &b);
    }
    return result;
}

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ f32 value;
    /* 0x10 */ f32 time;
} ScalarKey; /* 0x14 bytes */

/* Evaluates the scalar key track 1 of the current cinema at time t, then rescales and clamps it to [0, 1]. */
f32 Cinema_GetFadeAlpha_2(f32 t) {
    s32 *track;
    ScalarKey *key;
    s32 count;
    f32 u;
    f32 v;

    track = CISet__GetBlock(D_800C1BB0->resource, 1);
    count = track[1];
    key = (ScalarKey *) (track + 2);
    if (count == 0) {
        v = 0.0f;
    } else if (t <= key[0].time) {
        v = key[0].value;
    } else if (key[count - 1].time <= t) {
        v = key[count - 1].value;
    } else {
        while (key->time < t) {
            key++;
        }
        u = (t - key[-1].time) / (key->time - key[-1].time);
        v = key[-1].value * (1.0f - u) + key->value * u;
    }
    v *= 2.0f * D_800C1BC8 + 1.0f;
    v -= D_800C1BC8;
    if (1.0f < v) {
        v = 1.0f;
    }
    if (v < 0.0f) {
        v = 0.0f;
    }
    return v;
}

f32 Cinema_GetFOV_2(f32 t) {
    s32 *track;
    D_800C7470_Pair *key;
    s32 count;
    s32 i;
    f32 u;
    track = CISet__GetBlock(D_800C1BB0->resource, 3);
    key = (D_800C7470_Pair *)(track + 2);
    count = track[1];
    for (i = 0; i < count; i++) {
    }
    if (count == 0) {
        return 47.5f;
    }
    if (t <= key[0].second) {
        return key[0].first;
    }
    if (key[count - 1].second <= t) {
        return key[count - 1].first;
    }
    while (key->second < t) {
        key++;
    }
    u = (t - key[-1].second) / (key->second - key[-1].second);
    u = u * (u * 3.0f) - 2.0f * u * u * u;
    return key[-1].first * (1.0f - u) + key->first * u;
}

extern u8 D_800F1B40[];   /* collision info */
extern CCollisionResults gCollisionResults;  /* the last collision test */

extern s32 Cinema_PathPlaying(void);
extern CVector3 Cinema_GetEyePos_2(f32 t); /* eye path */
extern CVector3 Cinema_GetTargetPos_2(f32 t); /* target path */
extern void CMtxF__Identity(CMtxF *m);                         /* identity */
extern void CMtxF__SetEulerXYZ(CMtxF *m, f32 x, f32 y, f32 z);    /* rotation */
extern void CMtxF__TransformPoint(CMtxF *m, CVector3 *in, CVector3 *out);    /* transform */
extern void CScene__LoadVisibleGridSections(void *world);
extern s32 Collision_MoveInstance(CGameObjectInstance *actor, CVector3 to, void *info);
extern void CScene__ReleaseSectors(void *world);

/* Places the cinema relative to its target (flag 1) or to the player (flag 2). */
static __inline__ void Cinema_Anchor(CGameObjectInstance *player) {
    if ((D_800C1BB0->spec.flags & 1) && D_800C1BB0->target != NULL) {
        D_800C1BB0->position = D_800C1BB0->target->header.m_vPos;
        D_800C1BB0->yaw = D_800C1BB0->target->m_RotY + 1.5707964f;
    }
    if (D_800C1BB0->spec.flags & 2) {
        if (player != NULL) {
            D_800C1BB0->position = player->header.m_vPos;
            D_800C1BB0->yaw = player->m_RotY;
        } else {
            D_800C1BB0->position.x = 0.0f;
            D_800C1BB0->position.y = 0.0f;
            D_800C1BB0->position.z = 0.0f;
            D_800C1BB0->yaw = 0.0f;
        }
    }
}

/* Drives the camera along the cinema paths. */
void Cinema_UpdateCamera_2(CCamera *cam) {
    CVector3 eye;
    CVector3 target;
    CVector3 dir;
    CObjectHead saved;
    CMtxF mtx;
    CVector3 tmp;
    CGameObjectInstance *player;
    s32 hit;
    f32 yaw;

    if (!D_800C1BB0->active || !Cinema_PathPlaying()) {
        return;
    }
    if (D_800C1BB0->time < D_800C1BB0->startTime || D_800C1BB0->endTime < D_800C1BB0->time) {
        return;
    }
    eye = Cinema_GetEyePos_2(D_800C1BB0->time);
    target = Cinema_GetTargetPos_2(D_800C1BB0->time);
    CVector3__Scale(&eye, &eye, 10.24f);
    CVector3__Scale(&target, &target, 10.24f);
    cam->m_ViewParams.m_RotY = 0.0f;
    CVector3__Subtract(&dir, &target, &eye);
    if (((D_800C1BB0->spec.flags & 1) && D_800C1BB0->target != NULL) || (D_800C1BB0->spec.flags & 2)) {
        player = D_80119890;
        Cinema_Anchor(player);
        CMtxF__Identity(&mtx);
        CMtxF__SetEulerXYZ(&mtx, 0.0f, D_800C1BB0->yaw, 0.0f);
        CMtxF__TransformPoint(&mtx, &target, &tmp);
        target = tmp;
        CMtxF__TransformPoint(&mtx, &eye, &tmp);
        eye = tmp;
        CVector3__Add(&target, &target, &D_800C1BB0->position);
        CVector3__Add(&eye, &eye, &D_800C1BB0->position);
        CVector3__Subtract(&dir, &target, &eye);
        if (player != NULL) {
            saved = *(CObjectHead *) player;
            player->header.m_vPos = target;
            CScene__LoadVisibleGridSections(gScene);
            hit = Collision_MoveInstance(player, eye, D_800F1B40);
            CScene__ReleaseSectors(gScene);
            *(CObjectHead *) player = saved;
            if (hit) {
                eye = gCollisionResults.m_vInstancePos;
            }
        }
    }
    yaw = CVector3__GetYaw(&dir);
    cam->m_ViewParams.m_vRotOffset.x = -CVector3__GetPitch(&dir);
    cam->m_ViewParams.m_RotY += yaw;
    cam->m_ViewParams.m_vPos = eye;
}

extern void Render_InvalidateModeCache(void);

void Cinema_DrawFrameEdges_2(s32 inset) {
    f32 left;
    f32 top;
    f32 width;
    f32 height;
    Render_InvalidateModeCache();
    left = 0.0f;
    top = (f32)inset;
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (20) - (2)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((2) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((unsigned int)((((unsigned int)(0x0)) << 20))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xE2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (3) - (29)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((29) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((unsigned int)((( (((unsigned int)(0x1)) << (3 + 3)) | (((unsigned int)(0x2)) << (3 + 5)) | (((unsigned int)(0x0)) << (3 + 7)) | (((unsigned int)(0x1)) << (3 + 11)) | ( (((unsigned int)(((unsigned int)(0x0))) & (0xFFFFFFFFU >> (32 - (2)))) << (30)) | (((unsigned int)(((unsigned int)(0x0))) & (0xFFFFFFFFU >> (32 - (2)))) << (26)) | (((unsigned int)(((unsigned int)(0x1))) & (0xFFFFFFFFU >> (32 - (2)))) << (22)) | (((unsigned int)(((unsigned int)(0x0))) & (0xFFFFFFFFU >> (32 - (2)))) << (18)) ) )) | (( (((unsigned int)(0x1)) << (3 + 3)) | (((unsigned int)(0x2)) << (3 + 5)) | (((unsigned int)(0x0)) << (3 + 7)) | (((unsigned int)(0x1)) << (3 + 11)) | ( (((unsigned int)(((unsigned int)(0x0))) & (0xFFFFFFFFU >> (32 - (2)))) << (28)) | (((unsigned int)(((unsigned int)(0x0))) & (0xFFFFFFFFU >> (32 - (2)))) << (24)) | (((unsigned int)(((unsigned int)(0x1))) & (0xFFFFFFFFU >> (32 - (2)))) << (20)) | (((unsigned int)(((unsigned int)(0x0))) & (0xFFFFFFFFU >> (32 - (2)))) << (16)) ) )))); };
    width = (f32)D_800C2030;
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xFA) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); };
    height = (f32)(D_800C2034 - inset * 2);
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)((width - 1.0f)) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)((top + (2.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->w1 = ((((unsigned int)(left) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(top) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(((left + width) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((top + height) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->w1 = ((((unsigned int)(left) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((top + height) - (2.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)((left + (2.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((top + height) - 1.0f)) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->w1 = ((((unsigned int)(left) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(top) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(((left + width) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((top + height) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->w1 = ((((unsigned int)(((left + width) - (2.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(top) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xE7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = (0); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xE3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (20) - (2)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((2) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((unsigned int)((((unsigned int)(0x3)) << 20))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xFC) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (20)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (5)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (5)))) << (0))); _gbi->w1 = ((((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (28)) | (((unsigned int)(31) & (0xFFFFFFFFU >> (32 - (4)))) << (24)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (21)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (18)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (15)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (12)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (9)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (6)) | (((unsigned int)(7) & (0xFFFFFFFFU >> (32 - (3)))) << (3)) | (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (3)))) << (0))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xE2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(32 - (3) - (29)) & (0xFFFFFFFFU >> (32 - (8)))) << (8)) | (((unsigned int)((29) - 1) & (0xFFFFFFFFU >> (32 - (8)))) << (0))); _gbi->w1 = ((unsigned int)((0) | (0))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xF7) & (0xFFFFFFFFU >> (32 - (8)))) << (24))); _gbi->w1 = ((unsigned int)(0x10001)); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(((left + width) - 1.0f)) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(top) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->w1 = ((((unsigned int)(left) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(top) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(((left + width) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((top + height) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->w1 = ((((unsigned int)(left) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((top + height) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(left) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((top + height) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->w1 = ((((unsigned int)(left) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(top) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
    { Gfx *_gbi = (Gfx *)(D_800D8E24++); _gbi->w0 = ((((unsigned int)(0xF6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)((left + width)) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(((top + height) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); _gbi->w1 = ((((unsigned int)(((left + width) - (1.0f))) & (0xFFFFFFFFU >> (32 - (10)))) << (14)) | (((unsigned int)(top) & (0xFFFFFFFFU >> (32 - (10)))) << (2))); };
}

s32 Render_SetRenderMode(s32 a);
s32 Render_SetCombineMode(s32 a);

void Cinema_DrawLetterbox_2(void) {
    f32 fs;
    f32 k;
    f32 q;
    s32 sh;
    fs = ((f32)D_800C2034 - (f32)D_800C2030 / D_800C1BB0->spec.aspect) * 0.5f;
    q = 0.3f;
    k = 1.0f;
    if (D_800C1BB0->time < D_800C1BB0->startTime) {
        k = D_800C1BB0->startTime - D_800C1BB0->time;
        k = k / q;
        if (1.0f < k) {
            k = 1.0f;
        }
        k = 1.0f - k;
    }
    if (D_800C1BB0->time > D_800C1BB0->endTime) {
        k = D_800C1BB0->time - D_800C1BB0->endTime;
        k = k / q;
        if (1.0f < k) {
            k = 1.0f;
        }
        k = 1.0f - k;
    }
    fs = fs * k;
    { Gfx *_g = D_800D8E24++; _g->w0 = (0xE7000000); _g->w1 = (0); }
    { Gfx *_g = D_800D8E24++; _g->w0 = (0xE3000A01); _g->w1 = (0); }
    Render_SetRenderMode(0x13);
    Render_SetCombineMode(0x19);
    { Gfx *_g = D_800D8E24++; _g->w0 = (0xFA00FFFF); _g->w1 = (0xFF); }
    { Gfx *_g = D_800D8E24++; _g->w0 = (0xF6000000 | ((((D_800C2030) > (0) ? (D_800C2030) : (0)) & 0x3FF) << 14) | (((((s32)fs) > (0) ? ((s32)fs) : (0)) & 0x3FF) << 2)); _g->w1 = (0); }
    { Gfx *_g = D_800D8E24++; _g->w0 = (0xF6000000 | ((((D_800C2030) > (0) ? (D_800C2030) : (0)) & 0x3FF) << 14) | ((((D_800C2034) > (0) ? (D_800C2034) : (0)) & 0x3FF) << 2)); _g->w1 = (({ f32 _y = (f32)D_800C2034 - fs; if (!(0.0f <= _y)) _y = 0.0f; (((u32)_y) & 0x3FF) << 2; })); }
    sh = (s32)fs;
    Cinema_DrawFrameEdges_2(sh);
}

typedef struct {
    /* 0x000 */ u8 pad0[0x288];
    /* 0x288 */ f32 x;
    /* 0x28C */ f32 y;
    /* 0x290 */ f32 width;
    /* 0x294 */ f32 height;
} ScreenRectangle;

extern u8 gEngineApp[];
extern void CEngineApp__DrawFilledRect(void *ctx, u8 *rgba, s32 left, s32 top, s32 right, s32 bottom);

/* Fills the cinema's screen rectangle with the colour and alpha of the current time. */
void Cinema_DrawFade_2(ScreenRectangle *rectangle) {
    CVector3 color;
    CVector3 baseColor;
    u8 rgba[4];
    u32 width;
    u32 height;
    u32 right;
    u32 bottom;
    f32 alpha;
    f32 scale;
    f32 value;

    if (!D_800C1BB0->active) {
        if (!D_800C1BC4) {
            return;
        }
    } else {
        width = rectangle->width;
        height = rectangle->height;
        right = (f32) width + rectangle->x;
        bottom = (f32) height + rectangle->y;
        baseColor = Cinema_GetFadeColor_2(D_800C1BB0->time);
        alpha = Cinema_GetFadeAlpha_2(D_800C1BB0->time);
        color = baseColor;
        if (D_800C1BB0->fading) {
            scale = D_800C1BB0->fade;
            if (scale < 0.0f) {
                scale = 0.0f;
            }
            color.x = baseColor.x * scale;
            color.y = baseColor.y * scale;
            color.z = baseColor.z * scale;
            alpha += 1.0f - scale;
        }
        value = color.x * 255.0f;
        if (!(value <= 255.0f)) {
            value = 255.0f;
        }
        if (!(0.0f <= value)) {
            value = 0.0f;
        }
        rgba[0] = (u32) value;
        value = color.y * 255.0f;
        if (!(value <= 255.0f)) {
            value = 255.0f;
        }
        if (!(0.0f <= value)) {
            value = 0.0f;
        }
        rgba[1] = (u32) value;
        value = color.z * 255.0f;
        if (!(value <= 255.0f)) {
            value = 255.0f;
        }
        if (!(0.0f <= value)) {
            value = 0.0f;
        }
        rgba[2] = (u32) value;
        value = alpha * 255.0f;
        if (!(value <= 255.0f)) {
            value = 255.0f;
        }
        if (!(0.0f <= value)) {
            value = 0.0f;
        }
        rgba[3] = (u32) value;
        CEngineApp__DrawFilledRect(gEngineApp, rgba, width, height, right, bottom);
    }
    Cinema_DrawLetterbox_2();
}

/* Finds the last cinema entry whose ids match the key's and makes it pending, with owner as its target. */
void Cinema_RequestForObject_2(void *owner, CueObject *obj) {
    s32 size;
    s32 **table;
    s32 key;
    s32 found;
    s32 i;
    s32 n;
    CinemaEntry *entry;
    s32 id4;
    s32 idA;

    found = -1;
    if (D_800C1BB8 != 0) {
        id4 = obj->id4;
        idA = obj->idA;
        key = CISet__GetROMAddressAndSize(D_800F70D8, D_800F709C, 0, &size);
        if (key == 0) {
            table = NULL;
        } else {
            table = CCache__Request(0, key, size, 0x13, 0, NULL, "Cinema", 1);
        }
        n = *D_800F70D8 - 1;
        entry = (CinemaEntry *) (*table + 2);
        for (i = 0; i < n; i++, entry++) {
            if (entry->id4 == id4 && entry->id0 == idA) {
                found = i;
            }
        }
        if (found != -1) {
            D_800C1BB0->target = owner;
            D_800C1BB0->warpId = idA;
            D_800C1BB0->entryIndex = found;
            D_800C1BB0->resetPlayerNext = 1;
        }
        CCache__Unlock(0, table);
    }
}

/* Makes every cinema entry with id `key` pending; returns the resulting target id. */
s32 Cinema_RequestById_2(s32 key) {
    s32 size;
    s32 **table;
    s32 loaded;
    s32 i;
    s32 n;
    CinemaEntry *entries;

    if (D_800C1BB8 == 0) {
        return 0;
    }
    loaded = CISet__GetROMAddressAndSize(D_800F70D8, D_800F709C, 0, &size);
    if (loaded == 0) {
        table = NULL;
    } else {
        table = CCache__Request(0, loaded, size, 0x13, 0, NULL, "Cinema", 1);
    }
    i = 0;
    entries = (CinemaEntry *) (*table + 2);
    n = *D_800F70D8 - 1;
    for (; i < n; i++) {
        if (entries[i].id == key) {
            if ((entries[i].flags & 3) == 0) {
                D_800C1BB0->warpId = entries[i].id0;
            }
            D_800C1BB0->scriptId = key;
            D_800C1BB0->entryIndex = i;
            D_800C1BB0->resetPlayerNext = 1;
        }
    }
    CCache__Unlock(0, table);
    return D_800C1BB0->warpId;
}

extern s32 Util_StrCmp(const char *a, const char *b); /* strcmp */

/* Makes the cinema entry called `name` pending. */
void Cinema_RequestByName_2(const char *name) {
    s32 size;
    s32 **table;
    s32 loaded;
    s32 i;
    s32 id;
    s32 n;
    CinemaEntry *entries;

    if (D_800C1BB8 != 0) {
        loaded = CISet__GetROMAddressAndSize(D_800F70D8, D_800F709C, 0, &size);
        if (loaded == 0) {
            table = NULL;
        } else {
            table = CCache__Request(0, loaded, size, 0x13, 0, NULL, "Cinema", 1);
        }
        i = 0;
        entries = (CinemaEntry *) (*table + 2);
        n = *D_800F70D8 - 1;
        for (; i < n; i++) {
            if (Util_StrCmp(name, entries[i].name) == 0) {
                D_800C1BB0->scriptId = entries[i].id;
                id = entries[i].id0;
                D_800C1BB0->entryIndex = i;
                D_800C1BB0->resetPlayerNext = 1;
                D_800C1BB0->warpId = id;
                break;
            }
        }
        CCache__Unlock(0, table);
    }
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} CueRef;

s32 Cinema_NoWaitCues_2(void) {
    s32 any = 0;
    s32 i;
    Track *n = CISet__GetBlock(D_800C1BB0->resource, 2);
    s32 cnt = n->count;
    CueRef *ents = (CueRef *)((u8 *)n + 8);
    CueRef *e;
    CueObject *k;
    for (i = 0; i < cnt; i++) {
        e = &ents[i];
        if (e->unk4 == 0) {
            k = (CueObject *)(gScene->m_pRecords + e->unk8);
        } else {
            k = (CueObject *)(gScene->m_pAltRecords + e->unk8);
        }
        if (Cinema_IsWaitCue_2(k) != 0) {
            any = 1;
        }
    }
    return any ^ 1;
}

typedef struct {
    f32 time;
    s32 unk4;
    s32 unk8;
} TimedCueRef;

s32 Cinema_IsActive(void);
void LevelEvent_Trigger(CueObject *k);
void CScene__SetEventFired(CScene *g, CueObject *k, s32 z);

void Cinema_FireCues_2(void) {
    f32 now = D_800C1BB0->time;
    s32 flag = 0;
    Track *n;
    TimedCueRef *e;
    TimedCueRef *ents;
    CueObject *k;
    s32 cnt;
    s32 i;
    if (Cinema_IsActive() != 0) {
        if (D_800C1BB0->unk50 != 0) {
            n = CISet__GetBlock(D_800C1BB0->resource, 2);
            cnt = n->count;
            ents = (TimedCueRef *)((u8 *)n + 8);
            if (cnt != 0) {
                if (D_800C1BB0->endTime <= now) {
                    flag = 1;
                }
                for (i = 0; i < cnt; i++) {
                    e = &ents[i];
                    if (e->time <= -10000.0f) {
                        continue;
                    }
                    if (now < e->time) {
                        if (flag == 0) {
                            continue;
                        }
                    }
                    if (e->unk4 == 0) {
                        k = (CueObject *)(gScene->m_pRecords + e->unk8);
                    } else {
                        k = (CueObject *)(gScene->m_pAltRecords + e->unk8);
                    }
                    if (Cinema_IsWaitCue_2(k) != 0) {
                        Cinema_RequestForObject_2(0, k);
                        D_800C1BB0->resetPlayerNext = 0;
                    } else {
                        LevelEvent_Trigger(k);
                    }
                    e->time = -20000.0f;
                    k->unkE = (k->unkE | 1) & 0xEF;
                    CScene__SetEventFired(gScene, k, 0);
                }
            }
        }
    }
}

static __inline__ CVector3 getA(void) {
    CVector3 t;
    CVector3 v;
    Track *n;
    if (D_800C1BB0->active == 0) {
        v.x = 0;
        v.y = 0;
        v.z = 0;
        return v;
    }
    n = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(D_800C1BB0->resource, 0), 0), 0);
    t = Cinema_SplineEval_2((SplineNode *)((u8 *)n + 8), n->count, 0.0f);
    return t;
}
static __inline__ CVector3 getB(void) {
    CVector3 t;
    CVector3 v;
    Track *n;
    if (D_800C1BB0->active == 0) {
        v.x = 10.0f;
        v.y = 0;
        v.z = 0;
        return v;
    }
    n = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(D_800C1BB0->resource, 0), 0), 1);
    t = Cinema_SplineEval_2((SplineNode *)((u8 *)n + 8), n->count, 0.0f);
    return t;
}
CVector3 Cinema_GetStartAngles_2(void) {
    CVector3 a;
    CVector3 b;
    f32 dir[4];
    CVector3 res;
    if (D_800C1BB0->active == 0) {
        res.x = 0;
        res.y = 0;
    } else {
        a = getA();
        b = getB();
        CVector3__Subtract((CVector3 *) dir, &b, &a);
        res.x = -CVector3__GetPitch((CVector3 *) dir);
        res.y = CVector3__GetYaw((CVector3 *) dir);
    }
    do {
        res.z = 0;
    } while (0);
    return res;
}

void Cinema_StopAtEnd_2(void) {
    D_800C1BB0->stopAtEnd = 1;
}

void Cinema_RequestOnce_2(s32 a0) {
    register s32 a asm("$7");
    register u8 *bits asm("$4");
    register s32 id asm("$5");
    u8 b;
    s32 m;
    s32 i;
    a = a0;
    bits = (u8 *) D_80119890;
    if (D_800C1BB8 != 0) {
        id = a - 0x8D;
        if ((u32)id < 6 && bits != 0) {
            bits += 0x9F4;
            i = id >> 3;
            m = 1 << (id & 7);
            b = bits[i];
            if ((b & m) != 0) return;
            bits[i] = b | m;
        }
        Cinema_RequestById_2(a);
    }
}

CVector3 Cinema_GetStartEyePos_2(void) {
    CVector3 sp10;
    CVector3 sp20;
    StridedTable *temp_v0;
    if (D_800C1BB0->active == 0) {
        sp20.x = 0;
        sp20.y = 0;
        sp20.z = 0;
        return sp20;
    }
    temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(D_800C1BB0->resource, 0), 0), 0);
    sp10 = Cinema_SplineEval_2((SplineNode *) temp_v0->data, temp_v0->count, 0.0f);
    return sp10;
}

CVector3 Cinema_GetStartTargetPos_2(void) {
    CVector3 sp10;
    CVector3 sp20;
    StridedTable *temp_v0;
    if (D_800C1BB0->active == 0) {
        sp20.y = 0;
        sp20.z = 0;
        sp20.x = 10.0f;
        return sp20;
    }
    temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(D_800C1BB0->resource, 0), 0), 1);
    sp10 = Cinema_SplineEval_2((SplineNode *) temp_v0->data, temp_v0->count, 0.0f);
    return sp10;
}

f32 Cinema_GetDuration_2(void) {
    f32 temp_f0;
    f32 var_f20;
    s32 *temp_v0;
    s32 var_s0;
    var_f20 = 0.0f;
    var_s0 = 0;
loop_1:
    if (var_s0 < *(s32 *)CISet__GetBlock(D_800C1BB0->resource, 0)) {
        temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(D_800C1BB0->resource, 0), var_s0), 0);
        temp_v0 = (s32 *)((char *)temp_v0 + temp_v0[1] * 0x24);
        temp_f0 = *(f32 *)temp_v0;
        var_s0 += 1;
        var_f20 += temp_f0;
        goto loop_1;
    }
    return var_f20;
}

s32 Cinema_GetClipCount_2(void) {
    return *(s32 *)CISet__GetBlock(D_800C1BB0->resource, 0);
}

Spline *Cinema_GetClipEyeSpline_2(s32 index) {
    return CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(D_800C1BB0->resource, 0), index), 0);
}

void *Cinema_GetFadeTrack_2(void) {
    return CISet__GetBlock(D_800C1BB0->resource, 1);
}

s32 Cinema_TrackData_2(s32 arg0) {
    return arg0 + 8;
}
u32 Cinema_BitTest_2(u8 *bits, s32 index) {
    s32 byte = index >> 3;
    u32 mask;
    index &= 7;
    mask = 1;
    bits += byte;
    return *bits & (mask << index);
}

void Cinema_BitSet_2(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_a0;
    s32 temp_a3;
    u8 *temp_v0;
    u8 *temp_v1;

    temp_a3 = arg1 >> 3;
    temp_a0 = 1 << (arg1 & 7);
    if (arg2 != 0) {
        temp_v1 = (u8 *) arg0 + temp_a3;
        *temp_v1 |= temp_a0;
        return;
    }
    temp_v0 = (u8 *) arg0 + temp_a3;
    *temp_v0 &= ~temp_a0;
}

/* Allocates a temporary block of `size` bytes; the word before the data's 0x10-byte header points back at the handle. */
void *Cinema_TempAlloc_2(s32 size) {
    s32 **handle;
    s32 *block;

    handle = CCache__Allocate(0, size + 0x10, 0x13, "Cinema temp alloc");
    if (handle != NULL) {
        block = *handle;
        *block = (s32) handle;
        return block + 4;
    }
    return NULL;
}

void Cinema_TempFree_2(void *arg0) {
    if (arg0 != NULL) {
        CCache__Deallocate(0, ((s32 **) arg0)[-4]);
    }
}

s32 Cinema_SplineBlockSize_2(void *object) {
    struct IntPair *first = CISet__GetBlock(object, 0);
    struct IntPair *second = CISet__GetBlock(object, 2);
    s32 size = first->field_4 * 36;
    return size + second->field_4 * 36 + 0x30;
}

s32 Cinema_GetEntryCount_2(void) {
    return *D_800F70D8 - 1;
}

/* Loads the cinema table. */
s32 **Cinema_LoadTable_2(void) {
    s32 size;
    s32 loaded;
    CScene *resources = gScene;

    loaded = CISet__GetROMAddressAndSize(resources->m_pCinemas, resources->m_CinemasKey, 0, &size);
    if (loaded == 0) {
        return NULL;
    }
    return CCache__Request(0, loaded, size, 0x13, 0, NULL, "Cinema", 1);
}

/* A Hermite basis term, (1 - t)^3 - (1 - t) and t^3 - t, weighted. */
f32 Cinema_SplineCubicTerm_2(f32 a, f32 b, f32 t) {
    f32 u;
    u = 1.0f - t;
    return (a * ((u * u * u) - u)) + (b * ((t * t * t) - t));
}

f32 Cinema_Lerp_2(f32 a, f32 b, f32 t) {
    return (a * (1.0f - t)) + (b * t);
}

void *Cinema_GetClip_2(s32 index) {
    return CISet__GetBlock(CISet__GetBlock(D_800C1BB0->resource, 0), index);
}

s32 Cinema_ClipEyeHeld_2(s32 index) {
    return *(s32 *)CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(D_800C1BB0->resource, 0), index), 2);
}

s32 Cinema_ClipTargetHeld_2(s32 index) {
    return ((s32 *)CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(D_800C1BB0->resource, 0), index), 2))[1];
}

void *Cinema_GetClipTargetSpline_2(s32 index) {
    return CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(D_800C1BB0->resource, 0), index), 1);
}

void *Cinema_GetFOVTrack_2(void) {
    return CISet__GetBlock(D_800C1BB0->resource, 3);
}
