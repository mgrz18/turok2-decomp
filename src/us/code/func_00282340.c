/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCHED/8023A284_4.CPP, func_8023A4E0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "gfx.h"
#include "structs/camera.h"
#include "structs/emitter.h"
#include "globals/engine.h"

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
extern void CCache__Unlock(s32, void *resource);

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
                            g->w1 = (u32)&object->m_Vertices[gEvenOdd][face * 3];
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
