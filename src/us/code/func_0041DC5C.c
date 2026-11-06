/* WRONG CUT: real function 0x0041DC5C, size 0x134 (through 0x0041DD90): splat cut it in two
 * (PakMenu_GetReturnMenu, func_0041DCB0) at a load in a delay slot. MATCHES over the real extent
 * (checked with the symbol size overridden to 0x134). */
#include "common.h"
#include "structs/menu.h"

extern s32 gPakMenuIsSave;
extern s32 gPakMenuAnyPak;
extern s32 gPakMenuIsLoad;
extern s32 gSavePortalActive;
extern s32 gPakMenuIsProfile;
extern s32 gPakMenuPort;
extern u8 gCharacterMenuAfterPakDesc[];
extern u8 gSingleMenuAfterPakDesc[];
extern u8 gStatsAfterPakMenuDesc1[];
extern u8 gStatsAfterPakMenuDesc2[];
extern u8 gStatsAfterPakMenuDesc3[];
extern u8 gStatsAfterPakMenuDesc4[];
extern u8 gPakSaveGameAnyPakMenuDesc[];
extern u8 gSavePortalMenuDescB[];

static __inline__ void *TmplForKind(s32 *kindp) {
    /* by address: kind stays in its stack slot, as in the ROM */
    void *t;

    switch (*kindp) {
    case 0:
    default:
        t = gStatsAfterPakMenuDesc1;
        break;
    case 1:
        t = gStatsAfterPakMenuDesc2;
        break;
    case 2:
        t = gStatsAfterPakMenuDesc3;
        break;
    case 3:
        t = gStatsAfterPakMenuDesc4;
        break;
    }
    return t;
}

/* Picks the menu template to open for the current game mode. */
void *PakMenu_GetReturnMenu(Menu *c) {
    void *tmpl = NULL;
    s32 kind;

    if (gPakMenuIsSave) {
        if (gPakMenuIsProfile) {
            if (gPakMenuAnyPak) {
                kind = gPakMenuPort;
            } else {
                kind = c->controller->port;
            }
            tmpl = TmplForKind(&kind);
        } else {
            tmpl = gSavePortalMenuDescB;
        }
    } else if (gPakMenuIsLoad) {
        if (gPakMenuIsProfile) {
            tmpl = gCharacterMenuAfterPakDesc;
        } else if (gSavePortalActive) {
            tmpl = gSavePortalMenuDescB;
        } else {
            tmpl = gSingleMenuAfterPakDesc;
        }
    } else if (gPakMenuAnyPak) {
        tmpl = gPakSaveGameAnyPakMenuDesc;
    }
    return tmpl;
}
