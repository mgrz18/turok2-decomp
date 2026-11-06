#include "common.h"
#include "structs/intelligence.h"
#include "structs/player.h"
#include "gfx.h"


extern u8 D_8011AAF1;           /* multiplayer */
extern s32 D_8011ACE4;
extern s32 D_800ADB5C;
extern s32 D_800ADB30[];
extern u8 gScene[];

void CScene__SetInstanceObjectType(void *world, CPlayer *player, s32 model, s32 arg3);
void CGameObjectInstance__Draw(CPlayer *player, s32 arg1);

/* Draws the player's model: picks it (single player, tag mode or the
   character), then sets up the render state before CGameObjectInstance__Draw. */
void CPlayer__DrawCharacter(CPlayer *player, s32 arg1) {
    s32 model;
    Gfx *g;

    if (D_8011AAF1 == 0) {
        model = 0x66;
    } else if (D_8011ACE4 != 0 && player->m_pOptions->m_TagMode != 0) {
        model = D_800ADB5C;
    } else {
        model = D_800ADB30[player->m_CharacterActor.header.m_pIntelligence->m_Id];
    }
    CScene__SetInstanceObjectType(gScene, player, model, player->m_CharAnim);
    g = D_800D8E24++;
    g->w0 = 0xE7000000;
    g->w1 = 0;
    g = D_800D8E24++;
    g->w0 = 0xE3000A01;
    g->w1 = 0x100000;
    g = D_800D8E24++;
    g->w0 = 0xD9FFFFFF;
    g->w1 = 0x10000;
    g = D_800D8E24++;
    g->w0 = 0xDB040004;
    g->w1 = 2;
    g = D_800D8E24++;
    g->w0 = 0xDB04000C;
    g->w1 = 2;
    g = D_800D8E24++;
    g->w0 = 0xDB040014;
    g->w1 = 0xFFFE;
    g = D_800D8E24++;
    g->w0 = 0xDB04001C;
    g->w1 = 0xFFFE;
    CGameObjectInstance__Draw(player, arg1);
}
