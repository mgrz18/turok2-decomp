/* WRONG CUT: real function 0x0041DC5C, size 0x134 (through 0x0041DD90): splat cut it in two
 * (PakMenu_GetReturnMenu_2, func_0041DCB0) at a load in a delay slot. MATCHES over the real extent
 * (checked with the symbol size overridden to 0x134). */
#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"

static __inline__ void *TmplForKind(s32 *kindp) {
    /* by address: kind stays in its stack slot, as in the ROM */
    void *t;

    switch (*kindp) {
    case 0:
    default:
        t = &gStatsAfterPakMenuDesc1_2;
        break;
    case 1:
        t = &gStatsAfterPakMenuDesc2_2;
        break;
    case 2:
        t = &gStatsAfterPakMenuDesc3_2;
        break;
    case 3:
        t = &gStatsAfterPakMenuDesc4_2;
        break;
    }
    return t;
}

/* Picks the menu template to open for the current game mode. */
void *PakMenu_GetReturnMenu_2(Menu *c) {
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
            tmpl = &gSavePortalMenuDescB_2;
        }
    } else if (gPakMenuIsLoad) {
        if (gPakMenuIsProfile) {
            tmpl = &gCharacterMenuAfterPakDesc_2;
        } else if (gSavePortalActive) {
            tmpl = &gSavePortalMenuDescB_2;
        } else {
            tmpl = &gSingleMenuAfterPakDesc_2;
        }
    } else if (gPakMenuAnyPak) {
        tmpl = &gPakSaveGameAnyPakMenuDesc_2;
    }
    return tmpl;
}
