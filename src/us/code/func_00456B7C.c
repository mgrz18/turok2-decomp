#include "common.h"
#include "structs/menu.h"
#include "globals/menu_desc.h"

/* Pick the menu page to go back to, from the game mode flags. */

extern s32 gPakMenuIsSave;
extern s32 gPakMenuAnyPak;
extern s32 gPakMenuIsLoad;
extern s32 gSavePortalActive;
extern s32 gPakMenuIsProfile;
extern s32 gPakMenuPort;

static __inline__ void *modePage(s32 *mode) {
    void *page;

    switch (*mode) {
    case 0:
    default:
        page = &gStatsAfterPakMenuDesc1_2;
        break;
    case 1:
        page = &gStatsAfterPakMenuDesc2_2;
        break;
    case 2:
        page = &gStatsAfterPakMenuDesc3_2;
        break;
    case 3:
        page = &gStatsAfterPakMenuDesc4_2;
        break;
    }
    return page;
}

void *PakMenu_GetReturnMenu_2(Menu *event) {
    void *page = NULL;
    s32 mode;

    if (gPakMenuIsSave != 0) {
        if (gPakMenuIsProfile != 0) {
            if (gPakMenuAnyPak != 0) {
                mode = gPakMenuPort;
            } else {
                mode = event->controller->port;
            }
            page = modePage(&mode);
        } else {
            page = &gSavePortalMenuDescB_2;
        }
    } else if (gPakMenuIsLoad != 0) {
        if (gPakMenuIsProfile != 0) {
            page = &gCharacterMenuAfterPakDesc_2;
        } else if (gSavePortalActive != 0) {
            page = &gSavePortalMenuDescB_2;
        } else {
            page = &gSingleMenuAfterPakDesc_2;
        }
    } else if (gPakMenuAnyPak != 0) {
        page = &gPakSaveGameAnyPakMenuDesc_2;
    }
    return page;
}
