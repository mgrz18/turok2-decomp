#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/cinema.h"


extern Cinema *D_800C1BB0;
extern CPlayer *D_80119890;          /* current player */
extern u8 D_801198B0[];
extern u8 D_800F5A50[];
/* The world and fields of it (see CScene__UpdateWarp's World). D_80110020 and
 * D_80110048 are arrays because the ROM accesses them as struct members: a
 * store through a pointer may alias them, so their loads stay after it. */
extern u8 D_800F7078[];             /* the world */
extern s32 D_80110020[];            /* world +0x18FA8: the current warp id */
extern s32 D_80110030;              /* world +0x18FB8: selectionFlags */
extern u8 D_80110034[];             /* world +0x18FBC: selectionSpawn */
extern s32 D_80110048[];            /* world +0x18FD0: selectionValues */

void CPlayer__ResetControlState(CPlayer *player);
s32 CScene__GetRegionIndex(void *world, s32 region);
void CCameraPool__ReleaseAllTexts(void *game);
void RumblePool_FreeAll(void *arg0);
s32 CScene__FindNearestWarpDest(void *world, s32 *out, s32 id, void *spawn);
void CScene__LoadLevel(void *world, s32 id, s32 flags);
s32 Cinema_NoWaitCues(void);

/* Runs once (until the state is cleared): resets the current player when
 * asked, remembering where it starts the first time; warps to the pending
 * warp id unless it is the current one; then marks the state started. */
void Cinema_Start(void) {
    CPlayer *player;
    s32 warp;
    void *world;
    s32 out[3];

    warp = D_800C1BB0->warpId;
    if (D_800C1BB0->started != 0) {
        return;
    }
    if (D_800C1BB0->resetPlayer != 0) {
        player = D_80119890;
        if (player != NULL) {
            CPlayer__ResetControlState(player);
            if (D_800C1BB0->saveStart != 0 && D_800C1BB0->startLevel == -1
                && D_800C1BB0->startWarp == D_800C1BB0->startLevel) {
                D_800C1BB0->startLevel = D_80110048[0];
                D_800C1BB0->startWarp = D_80110020[0];
                D_800C1BB0->restored = 0;
                D_800C1BB0->startPos = player->m_CharacterActor.header.m_vPos;
                D_800C1BB0->startRotY = player->m_CharacterActor.m_RotY;
                D_800C1BB0->startRegion = CScene__GetRegionIndex(D_800F7078, player->m_CharacterActor.header.m_pCurrentRegion);
            }
            player->m_WeaponActor.m_AI.m_VarBuffer[0] = 0;
            player->m_WeaponActor.m_AI.m_VarBuffer[1] = 0;
            player->m_WeaponActor.m_AI.m_VarBuffer[5] = 0;
        }
        CCameraPool__ReleaseAllTexts(D_801198B0);
        RumblePool_FreeAll(D_800F5A50);
    }
    if (warp != -1) {
        world = D_800F7078;
        if (D_80110020[0] != warp) {
            out[0] = 0;
            out[1] = 0;
            out[2] = 0;
            D_80110048[0] = CScene__FindNearestWarpDest(world, out, warp, D_80110034);
            D_80110030 = D_80110048[0] != 0;
            CScene__LoadLevel(world, warp, D_80110030);
        }
    }
    D_800C1BB0->restoreAtEnd = Cinema_NoWaitCues();
    D_800C1BB0->unk50 = 1;
    D_800C1BB0->warpId = -1;
    D_800C1BB0->started = 1;
}
