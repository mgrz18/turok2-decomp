/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0044BAC0.c, func_0044BAC0, with Turok 2's layouts. */
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

extern char D_800F5A50[];
extern char D_800F5A80[];
extern s8 D_800F6750;     /* pak queue state */
extern char D_800F6758[]; /* pak OSMesgQueue */
extern char D_800F6798[]; /* SI OSMesgQueue */
extern char D_800F6770[];
extern char D_800F5E9C[];
extern u8 D_800B6CF1;     /* controller bit pattern */
extern PortStatus D_800F6778[];
extern s16 D_800F5EB8;    /* lowest port with a controller */
extern u8 D_800B6CF2;     /* number of controllers */
extern u8 D_800B6CF0;     /* controllers initialised */
extern s32 D_800F6788[];
extern s32 D_800F67B0[];
extern s32 D_800F5EA8[];
extern Ctl D_800F5EC0[];

s32 func_0026EB3C(void *dst, void *src, s32 n);
void func_0029B030(void *mq, void *msgBuf, s32 count); /* osCreateMesgQueue */
void func_0029B950(s32 event, void *mq, s32 msg);     /* osSetEventMesg */
s32 func_00298A40(void *mq, u8 *bitpattern, PortStatus *status); /* osContInit */
s32 osSendMesg(void *mq, void *msg, s32 flag);
void func_00285A80(s32);
s32 func_004301C8(Ctl *, s32);

/* Initialises the controllers; returns the lowest port with one plugged in, or -1. */
s16 func_00430070(void) {
    s32 i;

    func_0026EB3C(D_800F5A50, D_800F5A80, 8);
    D_800F6750 = 2;
    func_0029B030(D_800F6798, D_800F6770, 1);
    func_0029B030(D_800F6758, D_800F5E9C, 1);
    func_0029B950(5, D_800F6798, 1);
    func_00298A40(D_800F6798, &D_800B6CF1, D_800F6778);
    osSendMesg(D_800F6758, 0, 0);
    D_800F5EB8 = -1;
    D_800B6CF2 = 0;
    for (i = 3; i >= 0; i--) {
        D_800F6788[i] = 0;
        D_800F67B0[i] = 0;
        D_800F5EA8[i] = 0;
        func_00285A80(i);
        if (func_004301C8(&D_800F5EC0[i], i) != 0) {
            D_800F5EB8 = i;
            D_800B6CF2++;
        }
    }
    D_800B6CF0 = 1;
    return D_800F5EB8;
}
