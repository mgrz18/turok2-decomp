/* WRONG CUT: one function 0x00422484-0x00422608 (size 0x184); splat cut its tail (the last range
 * tests and the copy loop) into func_00422574. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/pak.h"


s32 CharNoteMenu_UpdateRank_2(MenuItem *item) {
    s32 score = gPakProfileRead.kills;
    char *src = NULL;
    char *dst;
    s32 i;

    if (score >= 0 && score < 50) {
        src = sCharNoteMenuText_Pathetic;
    } else if (score >= 50 && score < 150) {
        src = sCharNoteMenuText_Poor;
    } else if (score >= 150 && score < 300) {
        src = sCharNoteMenuText_Harmless;
    } else if (score >= 300 && score < 500) {
        src = sCharNoteMenuText_Nuisance;
    } else if (score >= 500 && score < 800) {
        src = sCharNoteMenuText_Acceptable;
    } else if (score >= 800 && score < 1300) {
        src = sCharNoteMenuText_Average;
    } else if (score >= 1300 && score < 2000) {
        src = sCharNoteMenuText_Good;
    } else if (score >= 2000 && score < 3000) {
        src = sCharNoteMenuText_Excellent;
    } else if (score >= 3000 && score < 4500) {
        src = sCharNoteMenuText_Dangerous;
    } else if (score >= 4500 && score < 6500) {
        src = sCharNoteMenuText_Brutal;
    } else if (score >= 6500 && score < 9999) {
        src = sCharNoteMenuText_Savage;
    } else if (score >= 9999) {
        src = sCharNoteMenuText_Elite;
    }
    dst = item->text;
    dst += 8;
    if (src != NULL) {
        for (i = 0; i < 11; i++) {
            *dst++ = *src++;
        }
    }
    return 0;
}
