#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/pak.h"

/* Menu item: append the rank title earned for the number of kills (11 chars). */


s32 CharNoteMenu_UpdateRank_2(MenuItem *item) {
    s32 kills = gPakProfileRead.kills;
    char *rank = NULL;
    char *dst;
    s32 i;

    if (kills >= 0 && kills < 50) {
        rank = sCharNoteMenuText_Pathetic;
    } else if (kills >= 50 && kills < 150) {
        rank = sCharNoteMenuText_Poor;
    } else if (kills >= 150 && kills < 300) {
        rank = sCharNoteMenuText_Harmless;
    } else if (kills >= 300 && kills < 500) {
        rank = sCharNoteMenuText_Nuisance;
    } else if (kills >= 500 && kills < 800) {
        rank = sCharNoteMenuText_Acceptable;
    } else if (kills >= 800 && kills < 1300) {
        rank = sCharNoteMenuText_Average;
    } else if (kills >= 1300 && kills < 2000) {
        rank = sCharNoteMenuText_Good;
    } else if (kills >= 2000 && kills < 3000) {
        rank = sCharNoteMenuText_Excellent;
    } else if (kills >= 3000 && kills < 4500) {
        rank = sCharNoteMenuText_Dangerous;
    } else if (kills >= 4500 && kills < 6500) {
        rank = sCharNoteMenuText_Brutal;
    } else if (kills >= 6500 && kills < 9999) {
        rank = sCharNoteMenuText_Savage;
    } else if (kills >= 9999) {
        rank = sCharNoteMenuText_Elite;
    }
    dst = item->text;
    dst += 8;
    if (rank != NULL) {
        for (i = 0; i < 11; i++) {
            *dst++ = *rank++;
        }
    }
    return 0;
}
