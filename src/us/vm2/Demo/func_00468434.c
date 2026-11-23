/* The second VM module's copy of src/us/vm1/Demo/func_0042F514.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/demo.h"
#include "structs/game.h"
#include "globals/game.h"

/* Ends a demo: puts back the game settings saved when it started. */
void func_00468434(void) {
    DemoPlayer *s = &gDemoPlayer;

    if (s->saved != 0) {
        s->saved = 0;
        *(GameSettingsBlock *)&gEngineApp_m_Settings = s->settings;
    }
    s->active = 0;
    s->unk4 = 0;
    s->playing = 0;
}
