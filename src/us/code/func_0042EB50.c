/* COnScreen__Construct (the HUD's lookups and font texture), 0x0042EB50-0x0042EBB4 (splat had it inside the data D_0042D50C). */
#include "common.h"

typedef struct Hud {
    /* 0x000 */ u8 pad0[0x90];
    /* 0x090 */ s32 unk90;
    /* 0x094 */ s32 unk94;
    /* 0x098 */ s32 unk98;
    /* 0x09C */ s32 unk9C;
    /* 0x0A0 */ u8 padA0[0xC8 - 0xA0];
    /* 0x0C8 */ u8 unkC8[0x148 - 0xC8];
    /* 0x148 */ u8 unk148[0x80];
} Hud;

extern u8 D_800B2B78[];
extern u8 D_800B2AC4[];

extern void func_0042EBB4(void);
extern void COnScreen__LoadFontTexture(void);
extern void COnScreen__BuildCharacterLookUp(Hud *hud, u8 *anim, void *item);

void COnScreen__Construct(Hud *hud) {
    hud->unk90 = 0;
    hud->unk94 = 0;
    hud->unk98 = 0;
    hud->unk9C = 0;
    func_0042EBB4();
    COnScreen__LoadFontTexture();
    COnScreen__BuildCharacterLookUp(hud, D_800B2B78, hud->unkC8);
    COnScreen__BuildCharacterLookUp(hud, D_800B2AC4, hud->unk148);
}
