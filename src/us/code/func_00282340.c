/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCHED/8023A284_4.CPP, func_8023A4E0_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

/* A vertex as the RSP reads it. */
typedef struct UnitVtx {
    /* 0x0 */ s16 x, y, z;
    /* 0x6 */ s16 flag;
    /* 0x8 */ s16 s, t;
    /* 0xC */ u8 r, g, b, a;
} UnitVtx;

typedef struct EmitterDef {
    /* 0x00 */ s16 texture;
    /* 0x02 */ u8 red, green, blue;
    /* 0x05 */ u8 envRed, envGreen, envBlue;
    /* 0x08 */ f32 spinRate;
    /* 0x0C */ f32 textureScale;
    /* 0x10 */ f32 phaseRate;
    /* 0x14 */ f32 baseSpeed;
    /* 0x18 */ f32 fadeDepth;
    /* 0x1C */ f32 hostScale;
    /* 0x20 */ f32 throttleRate;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ u8 alpha;
    /* 0x29 */ u8 flags;
} EmitterDef;

/* A running emitter (see func_00282E4C): a textured fan drawn flat at its depth. */
typedef struct Emitter {
    /* 0x000 */ struct Emitter *last;
    /* 0x004 */ struct Emitter *next;
    /* 0x008 */ u8 pad8[4];
    /* 0x00C */ EmitterDef *def;
    /* 0x010 */ u16 mask;
    /* 0x012 */ u8 pad12[2];
    /* 0x014 */ s32 texture[2];
    /* 0x01C */ f32 offsetS;
    /* 0x020 */ f32 offsetT;
    /* 0x024 */ u8 pad24[4];
    /* 0x028 */ UnitVtx vertices[2][15];
    /* 0x208 */ f32 throttle;
    /* 0x20C */ f32 phase;
    /* 0x210 */ f32 depth;
    /* 0x214 */ f32 phaseScale;
    /* 0x218 */ f32 textureScale;
    /* 0x21C */ f32 trim;
    /* 0x220 */ f32 opacity;
} Emitter;

typedef struct EmitterView {
    /* 0x000 */ u8 pad0[0x24C];
    /* 0x24C */ Vec3f fan[5];       /* centre, then the four corners */
    /* 0x288 */ u8 pad288[0x2F8 - 0x288];
    /* 0x2F8 */ f32 clipPlane[4];
} EmitterView;

typedef struct AnimationInfo {
    /* 0x0 */ s32 frameCount;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ u8 *frame;            /* format, rate, log2 width, log2 height */
    /* 0xC */ u8 padC[4];
} AnimationInfo;

extern Gfx *D_800D8E24;             /* display list head */
extern s32 D_800B6D1C;              /* frame buffer index */

extern void *func_0020798C(void *texture);
extern void func_002079D8(void *resource, AnimationInfo *info);
extern void func_00206B30(void *texture, void *resource, s32 frame, s32, s32 scaleS, s32 scaleT, s32, s32, s32);
extern void func_00207A70(s32 mode);
extern f32 func_0026F400(f32 *plane, Vec3f *from, Vec3f *to);
extern void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a - b */
extern void func_0020EFDC(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a x b */
extern void func_002051F4(s32, void *resource);

/* Clips and draws an emitter as a textured translucent fan at its depth. */
void func_00282340(Emitter *object, EmitterView *view) {
    AnimationInfo animation;
    Vec3f points[5];
    f32 red[5], green[5], blue[5], alpha[5];
    Vec3f triangle[3];
    Vec3f texcoord[3];
    f32 triRed[3], triGreen[3], triBlue[3], triAlpha[3];
    Vec3f edge0, edge1, normal;
    u8 center[3], rim[3];
    s32 below;
    void *resource;
    EmitterDef *descriptor;
    s32 frameIndex, scaleS, scaleT;
    u8 *frame;
    f32 phase, fadeRange, fadeOffset;
    s32 format;
    s32 i, face, first, second, third;
    f32 scale, texturePeriod, wrapPeriod, distance, fade, depth, t;
    f32 maxS, minS, maxT, minT, shiftS, shiftT, factor;
    UnitVtx *vertex;

    resource = func_0020798C(object->texture);
    if (resource != NULL) {
        func_002079D8(resource, &animation);
        if (animation.frameCount != 0) {
            descriptor = object->def;
            frame = animation.frame;
            phase = object->phase / frame[1];
            scale = descriptor->textureScale * (object->textureScale * 768.0f);
            scaleS = (s32)((1 << frame[2]) * scale);
            scaleT = (s32)((1 << frame[3]) * scale);
            {
                Gfx *g = D_800D8E24++;
                g->w0 = 0xE7000000;     /* gDPPipeSync */
                g->w1 = 0;
            }
            frameIndex = (s32)phase % animation.frameCount;
            func_00206B30(object->texture, resource, frameIndex, 0, scaleS, scaleT, 0, 0, 15);
            texturePeriod = 5120.0f;
            wrapPeriod = 10240.0f;
            frame = animation.frame;
            format = frame[0];
            switch (format) {
            case 2:
            default:
                func_00207A70(32);
                {
                    Gfx *g = D_800D8E24++;
                    g->w0 = 0xFB000000;     /* gDPSetEnvColor */
                    g->w1 = (descriptor->envRed << 24) | (descriptor->envGreen << 16) | (descriptor->envBlue << 8) | 0xFF;
                }
                center[0] = descriptor->red;
                center[1] = descriptor->green;
                center[2] = descriptor->blue;
                rim[0] = descriptor->red;
                rim[1] = descriptor->green;
                rim[2] = descriptor->blue;
                break;
            case 0:
            case 1:
                func_00207A70(33);
                center[0] = 255;
                center[1] = 255;
                center[2] = 255;
                rim[0] = 255;
                rim[1] = 255;
                rim[2] = 255;
                break;
            }
            below = 1;
            depth = object->depth;
            if (!(view->fan[0].y < (view->fan[1].y + view->fan[2].y + view->fan[3].y + view->fan[4].y) * 0.25f)) {
                below = 0;
            }
            distance = view->fan[0].y - depth;
            fade = descriptor->alpha;
            if (distance < 0.0f) {
                distance = -distance;
            }
            if (distance < descriptor->fadeDepth) {
                fadeOffset = distance - descriptor->fadeDepth * 0.25f;
                fadeRange = descriptor->fadeDepth * 0.75f;
                if (fadeOffset < 0.0f) {
                    fade = 0.0f;
                } else {
                    fade = fadeOffset * fade / fadeRange;
                }
            }
            fade *= object->throttle * object->opacity;
            for (i = 0; i < 5; i++) {
                points[i].x = view->fan[i].x;
                points[i].y = depth;
                points[i].z = view->fan[i].z;
                red[i] = i == 0 ? center[0] : rim[0];
                green[i] = i == 0 ? center[1] : rim[1];
                blue[i] = i == 0 ? center[2] : rim[2];
                alpha[i] = i == 0 ? fade : 0.0f;
            }
            for (i = 1; i < 5; i++) {
                t = func_0026F400(view->clipPlane, &points[0], &points[i]);
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
                func_0020EF2C(&edge0, &triangle[1], &triangle[0]);
                func_0020EF2C(&edge1, &triangle[2], &triangle[1]);
                func_0020EFDC(&normal, &edge0, &edge1);
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
                            texcoord[i].x += object->offsetS;
                            texcoord[i].z += object->offsetT;
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
                            vertex = i + &object->vertices[D_800B6D1C][face * 3];
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
                            Gfx *g = D_800D8E24++;
                            g->w0 = 0x01003006;     /* gSPVertex(v, 3, 0) */
                            g->w1 = (u32)&object->vertices[D_800B6D1C][face * 3];
                        }
                        {
                            Gfx *g = D_800D8E24++;
                            g->w0 = 0x05000204;     /* gSP1Triangle(0, 1, 2, 0) */
                            g->w1 = 0;
                        }
                    }
                }
            }
        }
        func_002051F4(0, resource);
    }
}
