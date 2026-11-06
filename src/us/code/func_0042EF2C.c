/* REAL Demo_Start size 0x298 (0x0042EF2C-0x0042F1C4; splat cut it into Demo_Start + func_0042F0E4). Matches whole (checked with the size overridden). The settings loop is written out in the function (not an inline) so both loops share the counter i ($a2), as in the ROM. */
#include "common.h"
#include "structs/demo.h"
#include "structs/game.h"

extern s32 D_800B6D60;

extern void Demo_DecodeButtons();
extern void Demo_DecodeStick();
extern void Demo_DecodeMatch();
extern void GameSettings_SetDefaults(GameSettings *setup);
extern s32 Random_Next(void);
static __inline__ void initSetting(DemoStream *t, DemoHeader *config, s32 index, void (*handler)(), s32 value) {
    t->bits = (u8 *)config;
    t->player = index;
    t->frames = 1;
    t->decode = handler;
    t->unk18 = 0;
    t->unk1C = 0;
    t->pos = value;
}

/* Starts a match from config: sets up the per-player settings and the game setup. */
void Demo_Start(DemoHeader *arg) {
    DemoPlayer *s = &D_800F56B0;
    GameSettings *setup;
    s32 i;
    s32 r;

    DemoHeader *owner = arg;
    DemoHeader *config = arg;

    D_800B6D60++;
    s->unk4 = 0;
    s->playing = 1;
    s->done = 0;
    s->fastForward = 1;
    if (s->saved == 0) {
        s->saved = 1;
        s->settings = *(GameSettingsBlock *)&D_8011AAD8;
    }
    {
        GameSettings *g = &D_8011AAD8;
        for (i = 0; i < 4; i++) {
            g->defaultSlot.m_bActive = 0;
            g->slots[i].m_bActive = 0;
            initSetting(&s->buttons[i], owner, i, Demo_DecodeButtons, config->buttonsStart[i]);
            initSetting(&s->stickX[i], owner, i, Demo_DecodeStick, config->stickXStart[i]);
            initSetting(&s->stickY[i], owner, i, Demo_DecodeStick, config->stickYStart[i]);
        }
    }
    initSetting(&s->match, owner, 0, Demo_DecodeMatch, config->matchStart);
    s->playerCount = config->playerCount;
    s->unk1B8 = 0;
    s->countdown = config->frames;
    s->time = 0;
    s->checksum = 0;
    D_8011AAD8.matchMode = config->matchMode;
    setup = &D_8011AAD8;
    GameSettings_SetDefaults(setup);
    if (s->playerCount >= 2) {
        do {
            for (i = 0; i < s->playerCount; i++) {
                setup->slots[i].m_bActive = 1;
                setup->slots[i].m_nCharacter = config->characters[i];
            }
            D_8011AAD8.multiplayer = 1;
        } while (0);
    } else {
        setup->defaultSlot.m_bActive = 1;
        setup->multiplayer = 0;
    }
    setup = &D_8011AAD8;
    setup->unk17 = 0;
    setup->playerCount = s->playerCount;
    setup->cheatFlags = 12;
    r = Random_Next() % 9;
    setup->dmTextureSet = r;
    setup->tagTextureSet = r;
    setup->teamTextureSet = r;
}
