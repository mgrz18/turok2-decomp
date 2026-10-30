/* WRONG CUT: the real function is func_004301C8, size 0xD0 (splat split it at func_00430250). */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0044BC58.c, func_0044BC58, with Turok 2's layouts. */
#include "common.h"

typedef struct CtlEnt {
    /* 0 */ u8 a;
    /* 1 */ u8 b;
    /* 2 */ u8 c;
    /* 3 */ u8 d;
} CtlEnt;

/* Controller / rumble slot, 0x224 bytes (Controller in func_00285AE0). */
typedef struct Ctl {
    /* 0x000 */ s32 present;
    /* 0x004 */ s8 port;
    /* 0x005 */ u8 pad5[3];
    /* 0x008 */ s32 f8;
    /* 0x00C */ s32 fC;
    /* 0x010 */ s32 f10;
    /* 0x014 */ s32 f14;
    /* 0x018 */ s32 f18;
    /* 0x01C */ s32 f1C;
    /* 0x020 */ s32 f20;
    /* 0x024 */ s32 f24;
    /* 0x028 */ s32 f28;
    /* 0x02C */ CtlEnt ent[32];
    /* 0x0AC */ u8 padAC[0xC4 - 0xAC];
    /* 0x0C4 */ u8 fC4;
    /* 0x0C5 */ u8 fC5;
    /* 0x0C6 */ u8 fC6;
    /* 0x0C7 */ u8 fC7;
    /* 0x0C8 */ s32 rumbleOk;
    /* 0x0CC */ s32 rumbleState;
    /* 0x0D0 */ s32 rumbleAmount;
    /* 0x0D4 */ s32 rumbleTimer;
    /* 0x0D8 */ u8 motor[0x140 - 0xD8];
    /* 0x140 */ u8 label[0x2C];
    /* 0x16C */ u8 label2[0xB4];
    /* 0x220 */ s32 f220;
} Ctl;

/* OSContStatus, 4 bytes per port (errno bit 3 = no controller). */
typedef struct PortStatus {
    /* 0 */ u8 b[4];
} PortStatus;

extern PortStatus D_800F6778[];
s32 func_0026EB3C(void *dst, void *src, s32 n);

/* Resets a controller slot; returns whether a controller is plugged into its port. */
s32 func_004301C8(Ctl *c, s32 port) {
    s32 i;

    c->port = port;
    c->present = 0;
    c->rumbleOk = 0;
    c->rumbleState = 0;
    c->f220 = 0;
    c->f8 = 0;
    c->fC = 0;
    c->f10 = 0;
    c->fC4 = 0;
    c->fC5 = 0;
    c->fC6 = 0;
    c->fC7 = 0;
    c->f14 = 0;
    c->f18 = 0;
    c->f1C = 0;
    c->f20 = 0;
    c->f24 = 0;
    c->f28 = 0;
    for (i = 0; i < 32; i++) {
        c->ent[i].a = 0;
        c->ent[i].b = 0;
        c->ent[i].c = 0x30;
        c->ent[i].d = 0;
    }
    c->present = ((D_800F6778[port].b[3] >> 3) ^ 1) & 1;
    c->rumbleAmount = 0;
    c->rumbleTimer = 0;
    func_0026EB3C(c->label, c->label2, 3);
    return c->present;
}
