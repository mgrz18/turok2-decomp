/* Boundary fixed in versions/cuts.us.txt: splat starts this function 2 words early (0x285CE8: never-executed
 * `swc1 $f21,0x10($sp); sw $s0,0x14($sp)`); the real entry is 0x285CF0, as the
 * pointer `.word 0x00285CF0` in us/asm/data/AB858.rodata.s shows. */
#include "common.h"
#include "structs/game.h"
#include "globals/engine.h"

extern s32 gFogEnabled;
extern u8 gScene[];
extern u8 gEngineApp_m_Game_m_CameraPool_m_MainCamera[];
extern f32 sExpansionPakMtx[][16];        /* per-frame matrices */
extern u8 gDefaultLights[];
extern u8 sExpansionPakText[];
extern s32 gScreenWidth;              /* screen width */

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

    gFogEnabled = 0;
    id = CScene__FindObjectType(gScene, 0xE10);
    if (id == -1) {
        return;
    }
    CCamera__DisplayListSetup(gEngineApp_m_Game_m_CameraPool_m_MainCamera);
    CMtxF__SetTranslate(mtx, 0.0f, -100.0f, -500.0f);
    CMtxF__PreRotateY(mtx, view->m_Settings.modeTimer * 0.17453295f);
    CMtxF__SwapYZRows(mtx);
    CMtxF__ToMtxAffine(mtx, sExpansionPakMtx[gEvenOdd]);
    key = CScene__GetObject(gScene, id);
    Render_ResetBatches();
    Render_QueueModelByKey(key, sExpansionPakMtx[gEvenOdd], gDefaultLights, 0, -1);
    Render_FlushBatches();
    Render_DrawTextLines(view, 0, sExpansionPakText, gScreenWidth / 2, 0xBE, 0xA, 0.8f, 1.0f);
}
