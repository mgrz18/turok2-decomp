/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCHED/8023A284_4.CPP, func_8023A4E0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "gfx.h"
#include "gbi_t2.h"
#include "structs/camera.h"
#include "structs/emitter.h"
#include "globals/engine.h"
#include "protos/cache.h"

typedef struct AnimationInfo {
    /* 0x0 */ s32 frameCount;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ u8 *frame;            /* format, rate, log2 width, log2 height */
    /* 0xC */ u8 padC[4];
} AnimationInfo;
extern void *Texture_Request(void *texture);
extern void Texture_GetInfo(void *resource, AnimationInfo *info);
extern void Texture_Load(void *texture, void *resource, s32 frame, s32, s32 scaleS, s32 scaleT, s32, s32, s32);
extern void Render_SetCombineMode(s32 mode);
extern f32 Plane_SegmentFraction(f32 *plane, CVector3 *from, CVector3 *to);
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a - b */
extern void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);   /* out = a x b */

/* Clips and draws an emitter as a textured translucent fan at its depth. */
void CEmitter__Draw(CEmitter *object, CCamera *view) {
    AnimationInfo animation;
    CVector3 points[5];
    f32 red[5], green[5], blue[5], alpha[5];
    CVector3 triangle[3];
    CVector3 texcoord[3];
    f32 triRed[3], triGreen[3], triBlue[3], triAlpha[3];
    CVector3 edge0, edge1, normal;
    u8 center[3], rim[3];
    s32 below;
    void *resource;
    CEmitterDef *descriptor;
    s32 frameIndex, scaleS, scaleT;
    u8 *frame;
    f32 phase, fadeRange, fadeOffset;
    s32 format;
    s32 i, face, first, second, third;
    f32 scale, texturePeriod, wrapPeriod, distance, fade, depth, t;
    f32 maxS, minS, maxT, minT, shiftS, shiftT, factor;
    UnitVtx *vertex;

    resource = Texture_Request(object->m_Texture);
    if (resource != NULL) {
        Texture_GetInfo(resource, &animation);
        if (animation.frameCount != 0) {
            descriptor = object->m_pDef;
            frame = animation.frame;
            phase = object->m_Phase / frame[1];
            scale = descriptor->m_TextureScale * (object->m_TextureScale * 768.0f);
            scaleS = (s32)((1 << frame[2]) * scale);
            scaleT = (s32)((1 << frame[3]) * scale);
            {
                Gfx *g = gDList++;
                g->w0 = 0xE7000000;     /* gDPPipeSync */
                g->w1 = 0;
            }
            frameIndex = (s32)phase % animation.frameCount;
            Texture_Load(object->m_Texture, resource, frameIndex, 0, scaleS, scaleT, 0, 0, 15);
            texturePeriod = 5120.0f;
            wrapPeriod = 10240.0f;
            frame = animation.frame;
            format = frame[0];
            switch (format) {
            case 2:
            default:
                Render_SetCombineMode(32);
                {
                    Gfx *g = gDList++;
                    g->w0 = 0xFB000000;     /* gDPSetEnvColor */
                    g->w1 = (descriptor->m_EnvRed << 24) | (descriptor->m_EnvGreen << 16) | (descriptor->m_EnvBlue << 8) | 0xFF;
                }
                center[0] = descriptor->m_Red;
                center[1] = descriptor->m_Green;
                center[2] = descriptor->m_Blue;
                rim[0] = descriptor->m_Red;
                rim[1] = descriptor->m_Green;
                rim[2] = descriptor->m_Blue;
                break;
            case 0:
            case 1:
                Render_SetCombineMode(33);
                center[0] = 255;
                center[1] = 255;
                center[2] = 255;
                rim[0] = 255;
                rim[1] = 255;
                rim[2] = 255;
                break;
            }
            below = 1;
            depth = object->m_Height;
            if (!(view->m_vTCorners[0].y < (view->m_vTCorners[1].y + view->m_vTCorners[2].y + view->m_vTCorners[3].y + view->m_vTCorners[4].y) * 0.25f)) {
                below = 0;
            }
            distance = view->m_vTCorners[0].y - depth;
            fade = descriptor->m_Alpha;
            if (distance < 0.0f) {
                distance = -distance;
            }
            if (distance < descriptor->m_FadeDepth) {
                fadeOffset = distance - descriptor->m_FadeDepth * 0.25f;
                fadeRange = descriptor->m_FadeDepth * 0.75f;
                if (fadeOffset < 0.0f) {
                    fade = 0.0f;
                } else {
                    fade = fadeOffset * fade / fadeRange;
                }
            }
            fade *= object->m_Throttle * object->m_Opacity;
            for (i = 0; i < 5; i++) {
                points[i].x = view->m_vTCorners[i].x;
                points[i].y = depth;
                points[i].z = view->m_vTCorners[i].z;
                red[i] = i == 0 ? center[0] : rim[0];
                green[i] = i == 0 ? center[1] : rim[1];
                blue[i] = i == 0 ? center[2] : rim[2];
                alpha[i] = i == 0 ? fade : 0.0f;
            }
            for (i = 1; i < 5; i++) {
                t = Plane_SegmentFraction((f32 *)&view->m_ViewVolume[2], &points[0], &points[i]);
                if (t >= 0.0f && t < 1.0f) {
                    points[i].x = points[0].x + t * (points[i].x - points[0].x);
                    points[i].z = points[0].z + t * (points[i].z - points[0].z);
                }
            }
            for (face = 0; face < 4; face++) {
                switch (face) {
                case 0:
                default:
                    first = 0;
                    second = 1;
                    third = 3;
                    break;
                case 1:
                    first = 0;
                    second = 3;
                    third = 4;
                    break;
                case 2:
                    first = 0;
                    second = 4;
                    third = 2;
                    break;
                case 3:
                    first = 0;
                    second = 2;
                    third = 1;
                    break;
                }
                triangle[0] = points[first];
                triangle[1] = points[second];
                triangle[2] = points[third];
                CVector3__Subtract(&edge0, &triangle[1], &triangle[0]);
                CVector3__Subtract(&edge1, &triangle[2], &triangle[1]);
                CVector3__Cross(&normal, &edge0, &edge1);
                if (normal.y <= 0.0f) {
                    if (below != 1) {
                        goto draw_face;
                    }
                } else {
                    if (below != 0) {
                    draw_face:
                        triRed[0] = red[first];
                        triRed[1] = red[second];
                        triRed[2] = red[third];
                        triGreen[0] = green[first];
                        triGreen[1] = green[second];
                        triGreen[2] = green[third];
                        triBlue[0] = blue[first];
                        triBlue[1] = blue[second];
                        triBlue[2] = blue[third];
                        triAlpha[0] = alpha[first];
                        triAlpha[1] = alpha[second];
                        triAlpha[2] = alpha[third];
                        texcoord[0] = points[first];
                        texcoord[1] = points[second];
                        texcoord[2] = points[third];
                        for (i = 0; i < 3; i++) {
                            texcoord[i].x -= triangle[0].x * 0.5f;
                            texcoord[i].z -= triangle[0].z * 0.5f;
                            texcoord[i].x += object->m_ScrollS;
                            texcoord[i].z += object->m_ScrollT;
                        }
                        maxS = texcoord[2].x;
                        if (!(texcoord[1].x <= maxS)) maxS = texcoord[1].x;
                        if (!(texcoord[0].x <= maxS)) maxS = texcoord[0].x;
                        minS = texcoord[2].x;
                        if (!(minS <= texcoord[1].x)) minS = texcoord[1].x;
                        if (!(minS <= texcoord[0].x)) minS = texcoord[0].x;
                        maxT = texcoord[2].z;
                        if (!(texcoord[1].z <= maxT)) maxT = texcoord[1].z;
                        if (!(texcoord[0].z <= maxT)) maxT = texcoord[0].z;
                        minT = texcoord[2].z;
                        if (!(minT <= texcoord[1].z)) minT = texcoord[1].z;
                        if (!(minT <= texcoord[0].z)) minT = texcoord[0].z;
                        if (wrapPeriod <= maxS) {
                            shiftS = (s32)(maxS / wrapPeriod) * -wrapPeriod;
                        } else if (minS <= -wrapPeriod) {
                            shiftS = (s32)(minS / wrapPeriod) * wrapPeriod;
                        } else {
                            shiftS = 0.0f;
                        }
                        if (wrapPeriod <= maxT) {
                            shiftT = (s32)(maxT / wrapPeriod) * -wrapPeriod;
                        } else if (minT <= -wrapPeriod) {
                            shiftT = (s32)(minT / wrapPeriod) * wrapPeriod;
                        } else {
                            shiftT = 0.0f;
                        }
                        for (i = 0; i < 3; i++) {
                            vertex = i + &object->m_Vertices[gEvenOdd][face * 3];
                            vertex->flag = 0;
                            vertex->x = (s32)triangle[i].x;
                            vertex->y = (s32)triangle[i].y;
                            vertex->z = (s32)triangle[i].z;
                            factor = 16384.0f / texturePeriod;
                            vertex->s = (s32)((texcoord[i].x + shiftS) * factor);
                            vertex->t = (s32)((texcoord[i].z + shiftT) * factor);
                            vertex->r = (u32)triRed[i];
                            vertex->g = (u32)triGreen[i];
                            vertex->b = (u32)triBlue[i];
                            vertex->a = (u32)triAlpha[i];
                        }
                        {
                            Gfx *g = gDList++;
                            g->w0 = 0x01003006;     /* gSPVertex(v, 3, 0) */
                            g->w1 = GBI_ADDR(&object->m_Vertices[gEvenOdd][face * 3]);
                        }
                        {
                            Gfx *g = gDList++;
                            g->w0 = 0x05000204;     /* gSP1Triangle(0, 1, 2, 0) */
                            g->w1 = 0;
                        }
                    }
                }
            }
        }
        CCache__Unlock(0, resource);
    }
}

#include "structs/region.h"
#include "globals/scene.h"
#include "protos/list.h"
#include "protos/scene.h"
#define MIN(a, b) ((a) > (b) ? (b) : (a))

f32 CGameRegion__GetHeight(sptr region, f32 x, f32 z);
CVector3 CCamera__GetViewOrigin(CCamera *camera);
void Material_GetTextureSet(s32 *resource, s32 id);

/* CEmitterPool__TakeEmitter, inlined: takes a free emitter. */
static __inline__ CEmitter *TakeEmitter(CEmitterPool *owner) {
    CEmitter *e = owner->m_FreeList.m_pHead;

    if (e != NULL) {
        CList__Remove(&owner->m_FreeList, e);
        CList__AddTail(&owner->m_ActiveList, e);
    }
    return e;
}

/* CEmitterPool__ReleaseEmitter, inlined: gives an emitter back. */
static __inline__ void FreeEmitter(CEmitterPool *owner, CEmitter *e) {
    CList__Remove(&owner->m_ActiveList, e);
    CList__AddHead(&owner->m_FreeList, e);
}

/* CEmitter__Start, inlined. */
static __inline__ void InitEmitter(CEmitter *e, CEmitterDef *def, s16 mask, f32 scale,
                                   f32 offset0, f32 offset1, f32 offset2) {
    e->m_pDef = def;
    e->m_Mask = mask;
    Material_GetTextureSet(e->m_Texture, def->m_Texture);
    e->m_ScrollS = 0;
    e->m_ScrollT = 0;
    e->m_Throttle = 0;
    e->m_Phase = 0;
    e->m_PhaseScale = scale;
    e->m_TextureScale = offset0;
    e->m_HeightOffset = offset1;
    e->m_Opacity = offset2;
}

/* CEmitter__Advance, inlined: returns 1 once the emitter has wound down. */
static __inline__ s32 UpdateEmitter(CEmitter *e, s32 mask, f32 fallback, s32 boosting, f32 boost, CCamera *camera) {
    CEmitterDef *def = e->m_pDef;
    f32 x;
    f32 step;
    f32 t;
    f32 wrap;

    if (e->m_Mask & mask) {
        f32 up = e->m_Throttle;
        up += def->m_ThrottleRate * gFrameIncrement;
        if (1.0f < up) {
            up = 1.0f;
        }
        e->m_Throttle = up;
    } else {
        f32 dn;
        if (e->m_Throttle == 0.0f) {
            return 1;
        }
        dn = e->m_Throttle - def->m_ThrottleRate * gFrameIncrement;
        dn = (dn < 0.0f) ? 0.0f : dn;
        e->m_Throttle = dn;
    }
    e->m_Phase = e->m_Phase + gFrameIncrement * def->m_PhaseRate * e->m_PhaseScale;
    step = gFrameIncrement * gEmitterTimeScale * def->m_SpinRate;
    t = e->m_ScrollS + step * 1.536f;
    wrap = 20480.0f;
    e->m_ScrollS = t;
    if (wrap < t) {
        e->m_ScrollS = t - wrap;
    }
    t = e->m_ScrollT + step * 3.072f;
    e->m_ScrollT = t;
    if (wrap < t) {
        e->m_ScrollT = t - wrap;
    }
    x = def->m_BaseHeight + e->m_HeightOffset;
    if (boosting != 0 && camera->m_ViewParams.m_nType == 0) {
        x += boost;
    } else {
        u8 b = def->m_Flags;
        if (b & EMITTER_DEF_FLAG_CAMERA) {
            x += camera->m_vPos.y * def->m_CameraScale;
        } else if (b & EMITTER_DEF_FLAG_FLOOR) {
            x += fallback;
        }
    }
    e->m_Height = x;
    return 0;
}

/* Starts the emitters the camera's region asks for (one per bit of its mask,
 * from the table at gScene_m_pSections) and updates the running ones, giving back
 * those that have wound down. Under a region flagged 1, the emitters get a
 * boost while the camera is below the region's height. */
void CEmitterPool__Advance(CEmitterPool *owner, CCamera *camera) {
    CRegionInfo *info;
    CEmitterDef *def;
    CEmitter *e;
    CEmitter *next;
    sptr region;
    s32 n;
    s32 bit;
    s32 mask;
    s32 below;
    f32 ground;
    f32 boost;
    f32 depth;

    region = (sptr)camera->m_ViewParams.m_pCurrentRegion;
    if (region == 0) {
        return;
    }
    info = CScene__GetRegionInfo(&gScene, region);
    if (info == NULL) {
        return;
    }
    boost = 0.0f;
    ground = CGameRegion__GetHeight(region, camera->m_ViewParams.m_vPos.x, camera->m_ViewParams.m_vPos.z);
    below = 0;
    if (info->m_dwFlags & 1) {
        depth = MIN((info->m_WaterHeight - CCamera__GetViewOrigin(camera).y) * 0.5f, 20.48f);
        below = CCamera__GetViewOrigin(camera).y < info->m_WaterHeight;
        boost = info->m_WaterHeight - depth;
    }
    def = gScene_m_pSections;
    mask = info->m_EmitterMask;
    bit = 1;
    n = gScene_m_nSections;
    while (--n != -1) {
        if ((mask & bit) && def->m_Texture != 0 && !(owner->m_ActiveMask & bit)) {
            e = TakeEmitter(owner);
            if (e != NULL) {
                InitEmitter(e, def, bit, 1.0f, 1.0f, 0.0f, 1.0f);
                owner->m_ActiveMask |= bit;
            }
        }
        def++;
        bit <<= 1;
    }
    e = owner->m_ActiveList.m_pHead;
    while (e != NULL) {
        next = e->m_pNext;
        if (UpdateEmitter(e, mask, ground, below, boost, camera)) {
            owner->m_ActiveMask &= ~e->m_Mask;
            FreeEmitter(owner, e);
        }
        e = next;
    }
}

#include "globals/frame.h"

void Render_SetRenderMode(s32 mode);
void Sort_Quick(void *items, s32 count, s32 size, void *compare, void *swap);
s32 Emitter_CompareHeightDesc(CEmitter **arg0, CEmitter **arg1);
s32 Emitter_CompareHeightAsc(CEmitter **a, CEmitter **b);
void Emitter_SwapPointers(s32 *arg0, s32 *arg1);

/* Draws the running emitters: sets up the display list, splits them into
 * those deeper than the view and the others (four of each at most), sorts
 * each group and draws the deeper group first. */
void CEmitterPool__Draw(CEmitterPool *owner, CCamera *view) {
    CEmitter *near[4];
    CEmitter *far[4];
    CEmitter *e;
    s32 nearCount;
    s32 farCount;
    s32 i;

    if (owner->m_ActiveList.m_nCount == 0) {
        return;
    }
    GFX_CMD(0xE7000000, 0)
    GFX_CMD(0xDA380003, GBI_ADDR(&gIdentityMtx))
    GFX_CMD(0xDB040004, 2)
    GFX_CMD(0xDB04000C, 2)
    GFX_CMD(0xDB040014, 0xFFFE)
    GFX_CMD(0xDB04001C, 0xFFFE)
    GFX_CMD(0xD9FEFFFF, 0)
    GFX_CMD(0xE3000A01, 0)
    Render_SetRenderMode(0xD);
    GFX_CMD(0xD9F9FBFF, 0)
    GFX_CMD(0xD9FFFFFF, 0x200004)
    GFX_CMD(0xE3000C00, 0x80000)
    GFX_CMD(0xE3001201, 0x2000)
    nearCount = 0;
    farCount = 0;
    for (e = owner->m_ActiveList.m_pHead; e != NULL; e = e->m_pNext) {
        if (e->m_Height > view->m_vPos.y) {
            near[nearCount++] = e;
        } else {
            far[farCount++] = e;
        }
    }
    Sort_Quick(near, nearCount, 4, Emitter_CompareHeightDesc, Emitter_SwapPointers);
    Sort_Quick(far, farCount, 4, Emitter_CompareHeightAsc, Emitter_SwapPointers);
    for (i = 0; i < nearCount; i++) {
        CEmitter__Draw(near[i], view);
    }
    for (i = 0; i < farCount; i++) {
        CEmitter__Draw(far[i], view);
    }
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_6.c, func_8023B694_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void Material_GetTextureSet(s32 *arg0, s32 arg1);

void CEmitter__Start(Effect *effect, s16 *definition, s16 mask, f32 scale,
                   f32 offset0, f32 offset1, f32 offset2) {
    effect->definition = definition;
    effect->mask = mask;
    Material_GetTextureSet(&effect->resource, *definition);
    effect->angle0 = 0;
    effect->angle1 = 0;
    effect->timer = 0;
    effect->velocity = 0;
    effect->scale = scale;
    effect->offset0 = offset0;
    effect->offset1 = offset1;
    effect->offset2 = offset2;
}

typedef struct B09EDef {
               s16 id;
               u8 pad2[6];
               f32 f8;
               u8 padC[4];
               f32 f10;
               f32 f14;
               u8 pad18[4];
               f32 f1C;
               f32 f20;
               u8 pad24[5];
               u8 flags29;
} B09EDef;
typedef struct B09Emitter B09Emitter;
struct B09Emitter {
                u8 pad0[4];
                B09Emitter *next;
                u8 pad8[4];
                B09EDef *def;
                u16 mask;
                u8 pad12[2];
                u8 pad14[8];
                f32 f1C;
                f32 f20;
                u8 pad24[0x1E4];
                f32 f208;
                f32 f20C;
                f32 key;
                f32 f214;
                f32 f218;
                f32 f21C;
                f32 f220;
};

s32 CEmitter__Advance(B09Emitter *e, s32 mask, f32 f2, s32 flag, f32 f4, u8 *ctx) {
    B09EDef *def = e->def;
    f32 x;
    f32 step;
    f32 t;
    if (e->mask & mask) {
        f32 up = e->f208;
        up += def->f20 * gFrameIncrement;
        if (1.0f < up) {
            up = 1.0f;
        }
        e->f208 = up;
    } else {
        f32 dn;
        if (e->f208 == 0.0f) {
            return 1;
        }
        dn = e->f208 - def->f20 * gFrameIncrement;
        dn = (dn < 0.0f) ? 0.0f : dn;
        e->f208 = dn;
    }
    e->f20C = e->f20C + gFrameIncrement * def->f10 * e->f214;
    step = gFrameIncrement * gEmitterTimeScale * def->f8;
    t = e->f1C + step * 1.536f;
    e->f1C = t;
    if (2.048e4f < t) {
        e->f1C = t - 2.048e4f;
    }
    t = e->f20 + step * 3.072f;
    e->f20 = t;
    if (2.048e4f < t) {
        e->f20 = t - 2.048e4f;
    }
    x = def->f14 + e->f21C;
    if (flag != 0 && *(s32 *)(ctx + 0x24) == 0) {
        x += f4;
    } else {
        u8 b = def->flags29;
        if (b & 1) {
            x += *(f32 *)(ctx + 0x118) * def->f1C;
        } else if (b & 2) {
            x += f2;
        }
    }
    e->key = x;
    return 0;
}

/* Takes the first free emitter and moves it to the active list. */
sptr CEmitterPool__TakeEmitter(CEmitterPool *arg0) {
    sptr temp_s0;

    temp_s0 = (sptr) arg0->m_FreeList.m_pHead;
    if (temp_s0 != 0) {
        CList__Remove(&arg0->m_FreeList, temp_s0);
        CList__AddTail(&arg0->m_ActiveList, temp_s0);
    }
    return temp_s0;
}

/* Gives an emitter back: off the active list, onto the free list. */
void CEmitterPool__ReleaseEmitter(CEmitterPool *pool, CEmitter *emitter) {
    CList__Remove(&pool->m_ActiveList, emitter);
    CList__AddHead(&pool->m_FreeList, emitter);
}

#include "m2c_macros.h"

void Emitter_SwapPointers(s32 *arg0, s32 *arg1) {
    s32 temp_v1;

    temp_v1 = *arg0;
    *arg0 = *arg1;
    *arg1 = temp_v1;
}

/* qsort-style comparator on the float at 0x210 of the pointed-to objects. */
s32 Emitter_CompareHeightDesc(CEmitter **arg0, CEmitter **arg1) {
    s32 r = 1;
    if ((*arg0)->m_Height > (*arg1)->m_Height) {
        r = -1;
    }
    return r;
}

/* qsort-style comparator: ascending by the float at 0x210. */
s32 Emitter_CompareHeightAsc(CEmitter **a, CEmitter **b) {
    s32 r = 1;
    if ((*a)->m_Height < (*b)->m_Height) {
        r = -1;
    }
    return r;
}
