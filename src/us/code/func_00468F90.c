/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0044BAC0.c, func_0044BAC0, with Turok 2's layouts. */
/* Second VM module copy of func_00430070 (same source, linked 0xE0 lower). */
#include "common.h"
#include "structs/controller.h"


/* Controller / rumble slot, 0x224 bytes (Controller in func_00285AE0). */

extern char D_800F5A50[];
extern char D_800F5A80[];
extern s8 D_800F6750;     /* pak queue state */
extern char D_800F6758[]; /* pak OSMesgQueue */
extern char D_800F6798[]; /* SI OSMesgQueue */
extern char D_800F6770[];
extern char D_800F5E9C[];
extern u8 D_800B6CF1;     /* controller bit pattern */
extern s16 D_800F5EB8;    /* lowest port with a controller */
extern u8 D_800B6CF2;     /* number of controllers */
extern u8 D_800B6CF0;     /* controllers initialised */
extern s32 D_800F6788[];
extern s32 D_800F67B0[];
extern s32 D_800F5EA8[];

s32 func_0026EB3C(void *dst, void *src, s32 n);
void func_0029B030(void *mq, void *msgBuf, s32 count); /* osCreateMesgQueue */
void func_0029B950(s32 event, void *mq, s32 msg);     /* osSetEventMesg */
s32 func_00298A40(void *mq, u8 *bitpattern, OSContStatus *status); /* osContInit */
s32 osSendMesg(void *mq, void *msg, s32 flag);
void func_00285A80(s32);
s32 func_004300E8(ControllerProfile *, s32);

/* Initialises the controllers; returns the lowest port with one plugged in, or -1. */
s16 func_00468F90(void) {
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
        if (func_004300E8(&D_800F5EC0[i], i) != 0) {
            D_800F5EB8 = i;
            D_800B6CF2++;
        }
    }
    D_800B6CF0 = 1;
    return D_800F5EB8;
}
