#include "common.h"
#include "structs/demo.h"
#include "structs/game.h"

/* Ends a demo: puts back the game settings saved when it started. */
void Demo_End(void) {
    DemoPlayer *s = &gDemoPlayer;

    if (s->saved != 0) {
        s->saved = 0;
        *(GameSettingsBlock *)&gEngineApp_m_Settings = s->settings;
    }
    s->active = 0;
    s->unk4 = 0;
    s->playing = 0;
}
