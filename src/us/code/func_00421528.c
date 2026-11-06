/* D_80110060 is declared as an array so its store keeps its place after the save buffer reads (a scalar is assumed not to alias them and is hoisted). */
#include "common.h"
#include "structs/player.h"
#include "structs/world.h"
#include "structs/game.h"
#include "structs/save.h"

extern CScene gScene;
extern s32 D_80110060[];
extern u8 D_800EBAC0[];

void SaveGame_Restore(CPlayer *p) {
    CScene *level = &gScene;
    u8 keep;
    s32 i;
    u8 *src;
    u8 *dst;

    keep = D_8011AAD8.unk180;
    p->m_Data = D_800C201C->player;
    *(GameSettingsBlock *)&D_8011AAD8 = D_800C201C->settings;
    D_8011AAD8.match.frames = D_800C201C->frames;
    D_8011AAD8.match.seconds = D_800C201C->seconds;
    D_8011AAD8.match.minutes = D_800C201C->minutes;
    D_8011AAD8.match.hours = D_800C201C->hours;
    D_80110060[0] = D_8011AAD8.unlockedCheats;
    D_8011AAD8.unk180 = keep;
    src = D_800C201C->levelFlags;
    dst = D_800EBAC0;
    for (i = 0; i < level->unk_30; i++) {
        *dst++ = *src++;
    }
}
