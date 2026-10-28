#include "common.h"
#include "structs/camera.h"

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

typedef struct {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ void *frameBuffer;
} GfxTask;

#define GFX_CMD(a, b) { Gfx *_g = D_800D8E24++; _g->w0 = (a); _g->w1 = (b); }

extern s32 D_8011ACA4;
extern f32 D_800F5A44;              /* seconds the screen is cleared for (splat: D_800F5A40 + 4) */
extern f32 D_800B6D28;              /* frame time */
extern s32 D_800B6D1C;              /* frame buffer index */
extern s32 D_800C2030;              /* screen width */
extern s32 D_800C2034;              /* screen height */
extern Gfx *D_800D8E24;             /* display list head */
extern GfxTask *D_800F7070;
extern s32 D_80130940;
extern u8 D_80119870[];
extern Gfx D_800AB100[];
extern Gfx D_800AB130[];

void func_0020565C(s32 unused, void *resource);
void func_0020B5B8(void);
void func_0027E000(CCamera *cam);
void func_0027E770(CCamera *cam, void *world);
void func_0027C1E8(CCamera *cam);
void func_002800A0(CCameraPool *pool, CCamera *cam);
void func_00416AA4(void *menuPool);
void func_0027F64C(CCamera *cam);
void func_0025328C(void *game, CCamera *cam);

/* Updates every camera of the pool, then the main camera. While
 * D_800F5A44 counts down, the frame is first cleared to black. */
void func_0027F2C4(CCameraPool *pool, void *world) {
    CCamera *cam;
    CCamera *main;
    s32 w;
    s32 h;

    pool->m_MainCamera.m_FogColor[0] = 0;
    pool->m_MainCamera.m_FogColor[1] = 0;
    pool->m_MainCamera.m_FogColor[2] = 0;
    if (pool->m_pmeCameras != NULL) {
        func_0020565C(0, pool->m_pmeCameras);
    }
    if (D_8011ACA4 != 0 || pool->m_ActiveList.m_nCount == 3) {
        D_800F5A44 = 30.0f;
    }
    if (D_800F5A44 != 0.0f) {
        D_800F5A44 -= D_800B6D28;
    }
    if (D_800F5A44 < 0.0f) {
        D_800F5A44 = 0.0f;
    }
    if (D_800F5A44 != 0.0f) {
        cam = pool->m_ActiveList.m_pHead;
        if (cam != NULL) {
            GFX_CMD(0xE7000000, 0)
            GFX_CMD(0xDE000000, (u32)D_800AB100)
            GFX_CMD(0xDE000000, (u32)D_800AB130)
            cam->m_vp[D_800B6D1C].vscale[0] = D_800C2030;
            cam->m_vp[D_800B6D1C].vscale[1] = D_800C2034;
            cam->m_vp[D_800B6D1C].vtrans[0] = D_800C2030;
            cam->m_vp[D_800B6D1C].vtrans[1] = D_800C2034;
            GFX_CMD(0xDC080008, (u32)&cam->m_vp[D_800B6D1C])
            w = D_800C2030;
            h = D_800C2034;
            GFX_CMD(0xED000000, ((((s32)((f32)(w - 1) * 4.0f)) & 0xFFF) << 12) | (((s32)((f32)(h - 1) * 4.0f)) & 0xFFF))
            GFX_CMD(0xFF100000 | ((w - 1) & 0xFFF), (u32)D_800F7070->frameBuffer)
            GFX_CMD(0xE3000A01, 0x300000)
            GFX_CMD(0xFCFFFFFF, 0xFFFE793C)
            GFX_CMD(0xE200001C, 0)
            GFX_CMD(0xF7000000, 0x10001)
            GFX_CMD(0xF6000000 | ((w & 0x3FF) << 14) | ((h & 0x3FF) << 2), 0)
            func_0020B5B8();
        }
    }
    for (cam = pool->m_ActiveList.m_pHead; cam != NULL; cam = cam->m_pNext) {
        func_0027E000(cam);
        if (D_80130940 == 0) {
            func_0027E770(cam, world);
        }
        func_0027C1E8(cam);
        func_002800A0(pool, cam);
        func_00416AA4(cam->m_MenuPool);
        func_0027F64C(cam);
    }
    main = &pool->m_MainCamera;
    func_0027E000(main);
    if (pool->m_ActiveList.m_nCount == 0) {
        func_0027C1E8(main);
    }
    func_002800A0(pool, main);
    if (D_80119870[0x1281] != 0) {
        cam = pool->m_ActiveList.m_pHead;
        if (cam != NULL) {
            func_0025328C(D_80119870, cam);
        }
    }
    func_00416AA4(pool->m_MainCamera.m_MenuPool);
}
