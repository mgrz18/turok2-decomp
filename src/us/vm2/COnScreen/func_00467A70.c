/* Second VM module copy of COnScreen__Construct (same source, linked 0x0042EA70). */
#include "common.h"
#include "globals/hud.h"
#include "structs/player.h"

extern void func_00467AD4(void);
extern void COnScreen__LoadFontTexture(void);
extern void COnScreen__BuildCharacterLookUp(COnScreen *hud, u8 *anim, void *item);

void COnScreen__Construct_2(COnScreen *hud) {
    hud->m_pmeTinyFont = 0;
    hud->m_pmeSmallFont = 0;
    hud->m_pmeNormalFont = 0;
    hud->m_pmeKanjiFont = 0;
    func_00467AD4();
    COnScreen__LoadFontTexture();
    COnScreen__BuildCharacterLookUp(hud, sCharacterSmallLookUpPairs, hud->m_CharacterSmallLookUpPos);
    COnScreen__BuildCharacterLookUp(hud, sCharacterLookUpPairs, hud->m_CharacterSmallLookUpPos + 0x80);
}
