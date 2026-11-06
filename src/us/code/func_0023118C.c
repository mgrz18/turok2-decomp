/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/CODE_80281A70.CPP, Particle_DrawRenderList, with Turok 2's layouts. */
#include "common.h"
#include "structs/region.h"
#include "structs/particle.h"
#include "gfx.h"
#include "structs/camera.h"
#include "globals/camera.h"

/* Draws the particle render list: culls each sprite particle by its bounds
 * against the view, builds its matrix once per frame, draws it as a model or
 * a sprite (plus its trail), and queues the attached effects of the ones
 * that are drawn even when hidden.
 * The list walk is a goto loop and the built flag a local, as in the ROM: the
 * flag stays in a saved register and nothing else is hoisted out of the walk. */

#define GFX_CMD(a, b) { Gfx *_g = gDList++; _g->w0 = (a); _g->w1 = (b); }

typedef struct {
    /* 0x00 */ CVector3 min;
    /* 0x0C */ CVector3 max;
} Box;

extern Gfx *gDList;             /* display list head */
extern s32 gEvenOdd;              /* matrix buffer index */
extern u8 gScene[];
extern u8 gEngineApp_m_SwooshSystem[];

extern s32 Frustum_TestBox(void *planes, Box *box);
extern void CMtxF__TransformPoint(CMtxF m, CVector3 *in, CVector3 *out);   /* transform point */
extern void CParticle__BuildMatrix(CParticle *particle, void *mtx, CCamera *camera, f32 depth);
extern void CScene__GetLightsAt(void *list, CVector3 *pos, void *out);
extern void func_0020B9F4(void *model, void *mtx, void *mtx2, s32 a, s32 b);
extern void CParticle__DrawSprite(CParticle *particle, CCamera *view);
extern void CParticle__DrawShadow(CParticle *particle, CCamera *view);
extern void Swoosh_AddParticleNode(void *list, CParticle *particle, void *mtx);

void CParticleSystem__Draw(CParticleSystem *world, CCamera *view) {
    Box box;
    CVector3 transformed;
    CParticle *node;
    CCamera *camera;
    s32 visible, built;
    f32 zScale, xyScale, xScale, maximum, height, radius;
    f32 minX, maxX, minY, maxY, minZ, maxZ;
    f32 depth;
    s32 mask;
    u32 builtFlag;

    if (world->m_DrawList.m_pHead != 0) {
        GFX_CMD(0xE7000000, 0)
        GFX_CMD(0xE3001A01, 0x20)
        GFX_CMD(0xE3001801, 0x80)
        GFX_CMD(0xE3000A01, 0)
        GFX_CMD(0xD9F9FBFF, 0)
        node = world->m_DrawList.m_pHead;
        mask = view->m_ViewParams.m_VisBits;
        builtFlag = 0x100000;
        if (node != 0) {
        next:
            visible = 0;
            built = node->m_dwFlags & builtFlag;
            if (node->header.m_pCurrentRegion == 0 || (mask & node->header.m_pCurrentRegion->m_VisBits)) {
                if (!(node->m_dwFlags & 0x400000) && node->m_pEffect->m_pModel == 0) {
                    zScale = node->m_vSize.z;
                    xyScale = node->m_vSize.y * zScale;
                    xScale = node->m_vSize.x * zScale;
                    if (!(xScale <= xyScale)) xyScale = xScale;
                    maximum = zScale;
                    if (!(xyScale <= maximum)) maximum = xyScale;
                    height = maximum * 1.4142135f;
                    radius = height * 0.5f;
                    minX = node->header.m_vPos.x - radius;
                    box.min.x = minX;
                    maxX = node->header.m_vPos.x + radius;
                    box.max.x = maxX;
                    minY = node->header.m_vPos.y - radius;
                    box.min.y = minY;
                    maxY = node->header.m_vPos.y + height;
                    box.max.y = maxY;
                    minZ = node->header.m_vPos.z - radius;
                    box.min.z = minZ;
                    maxZ = node->header.m_vPos.z + radius;
                    box.max.z = maxZ;
                    if (minX < view->m_ViewMax.x && view->m_ViewMin.x < maxX &&
                        minZ < view->m_ViewMax.z && view->m_ViewMin.z < maxZ &&
                        minY < view->m_ViewMax.y && view->m_ViewMin.y < maxY &&
                        Frustum_TestBox(view->m_ViewVolume, &box)) {
                        visible = 1;
                    }
                } else visible = 1;
            }
            do {
                if (visible) {
                    if (!built) {
                        if (gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_nCount == 1) {
                            camera = gEngineApp_m_Game_m_CameraPool.m_ActiveList.m_pHead;
                            CMtxF__TransformPoint(camera->m_mfView, &node->header.m_vPos, &transformed);
                            depth = transformed.z;
                            if (depth < 0.0f) depth = -depth;
                            CParticle__BuildMatrix(node, node->m_mtxDraw[gEvenOdd], camera, depth);
                        } else {
                            CParticle__BuildMatrix(node, node->m_mtxDraw[gEvenOdd], 0, 0.0f);
                        }
                        node->m_dwFlags |= builtFlag;
                    }
                    if (node->m_pEffect->m_pModel != 0) {
                        CScene__GetLightsAt(gScene, &node->header.m_vPos, node->m_Lights[gEvenOdd]);
                        func_0020B9F4(node->m_pEffect->m_pModel, node->m_mtxDraw[gEvenOdd], node->m_Lights[gEvenOdd], 0, -1);
                    } else {
                        CParticle__DrawSprite(node, view);
                    }
                    if (node->m_pEffect->m_dwFlags & builtFlag) {
                        CParticle__DrawShadow(node, view);
                    }
                    if (!node->m_ActiveSwooshes || built) break;
                    if (!(node->m_pEffect->m_dwFlags & 0x10000)) {
                        Swoosh_AddParticleNode(gEngineApp_m_SwooshSystem, node, node->m_mtxDraw[gEvenOdd]);
                    }
                } else {
                    if (!node->m_ActiveSwooshes || built) break;
                    if (gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_nCount == 1) {
                        camera = gEngineApp_m_Game_m_CameraPool.m_ActiveList.m_pHead;
                        CMtxF__TransformPoint(camera->m_mfView, &node->header.m_vPos, &transformed);
                        depth = transformed.z;
                        if (depth < 0.0f) depth = -depth;
                        CParticle__BuildMatrix(node, node->m_mtxDraw[gEvenOdd], camera, depth);
                    } else {
                        CParticle__BuildMatrix(node, node->m_mtxDraw[gEvenOdd], 0, 0.0f);
                    }
                    node->m_dwFlags |= builtFlag;
                    if (!(node->m_pEffect->m_dwFlags & 0x10000)) {
                        Swoosh_AddParticleNode(gEngineApp_m_SwooshSystem, node, node->m_mtxDraw[gEvenOdd]);
                    }
                }
            } while (0);
            node = node->m_pDrawNext;
            if (node != 0) goto next;
        }
        GFX_CMD(0xE7000000, 0)
        GFX_CMD(0xE3000A01, 0x100000)
        GFX_CMD(0xE3001801, 0)
        GFX_CMD(0xE3001A01, 0x10)
    }
}
