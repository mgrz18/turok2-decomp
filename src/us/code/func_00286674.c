/* FAKEMATCH: case 4 duplicates its body under an always-true test to stop GCC keeping the constant 2 in $s1 (see there). */
/* The engine thread: boot-time setup, then the message loop (RSP task done / retrace / pre-NMI).
 * Same role as Turok: Rage Wars' func_00291404 (thejunkman9000/Turok-Rage-Wars-Decomp, src/game/func_00291404.c). */

#include "common.h"

typedef struct {
    /* 0x00 */ s16 type;
} EngineMsg;

typedef struct {
    /* 0x00000 */ u8 pad0[0x3C4];
    /* 0x003C4 */ s32 unk3C4;
    /* 0x003C8 */ u8 pad3C8[0x23FD8 - 0x3C8];
    /* 0x23FD8 */ s32 unk23FD8;
    /* 0x23FDC */ u8 pad23FDC[4];
    /* 0x23FE0 */ u8 unk23FE0;
    /* 0x23FE1 */ u8 pad23FE1[0x23FF0 - 0x23FE1];
    /* 0x23FF0 */ s32 unk23FF0;
} Engine;

extern s32 D_80000300;          /* osTvType */
extern u8 D_80130A58[];
extern void *D_80130A88[];      /* frame buffers */
extern f32 D_8011B0A0[];
extern EngineMsg *D_8011F124;
extern s32 D_8011B110;
extern s32 D_8011AD88;
extern s32 D_8011B10C;
extern s32 D_800F6CA8;
extern s32 D_8011AD14;
extern s32 D_8011B09C;
extern s32 D_8011F130;
extern s32 D_8011F134;
extern s32 D_8011F138;
extern s32 D_800F6CA4;
extern s32 D_8011B098;
extern u32 D_800C2210;          /* frame buffer count */
extern s32 D_800C2030;          /* width */
extern s32 D_800C2034;          /* height */
extern u8 D_800F7078[];
extern u8 D_800B6CF1;
extern s32 D_8011F13C;
extern s32 D_8011F148;
extern s32 D_8011B108;
extern s32 D_8011F144;
extern s32 D_8011B114;
extern f32 D_8011AD40;
extern u8 D_8011AD28[];         /* the engine message queue */
extern void *D_8011F140;
extern u8 D_800B75F0;

extern void func_00202130(void);
extern void osCreateViManager(s32 pri);
extern void func_0029A580(s32 black);           /* osViBlack */
extern void func_00425604(void *p);
extern void func_00425220(void *p);
extern void func_0029AC90(void *frameBuffer);   /* osViSwapBuffer */
extern void func_0042F4F0(void);
extern void func_00286440(Engine *e);
extern void func_00266C5C(void *p, s32 c, s32 n);   /* memset */
extern void func_0042FE4C(void *p);
extern void func_0042E834(void *p);
extern void CCollisionInfo__Reset(void);
extern void func_0042FBA4(void *p, s32 a);
extern void func_0042E5AC(void *p, s32 a, s32 b);
extern void func_00429FA0(void *p, s32 seed);
extern void func_00430070(void);
extern void func_0042EB50(void *p);
extern void func_004263DC(void);
extern void func_00285A94(void);
extern s32 func_0028D0E0(void);
extern void func_00288BC8(Engine *e, s32 mode);
extern s32 osRecvMesg(void *mq, void *msg, s32 flag);
extern void func_00288C80(Engine *e);
extern void func_00287ABC(Engine *e);
extern void func_00285304(void);
extern void func_00285410(s32 a);
extern void func_00288C5C(Engine *e, s32 mode);

void func_00286674(Engine *e) {
    u32 i;

    func_00202130();
    osCreateViManager(0xFE);
    func_0029A580(1);
    func_00425604(D_80130A58);
    func_00425220(D_80130A58);
    func_0029AC90(D_80130A88[0]);
    func_0029A580(0);
    if (D_80000300 == 0) {
        D_8011B0A0[1] = 50.0f;
    } else {
        D_8011B0A0[1] = 60.0f;
    }
    D_8011F124 = NULL;
    func_0042F4F0();
    D_8011B110 = 2;
    D_8011AD88 = 0;
    D_8011B10C = 0;
    D_800F6CA8 = 0;
    D_8011AD14 = 0;
    D_8011B09C = 0;
    D_8011F130 = 0;
    D_8011F134 = 0;
    D_8011F138 = 0;
    D_800F6CA4 = 0;
    D_8011B0A0[0] = 0;
    D_8011B098 = 0;
    func_00286440(e);
    for (i = 0; i < D_800C2210; i++) {
        func_00266C5C(D_80130A88[i], 0, D_800C2030 * D_800C2034 * 2);
    }
    func_0042FE4C((u8 *)e + 0x22C00);
    func_0042E834((u8 *)e + 0x22BC0);
    CCollisionInfo__Reset();
    e->unk3C4 = 0xFCBFB4;
    func_0042FBA4((u8 *)e + 0x22C00, 0);
    func_0042E5AC((u8 *)e + 0x22BC0, 0, 0);
    func_00429FA0(D_800F7078, e->unk3C4);
    if (D_800B6CF1 == 0) {
        func_00430070();
    }
    func_0042EB50((u8 *)e + 0x19440);
    func_004263DC();
    D_8011F13C = 0;
    D_8011F148 = 0;
    D_8011B108 = 0;
    D_8011F144 = 1;
    D_8011B114 = 0;
    D_8011AD40 = 120.0f;
    func_00285A94();
    if (func_0028D0E0()) {
        func_00288BC8(e, 13);
    } else {
        func_00288BC8(e, 0);
    }

    for (;;) {
        osRecvMesg(D_8011AD28, &D_8011F124, 1);
        switch (D_8011F124->type) {
        case 1:
            func_00288C80(e);
            break;
        case 2:
            if ((u8 *)D_8011F124 == (u8 *)e + 0xF0) {
                D_8011F140 = e;
            } else if ((u8 *)D_8011F124 == (u8 *)e + 0x230) {
                D_8011F140 = (u8 *)e + 0x140;
            } else if ((u8 *)D_8011F124 == (u8 *)e + 0x370) {
                D_8011F140 = (u8 *)e + 0x280;
            }
            if (e->unk23FD8 == 17) {
                D_8011B110++;
            } else {
                func_00287ABC(e);
            }
            break;
        case 4:
            /* FAKEMATCH: the test is always true and both branches are the same (cross-jumping
             * merges them); without it GCC also keeps the case constant 2 in $s1 across the loop
             * (12 words differ). Found by the permuter; no natural form found. */
            if (D_8011F124 != NULL) {
                D_800B75F0 = 0;
                func_00285304();
                func_00285410(0);
                e->unk23FE0 = 1;
                func_00288C5C(e, 17);
                e->unk23FF0 = 1;
                break;
            } else {
                D_800B75F0 = 0;
                func_00285304();
                func_00285410(0);
                e->unk23FE0 = 1;
                func_00288C5C(e, 17);
                e->unk23FF0 = 1;
                break;
            }
        }
    }
}
