#include "common.h"
#include "structs/demo.h"
#include "structs/game.h"

/* Ends a demo: puts back the game settings saved when it started. */
void func_0042F514(void) {
    DemoPlayer *s = &D_800F56B0;

    if (s->saved != 0) {
        s->saved = 0;
        *(GameSettingsBlock *)&D_8011AAD8 = s->settings;
    }
    s->active = 0;
    s->unk4 = 0;
    s->playing = 0;
}
