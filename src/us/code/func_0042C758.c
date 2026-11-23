/* WRONG CUT: the function starts at 0x0042C758 (versions/cuts.us.txt calls its two first words,
 * the prologue addiu sp,-24 / sw s0, a stray; the body at 0x42C760 saves ra at 20(sp) and pops 24).
 * REAL CScene__SetupLighting size 0x44 at CScene__RequestWarp+0x58 */
#include "common.h"
#include "structs/world.h"
#include "globals/render.h"




extern void CScene__LoadLightSettings(CScene *menu);

void CScene__SetupLighting(CScene *menu) {
    CScene__LoadLightSettings(menu);
    gFogEnabled = (menu->m_pLevelHeader->m_Flags >> 2) & 1;
}
