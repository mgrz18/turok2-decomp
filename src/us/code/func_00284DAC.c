/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802636B0_de_7.c, func_80263CF0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00284DAC(ControllerProfile *arg0);
extern u8 D_800F677B[];
extern s32 func_0026EB3C(void *arg0, void *arg1, s32 arg2);
void func_00284DAC(ControllerProfile *arg0) {
    ControllerProfile *c = arg0;
    s32 type;
    s32 bit;
    s32 bit2;
    s32 i;
    type = c->port;
    bit = ((D_800F677B[type * 4] >> 3) ^ 1) & 1;
    if (bit != 0 && c->connected == 0) {
        i = 0;
        c->port = type;
        c->connected = 0;
        c->rumblePak = 0;
        c->rumbleOn = 0;
        c->reset = 0;
        c->unk8 = 0;
        c->unkC = 0;
        c->unk10 = 0;
        c->inputStickX = 0;
        c->inputStickY = 0;
        c->stickX = 0;
        c->stickY = 0;
        c->inputPreviousButtons = 0;
        c->inputButtons = 0;
        c->inputButtonsSingle = 0;
        c->inputButtonsSingleNoHold = 0;
        c->inputButtonsDouble = 0;
        c->inputButtonsRepeat = 0;
        do {
            c->buttonHistory[i].taps = 0;
            c->buttonHistory[i].age = 0;
            c->buttonHistory[i].repeat = 0x30;
            c->buttonHistory[i].held = 0;
            i += 1;
        } while (i < 0x20);
        bit2 = ((D_800F677B[type * 4] >> 3) ^ 1) & 1;
        c->rumbleLevel = 0;
        c->rumbleTimer = 0;
        c->connected = bit2;
        func_0026EB3C(c->rumblePool, c->rumbleNodes, 3);
    }
    c->connected = bit;
}
