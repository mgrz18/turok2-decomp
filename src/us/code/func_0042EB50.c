/* COnScreen__Construct (the HUD's lookups and font texture), 0x0042EB50-0x0042EBB4 (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "globals/hud.h"

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

extern void func_0042EBB4(void);
extern void COnScreen__LoadFontTexture(void);
extern void COnScreen__BuildCharacterLookUp(Hud *hud, u8 *anim, void *item);

#ifndef TARGET_PORT
void COnScreen__Construct(Hud *hud) {
    hud->unk90 = 0;
    hud->unk94 = 0;
    hud->unk98 = 0;
    hud->unk9C = 0;
    func_0042EBB4();
    COnScreen__LoadFontTexture();
    COnScreen__BuildCharacterLookUp(hud, sCharacterSmallLookUpPairs, hud->unkC8);
    COnScreen__BuildCharacterLookUp(hud, sCharacterLookUpPairs, hud->unk148);
}
#else
#include "structs/player.h"
/* The port: the same on the typed COnScreen (the Hud view above has N64 offsets: 0x90..0x9C are
 * the four font entries, 0xC8 the small and 0x148 the normal character table, which COnScreen
 * keeps as one 256-byte array). With the view the tables stayed empty and every glyph was off. */
void COnScreen__Construct(COnScreen *hud) {
    hud->m_pmeTinyFont = 0;
    hud->m_pmeSmallFont = 0;
    hud->m_pmeNormalFont = 0;
    hud->m_pmeKanjiFont = 0;
    func_0042EBB4();
    COnScreen__LoadFontTexture();
    COnScreen__BuildCharacterLookUp((Hud *)hud, sCharacterSmallLookUpPairs, hud->m_CharacterSmallLookUpPos);
    COnScreen__BuildCharacterLookUp((Hud *)hud, sCharacterLookUpPairs, hud->m_CharacterSmallLookUpPos + 0x80);
}
#endif
