/* COnScreen__Construct (the HUD's lookups and font texture), 0x0042EB50-0x0042EBB4 (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "globals/hud.h"
#include "structs/player.h"

void func_0042EBB4(void);
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

#include "m2c_macros.h"

void func_0042EBB4(void) {

}

typedef struct Lists {
    /* 0x00 */ u8 list0[0x40];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 pad44[4];
    /* 0x48 */ u8 list1[0x40];
    /* 0x88 */ s32 unk88;
} Lists;
extern u8 D_800B2C30[];
extern u8 D_800B2C40[];
extern void COverlay__Construct(void *, s32, s32);
extern void COverlay__StartScript(void *, void *);

/* Host body: this is COnScreen's boss-bar reset (m_pBoss, m_BossOverlay, m_pBoss2,
 * m_BossOverlay2), but through a view at N64 offsets. On the host COverlay holds a pointer-wide
 * m_pScript, so the second overlay was constructed over m_pBossName/m_pBoss/m_BossPercent (m_pBoss
 * left non-NULL: an empty boss bar on screen from level 1 on, once DrawBossBars tested the right
 * member) and m_pBoss2 was never cleared (round 17 f4). The same calls by member. */
void COnScreen__ResetBossBars(void *arg0) {
    COnScreen *os = (COnScreen *)arg0;

    os->m_pBoss = NULL;
    COverlay__Construct(&os->m_BossOverlay, 0, -40);
    COverlay__StartScript(&os->m_BossOverlay, D_800B2C30);
    os->m_pBoss2 = NULL;
    COverlay__Construct(&os->m_BossOverlay2, 0, -20);
    COverlay__StartScript(&os->m_BossOverlay2, D_800B2C40);
}
