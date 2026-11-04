/* WRONG CUT: the real function is func_004301C8, size 0xD0 (splat split it at func_00430250). */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0044BC58.c, func_0044BC58, with Turok 2's layouts. */
#include "common.h"
#include "structs/controller.h"


/* Controller / rumble slot, 0x224 bytes (Controller in func_00285AE0). */

/* OSContStatus, 4 bytes per port (errno bit 3 = no controller). */
typedef struct PortStatus {
    /* 0 */ u8 b[4];
} PortStatus;

extern PortStatus D_800F6778[];
s32 func_0026EB3C(void *dst, void *src, s32 n);

/* Resets a controller slot; returns whether a controller is plugged into its port. */
s32 func_004301C8(ControllerProfile *c, s32 port) {
    s32 i;

    c->port = port;
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
    for (i = 0; i < 32; i++) {
        c->buttonHistory[i].taps = 0;
        c->buttonHistory[i].age = 0;
        c->buttonHistory[i].repeat = 0x30;
        c->buttonHistory[i].held = 0;
    }
    c->connected = ((D_800F6778[port].b[3] >> 3) ^ 1) & 1;
    c->rumbleLevel = 0;
    c->rumbleTimer = 0;
    func_0026EB3C(c->rumblePool, c->rumbleNodes, 3);
    return c->connected;
}
