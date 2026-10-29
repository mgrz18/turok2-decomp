/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/CODE_80281A70.CPP, Particle_DrawRenderList, with Turok 2's layouts. */
#include "common.h"

/* Draws the particle render list: culls each sprite particle by its bounds
 * against the view, builds its matrix once per frame, draws it as a model or
 * a sprite (plus its trail), and queues the attached effects of the ones
 * that are drawn even when hidden.
 * The list walk is a goto loop and the built flag a local, as in the ROM: the
 * flag stays in a saved register and nothing else is hoisted out of the walk. */

typedef struct { u32 w0, w1; } Gfx;
typedef struct { f32 x, y, z; } Vec3f;
typedef f32 Mtx4f[4][4];

#define GFX_CMD(a, b) { Gfx *_g = D_800D8E24++; _g->w0 = (a); _g->w1 = (b); }

typedef struct {
    /* 0x00 */ Vec3f min;
    /* 0x0C */ Vec3f max;
} Box;

typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ void *renderModel;
} ParticleDesc;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 mask;
} Region;

typedef struct DrawParticle {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ Region *region;
    /* 0x014 */ u8 pad14[0x40 - 0x14];
    /* 0x040 */ u32 flags;
    /* 0x044 */ u8 pad44[0x48 - 0x44];
    /* 0x048 */ u8 mtx[2][0x40];
    /* 0x0C8 */ u8 mtx2[2][0x18];
    /* 0x0F8 */ ParticleDesc *desc;
    /* 0x0FC */ u8 padFC[0x12C - 0xFC];
    /* 0x12C */ Vec3f size;
    /* 0x138 */ u8 pad138[0x1B5 - 0x138];
    /* 0x1B5 */ u8 drawWhenHidden;
    /* 0x1B6 */ u8 pad1B6[0x1D0 - 0x1B6];
    /* 0x1D0 */ struct DrawParticle *next;
} DrawParticle;

typedef struct {
    /* 0x0000 */ u8 pad0[0xEC14];
    /* 0xEC14 */ DrawParticle *renderHead;
} ParticleDrawWorld;

typedef struct {
    /* 0x000 */ u8 pad0[0x64];
    /* 0x064 */ s32 mask;
    /* 0x068 */ u8 pad68[0x2D8 - 0x68];
    /* 0x2D8 */ u8 clipPlanes[0x338 - 0x2D8];
    /* 0x338 */ Vec3f clipMin;
    /* 0x344 */ Vec3f clipMax;
} ParticleView;

typedef struct {
    /* 0x000 */ u8 pad0[0x20C];
    /* 0x20C */ Mtx4f viewMatrix;
} Camera;

extern Gfx *D_800D8E24;             /* display list head */
extern s32 D_800B6D1C;              /* matrix buffer index */
extern s32 D_801198E0;
extern Camera *D_801198D0;
extern u8 D_800F7078[];
extern u8 D_801102B8[];

extern s32 func_0026F60C(void *planes, Box *box);
extern void func_0020F85C(Mtx4f m, Vec3f *in, Vec3f *out);   /* transform point */
extern void func_0022D2FC(DrawParticle *particle, void *mtx, Camera *camera, f32 depth);
extern void func_002254C0(void *list, Vec3f *pos, void *out);
extern void func_0020B9F4(void *model, void *mtx, void *mtx2, s32 a, s32 b);
extern void func_0022E094(DrawParticle *particle, ParticleView *view);
extern void func_0022DCBC(DrawParticle *particle, ParticleView *view);
extern void func_00236170(void *list, DrawParticle *particle, void *mtx);

void func_0023118C(ParticleDrawWorld *world, ParticleView *view) {
    Box box;
    Vec3f transformed;
    DrawParticle *node;
    Camera *camera;
    s32 visible, built;
    f32 zScale, xyScale, xScale, maximum, height, radius;
    f32 minX, maxX, minY, maxY, minZ, maxZ;
    f32 depth;
    s32 mask;
    u32 builtFlag;

    if (world->renderHead != 0) {
        GFX_CMD(0xE7000000, 0)
        GFX_CMD(0xE3001A01, 0x20)
        GFX_CMD(0xE3001801, 0x80)
        GFX_CMD(0xE3000A01, 0)
        GFX_CMD(0xD9F9FBFF, 0)
        node = world->renderHead;
        mask = view->mask;
        builtFlag = 0x100000;
        if (node != 0) {
        next:
            visible = 0;
            built = node->flags & builtFlag;
            if (node->region == 0 || (mask & node->region->mask)) {
                if (!(node->flags & 0x400000) && node->desc->renderModel == 0) {
                    zScale = node->size.z;
                    xyScale = node->size.y * zScale;
                    xScale = node->size.x * zScale;
                    if (!(xScale <= xyScale)) xyScale = xScale;
                    maximum = zScale;
                    if (!(xyScale <= maximum)) maximum = xyScale;
                    height = maximum * 1.4142135f;
                    radius = height * 0.5f;
                    minX = node->pos.x - radius;
                    box.min.x = minX;
                    maxX = node->pos.x + radius;
                    box.max.x = maxX;
                    minY = node->pos.y - radius;
                    box.min.y = minY;
                    maxY = node->pos.y + height;
                    box.max.y = maxY;
                    minZ = node->pos.z - radius;
                    box.min.z = minZ;
                    maxZ = node->pos.z + radius;
                    box.max.z = maxZ;
                    if (minX < view->clipMax.x && view->clipMin.x < maxX &&
                        minZ < view->clipMax.z && view->clipMin.z < maxZ &&
                        minY < view->clipMax.y && view->clipMin.y < maxY &&
                        func_0026F60C(view->clipPlanes, &box)) {
                        visible = 1;
                    }
                } else visible = 1;
            }
            do {
                if (visible) {
                    if (!built) {
                        if (D_801198E0 == 1) {
                            camera = D_801198D0;
                            func_0020F85C(camera->viewMatrix, &node->pos, &transformed);
                            depth = transformed.z;
                            if (depth < 0.0f) depth = -depth;
                            func_0022D2FC(node, node->mtx[D_800B6D1C], camera, depth);
                        } else {
                            func_0022D2FC(node, node->mtx[D_800B6D1C], 0, 0.0f);
                        }
                        node->flags |= builtFlag;
                    }
                    if (node->desc->renderModel != 0) {
                        func_002254C0(D_800F7078, &node->pos, node->mtx2[D_800B6D1C]);
                        func_0020B9F4(node->desc->renderModel, node->mtx[D_800B6D1C], node->mtx2[D_800B6D1C], 0, -1);
                    } else {
                        func_0022E094(node, view);
                    }
                    if (node->desc->flags & builtFlag) {
                        func_0022DCBC(node, view);
                    }
                    if (!node->drawWhenHidden || built) break;
                    if (!(node->desc->flags & 0x10000)) {
                        func_00236170(D_801102B8, node, node->mtx[D_800B6D1C]);
                    }
                } else {
                    if (!node->drawWhenHidden || built) break;
                    if (D_801198E0 == 1) {
                        camera = D_801198D0;
                        func_0020F85C(camera->viewMatrix, &node->pos, &transformed);
                        depth = transformed.z;
                        if (depth < 0.0f) depth = -depth;
                        func_0022D2FC(node, node->mtx[D_800B6D1C], camera, depth);
                    } else {
                        func_0022D2FC(node, node->mtx[D_800B6D1C], 0, 0.0f);
                    }
                    node->flags |= builtFlag;
                    if (!(node->desc->flags & 0x10000)) {
                        func_00236170(D_801102B8, node, node->mtx[D_800B6D1C]);
                    }
                }
            } while (0);
            node = node->next;
            if (node != 0) goto next;
        }
        GFX_CMD(0xE7000000, 0)
        GFX_CMD(0xE3000A01, 0x100000)
        GFX_CMD(0xE3001801, 0)
        GFX_CMD(0xE3001A01, 0x10)
    }
}
