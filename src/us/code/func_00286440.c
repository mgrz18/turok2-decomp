#include "common.h"
#include "structs/game.h"

/* D_8011ACB0 (GameMatch in structs/game.h) as a local array view, under an
 * alias of the symbol: through it the compiler bases both stores on
 * D_8011ACB0 + 0x44, as the ROM does. */
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
extern u32 D_800C2210; /* task count */
extern Unk8011ACB0 D_8011ACB0_arr[] __asm__("D_8011ACB0");
extern s32 D_80130940;
extern s32 D_80130A88[];

extern void func_0029B030(void *, void *, s32);
extern void func_00266C5C(void *, s32, s32);
extern void func_0028ADF0(void *, void *, s32, s32, s32);
extern void func_0028AF00(void *, void *, void *);
extern void func_0028891C(CEngineApp *);
extern void func_00297AE0(void *);
extern void func_0028D0B8(void);
extern void func_0041BA50(void *);
extern void func_00430500(void *);
extern void func_002837E0(void);
extern void func_004302A0(void);
extern void func_0042D1C0(void *);
extern void CSunFrameData__Construct(CFrameData *);
extern void func_00288318(CEngineApp *);
extern void func_00275520(void);

void func_00286440(CEngineApp *state) {
    u32 i;
    u32 prev;
    u32 next;

    func_0029B030(D_8011AD28, D_8011B120, 0x1000);
    func_00266C5C(D_800CFC80, 7, 0x2000);
    func_0028ADF0(D_8011AD90, D_800CFC80 + 0x2000, D_800B6E98, 0, 1);
    func_0028AF00(D_8011AD90, D_8011F128, D_8011AD28);
    state->m_Settings.prevMode = -1;
    func_0028891C(state);
    D_800D8E24 = 0;
    func_00297AE0(D_8011B0C8);
    if (D_80120690 != 0) {
        func_0028D0B8();
        func_0041BA50(&D_8011AAD8);
    }
    func_00430500(&state->m_Game.m_DebugLines);
    i = 0;
    func_002837E0();
    func_004302A0();
    func_0042D1C0(&state->m_Settings.m_Sun);
    state->m_Settings.unk1C8 = 0;
    state->m_Settings.ready = 1;
    state->m_Settings.unk1C4 = 0;
    state->m_Settings.paused = 0;
    D_8011ACB0_arr->unk44 = 0;
    D_8011ACB0_arr->unk48 = 0;
    D_80130940 = 0;
    for (; i < D_800C2210; i++) {
        prev = (i - 1 + D_800C2210) % D_800C2210;
        next = (i + 1 + D_800C2210) % D_800C2210;
        state->m_FrameData[i].m_Msg.m_Type = 2;
        state->m_FrameData[i].m_pFrameBuffer = D_80130A88[i];
        state->m_FrameData[i].m_pPrev = &state->m_FrameData[prev];
        state->m_FrameData[i].m_pNext = &state->m_FrameData[next];
        CSunFrameData__Construct(&state->m_FrameData[i]);
    }
    func_00288318(state);
    func_00275520();
}
