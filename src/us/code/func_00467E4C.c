/* Second VM module copy of Demo_Start (linked 0x39000 lower; here the cut is whole, 0x298). The settings loop is written out in the function (not an inline) so both loops share the counter i ($a2), as in the ROM. */
#include "common.h"
#include "structs/demo.h"
#include "structs/game.h"
#include "globals/game.h"

extern s32 gAttractRunCount;

extern void Demo_DecodeButtons_2();
extern void Demo_DecodeStick_2();
extern void Demo_DecodeMatch_2();
extern void GameSettings_SetDefaults_2(GameSettings *setup);
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
void Demo_Start_2(DemoHeader *arg) {
    DemoPlayer *s = &gDemoPlayer;
    GameSettings *setup;
    s32 i;
    s32 r;

    DemoHeader *owner = arg;
    DemoHeader *config = arg;

    gAttractRunCount++;
    s->unk4 = 0;
    s->playing = 1;
    s->done = 0;
    s->fastForward = 1;
    if (s->saved == 0) {
        s->saved = 1;
        s->settings = *(GameSettingsBlock *)&gEngineApp_m_Settings;
    }
    {
        GameSettings *g = &gEngineApp_m_Settings;
        for (i = 0; i < 4; i++) {
            g->defaultSlot.m_bActive = 0;
            g->slots[i].m_bActive = 0;
            initSetting(&s->buttons[i], owner, i, Demo_DecodeButtons_2, config->buttonsStart[i]);
            initSetting(&s->stickX[i], owner, i, Demo_DecodeStick_2, config->stickXStart[i]);
            initSetting(&s->stickY[i], owner, i, Demo_DecodeStick_2, config->stickYStart[i]);
        }
    }
    initSetting(&s->match, owner, 0, Demo_DecodeMatch_2, config->matchStart);
    s->playerCount = config->playerCount;
    s->unk1B8 = 0;
    s->countdown = config->frames;
    s->time = 0;
    s->checksum = 0;
    gEngineApp_m_Settings.matchMode = config->matchMode;
    setup = &gEngineApp_m_Settings;
    GameSettings_SetDefaults_2(setup);
    if (s->playerCount >= 2) {
        do {
            for (i = 0; i < s->playerCount; i++) {
                setup->slots[i].m_bActive = 1;
                setup->slots[i].m_nCharacter = config->characters[i];
            }
            gEngineApp_m_Settings.multiplayer = 1;
        } while (0);
    } else {
        setup->defaultSlot.m_bActive = 1;
        setup->multiplayer = 0;
    }
    setup = &gEngineApp_m_Settings;
    setup->unk17 = 0;
    setup->playerCount = s->playerCount;
    setup->cheatFlags = 12;
    r = Random_Next() % 9;
    setup->dmTextureSet = r;
    setup->tagTextureSet = r;
    setup->teamTextureSet = r;
}
