/* Module-2 copy of GameSettings_SetDefaults (same code; callee at its link address). */
#include "common.h"
#include "structs/game.h"

extern void func_0041BBB0(CPlayerOptions *slot);

static __inline__ void InitSlot(CPlayerOptions *slot, u8 index) {
    slot->m_bActive = 0;
    slot->m_SettingX = 0x80;
    slot->m_SettingY = 0x80;
    slot->m_ControlSide = 0;
    slot->m_ControlType = 0;
    slot->m_bLookSpring = 1;
    slot->m_bAutoAim = 0;
    slot->m_nController = index;
    slot->m_nCharacter = 0;
    slot->m_WeaponSelect = 1;
    slot->m_Handicap = 0;
    func_0041BBB0(slot);
}

/* Default multiplayer setup. */
void GameSettings_SetDefaults_2(GameSettings *s) {
    s32 i;

    s->unk13 = 1;
    s->unk14 = 0x80;
    s->unk15 = 0xFA;
    s->unk16 = 0xFF;
    s->hudAlphaSetting.value = 0x80;
    s->difficulty = 0x10;
    s->unkC = 0;
    s->unk17 = 1;
    s->unk18 = 0;
    s->cheatFlags = 0;
    s->unlockedCheats = 0;
    s->unk1B = 0;
    s->unk180 = 0;
    s->dmTimeLimit = 10;
    s->dmScoreLimit = 0;
    s->dmArena = 0;
    s->dmTextureSet = 0;
    s->dmCrosshairs = 1;
    s->tagTimeLimit = 0;
    s->tagScoreLimit = 0;
    s->tagArena = 0;
    s->tagTextureSet = 0;
    s->tagCrosshairs = 1;
    s->teamTimeLimit = 10;
    s->teamScoreLimit = 0;
    s->teamArena = 0;
    s->teamTextureSet = 0;
    s->teamCrosshairs = 1;
    s->hudAlpha = s->hudAlphaSetting.value;
    s->speed = 1.0f;
    for (i = 0; i < 10; i++) {
        s->weaponEnabled[i] = 1;
    }
    s->unk3C = 0;
    s->unk3D = 3;
    s->unk3E = 0;
    s->unk3F = 3;
    for (i = 0; i < 4; i++) {
        InitSlot((CPlayerOptions *) ((u8 *) s + 0x80 + i * 0x40), i);
    }
    InitSlot((CPlayerOptions *) ((u8 *) s + 0x40), 0);
    s->defaultSlot.m_bActive = 1;
    s->multiplayer = 0;
    s->playerCount = 1;
    s->cheatFlags = 0x4000000;
    s->unlockedCheats = 0x4000000;
}
