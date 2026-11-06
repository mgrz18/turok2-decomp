#include "common.h"
#include "structs/menu.h"

extern char D_800B42D8[];
extern char D_800C0E70[]; /* "%d"-style format */
extern s32 gEngineApp_m_Settings_match_teamScore; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, s32 value);

/* Prints gEngineApp_m_Settings_match_teamScore over the last four characters of the label. */
s32 FinalScoreMenu_BlueScoreLabel(MenuItem *a) {
    a->text = D_800B42D8;
    sprintf(a->text + (MenuItem_TextLength(a) - 4), D_800C0E70, gEngineApp_m_Settings_match_teamScore);
    return 0;
}
