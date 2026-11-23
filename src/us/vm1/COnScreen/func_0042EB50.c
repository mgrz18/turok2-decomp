/* COnScreen__Construct (the HUD's lookups and font texture), 0x0042EB50-0x0042EBB4 (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "globals/hud.h"
#include "structs/player.h"

extern void func_0042EBB4(void);
extern void COnScreen__LoadFontTexture(void);
extern void COnScreen__BuildCharacterLookUp(COnScreen *hud, u8 *anim, void *item);

/* 0xC8 is the small and 0x148 the normal character table: COnScreen keeps them as one
 * 256-byte array. (The port: with the old N64-offset view the tables stayed empty and every
 * glyph was off.) */
void COnScreen__Construct(COnScreen *hud) {
    hud->m_pmeTinyFont = 0;
    hud->m_pmeSmallFont = 0;
    hud->m_pmeNormalFont = 0;
    hud->m_pmeKanjiFont = 0;
    func_0042EBB4();
    COnScreen__LoadFontTexture();
    COnScreen__BuildCharacterLookUp(hud, sCharacterSmallLookUpPairs, hud->m_CharacterSmallLookUpPos);
    COnScreen__BuildCharacterLookUp(hud, sCharacterLookUpPairs, hud->m_CharacterSmallLookUpPos + 0x80);
}
