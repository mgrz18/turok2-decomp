/* Boundary fixed in versions/cuts.us.txt: splat starts this function 2 words early (0x285CE8: never-executed
 * `swc1 $f21,0x10($sp); sw $s0,0x14($sp)`); the real entry is 0x285CF0, as the
 * pointer `.word 0x00285CF0` in us/asm/data/AB858.rodata.s shows. */
#include "common.h"
#include "structs/game.h"

extern s32 D_800AB8A0;
extern u8 D_800F7078[];
extern u8 D_801198F0[];
extern s32 D_800B6D1C;              /* frame buffer index */
extern f32 D_800F67C0[][16];        /* per-frame matrices */
extern u8 D_800AB0E8[];
extern u8 D_800B36F4[];
extern s32 D_800C2030;              /* screen width */

extern s32 CScene__FindObjectType(void *, s32);
extern void CCamera__DisplayListSetup(void *);
extern void CMtxF__SetTranslate(f32 *matrix, f32 x, f32 y, f32 z);
extern void CMtxF__PreRotateY(f32 *matrix, f32 angle);
extern void CMtxF__SwapYZRows(f32 *matrix);
extern void CMtxF__ToMtxAffine(f32 *src, f32 *dst);
extern s32 CScene__GetObject(void *, s32);
extern void Render_ResetBatches(void);
extern void Render_QueueModelByKey(s32 key, void *matrix, void *lights, s32 lod, s32 pass);
extern void Render_FlushBatches(void);
extern s32 Render_DrawTextLines(CEngineApp *, s32, void *, s32, s32, s32, f32, f32);

void EngineMode03_ExpansionPak_Draw(CEngineApp *view) {
    f32 mtx[16];
    s32 id;
    s32 key;

    D_800AB8A0 = 0;
    id = CScene__FindObjectType(D_800F7078, 0xE10);
    if (id == -1) {
        return;
    }
    CCamera__DisplayListSetup(D_801198F0);
    CMtxF__SetTranslate(mtx, 0.0f, -100.0f, -500.0f);
    CMtxF__PreRotateY(mtx, view->m_Settings.modeTimer * 0.17453295f);
    CMtxF__SwapYZRows(mtx);
    CMtxF__ToMtxAffine(mtx, D_800F67C0[D_800B6D1C]);
    key = CScene__GetObject(D_800F7078, id);
    Render_ResetBatches();
    Render_QueueModelByKey(key, D_800F67C0[D_800B6D1C], D_800AB0E8, 0, -1);
    Render_FlushBatches();
    Render_DrawTextLines(view, 0, D_800B36F4, D_800C2030 / 2, 0xBE, 0xA, 0.8f, 1.0f);
}
