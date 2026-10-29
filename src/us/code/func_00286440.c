#include "common.h"

typedef struct GfxTask {
    /* 0x000 */ u8 pad0[0xF0];
    /* 0x0F0 */ s16 type;
    /* 0x0F2 */ u8 padF2[0x110 - 0xF2];
    /* 0x110 */ s32 buffer;
    /* 0x114 */ u8 pad114[0x12C - 0x114];
    /* 0x12C */ struct GfxTask *prev;
    /* 0x130 */ struct GfxTask *next;
    /* 0x134 */ u8 pad134[0x140 - 0x134];
} GfxTask; /* size 0x140 */

typedef struct {
    /* 0x00000 */ GfxTask tasks[1];
    /* 0x00140 */ u8 pad140[0x23DF0 - 0x140];
    /* 0x23DF0 */ u8 unk23DF0[0x23FAC - 0x23DF0];
    /* 0x23FAC */ u8 unk23FAC[0x23FD4 - 0x23FAC];
    /* 0x23FD4 */ s32 unk23FD4;
    /* 0x23FD8 */ u8 pad23FD8[0x23FE8 - 0x23FD8];
    /* 0x23FE8 */ s32 unk23FE8;
    /* 0x23FEC */ s32 unk23FEC;
    /* 0x23FF0 */ s32 unk23FF0;
    /* 0x23FF4 */ s32 unk23FF4;
} GameState;

typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
} Unk8011ACB0;

extern u8 D_8011AD28[];
extern u8 D_8011B120[];
extern u8 D_800CFC80[];
extern u8 D_8011AD90[];
extern s32 D_800B6E98;
extern u8 D_8011F128[];
extern s32 D_800D8E24;
extern u8 D_8011B0C8[];
extern s32 D_80120690;
extern u8 D_8011AAD8[];
extern u32 D_800C2210; /* task count */
extern Unk8011ACB0 D_8011ACB0[];
extern s32 D_80130940;
extern s32 D_80130A88[];

extern void func_0029B030(void *, void *, s32);
extern void func_00266C5C(void *, s32, s32);
extern void func_0028ADF0(void *, void *, s32, s32, s32);
extern void func_0028AF00(void *, void *, void *);
extern void func_0028891C(GameState *);
extern void func_00297AE0(void *);
extern void func_0028D0B8(void);
extern void func_0041BA50(void *);
extern void func_00430500(void *);
extern void func_002837E0(void);
extern void func_004302A0(void);
extern void func_0042D1C0(void *);
extern void CSunFrameData__Construct(GfxTask *);
extern void func_00288318(GameState *);
extern void func_00275520(void);

void func_00286440(GameState *state) {
    u32 i;
    u32 prev;
    u32 next;

    func_0029B030(D_8011AD28, D_8011B120, 0x1000);
    func_00266C5C(D_800CFC80, 7, 0x2000);
    func_0028ADF0(D_8011AD90, D_800CFC80 + 0x2000, D_800B6E98, 0, 1);
    func_0028AF00(D_8011AD90, D_8011F128, D_8011AD28);
    state->unk23FD4 = -1;
    func_0028891C(state);
    D_800D8E24 = 0;
    func_00297AE0(D_8011B0C8);
    if (D_80120690 != 0) {
        func_0028D0B8();
        func_0041BA50(D_8011AAD8);
    }
    func_00430500(state->unk23DF0);
    i = 0;
    func_002837E0();
    func_004302A0();
    func_0042D1C0(state->unk23FAC);
    state->unk23FF0 = 0;
    state->unk23FE8 = 1;
    state->unk23FEC = 0;
    state->unk23FF4 = 0;
    D_8011ACB0->unk44 = 0;
    D_8011ACB0->unk48 = 0;
    D_80130940 = 0;
    for (; i < D_800C2210; i++) {
        prev = (i - 1 + D_800C2210) % D_800C2210;
        next = (i + 1 + D_800C2210) % D_800C2210;
        state->tasks[i].type = 2;
        state->tasks[i].buffer = D_80130A88[i];
        state->tasks[i].prev = &state->tasks[prev];
        state->tasks[i].next = &state->tasks[next];
        CSunFrameData__Construct(&state->tasks[i]);
    }
    func_00288318(state);
    func_00275520();
}
