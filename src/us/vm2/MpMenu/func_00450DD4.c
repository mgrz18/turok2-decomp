/* The second VM module's copy of src/us/vm1/MpMenu/func_00417DD4.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"

#include "globals/game.h"
extern s32 MenuItem_TextLength_2(MenuItem *a);
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
extern void sprintf(char *dst, char *fmt, ...);

/* Prints gEngineApp_m_Settings_match_teamScore over the last four characters of the label. */
s32 FinalScoreMenu_BlueScoreLabel_2(MenuItem *a) {
    a->text = sFinalScoreMenuText_BlueBloodScore;
    sprintf(a->text + (MenuItem_TextLength_2(a) - 4), "%04d", gEngineApp_m_Settings_match_teamScore[0]);
    return 0;
}
