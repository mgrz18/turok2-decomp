#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/cinema.h"


extern Cinema *D_800C1BB0;
extern CPlayer *D_80119890;          /* current player */
extern s32 D_8011AC88;
extern u8 gScene[];             /* the world */
extern s32 D_80110020;              /* world +0x18FA8: the current warp id */
extern s32 D_80110024;
extern s32 D_80110028;
extern s32 D_8011002C;
extern s32 D_80110048;              /* world +0x18FD0: selectionValues */
extern s32 D_8011004C;

void CScene__LoadLevel(void *world, s32 id, s32 flags);
s32 CScene__GetRegion(void *world, s32 region);
void CPlayer__UpdateOrbitCamera(CPlayer *player, CPlayer *other);

/* Puts the current player back where Cinema_Start saw it start (once,
 * when D_8011AC88 is 13 and a start was saved), warping there first if
 * needed; any other D_8011AC88 drops the saved start. */
void Cinema_RestorePlayer(void) {
    void *world;
    CPlayer *player;
    CVector3 pos;
    f32 rotY;
    s32 region;
    s32 level;
    s32 warp;

    world = gScene;
    level = D_800C1BB0->startLevel;
    warp = D_800C1BB0->startWarp;
    if (D_8011AC88 != 13) {
        D_800C1BB0->restored = 1;
        D_800C1BB0->saveStart = 0;
        D_800C1BB0->startLevel = -1;
        D_800C1BB0->startWarp = -1;
        return;
    }
    if (D_800C1BB0->restored == 0 && level != -1 && warp != -1 && D_800C1BB0->saveStart != 0) {
        D_800C1BB0->restored = 1;
        pos = D_800C1BB0->startPos;
        rotY = D_800C1BB0->startRotY;
        region = D_800C1BB0->startRegion;
        if (D_80110020 != warp) {
            CScene__LoadLevel(world, warp, 1);
        }
        player = D_80119890;
        D_80110024 = 0;
        D_80110028 = 0;
        D_8011002C = 0;
        D_80110048 = level;
        D_8011004C = level;
        if (player != NULL) {
            player->m_CharacterActor.header.m_vPos = pos;
            player->m_CharacterActor.m_RotY = rotY;
            player->m_CharacterActor.header.m_pCurrentRegion = CScene__GetRegion(world, region);
            CPlayer__UpdateOrbitCamera(player, player);
        }
        D_800C1BB0->saveStart = 0;
        D_800C1BB0->startLevel = -1;
        D_800C1BB0->startWarp = -1;
    }
}
