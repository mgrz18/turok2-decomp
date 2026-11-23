#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
extern s32 gCharNoteMenuSaving; /* local: globals/pak.h declares it; this file views gPakProfileRead_character, which the header types otherwise */

extern s8 gPakProfileRead_character[]; /* local type: the field's real type (or the member path) changes the ROM's code here */

s32 CharNoteMenu_UpdateType(MenuItem *item) {
    char *src;
    char *dst;
    s32 i;

    if (gCharNoteMenuSaving != 0) {
        item->text = sCharNoteMenuText_Empty;
    } else {
        item->text = sCharNoteMenuText_Type;
        switch (gPakProfileRead_character[0]) {
            case 0:
            default:
                src = sCharNoteMenuText_Turok;
                break;
            case 1:
                src = sCharNoteMenuText_Adon;
                break;
            case 2:
                src = sCharNoteMenuText_Gant;
                break;
            case 3:
                src = sCharNoteMenuText_FleshEater;
                break;
            case 4:
                src = sCharNoteMenuText_PurrLin;
                break;
            case 5:
                src = sCharNoteMenuText_EndTrail;
                break;
            case 6:
                src = sCharNoteMenuText_Fireborn;
                break;
            case 7:
                src = sCharNoteMenuText_Iggy;
                break;
            case 8:
                src = sCharNoteMenuText_Campaigner;
                break;
            case 9:
                src = sCharNoteMenuText_Telset;
                break;
        }
        dst = item->text;
        dst += 8;
        for (i = 0; i < 11; i++) {
            *dst++ = *src++;
        }
    }
    return 0;
}
