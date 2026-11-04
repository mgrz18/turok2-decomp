/* WRONG CUT: real function 0x0041DC5C, size 0x134 (through 0x0041DD90): splat cut it in two
 * (func_0041DC5C, func_0041DCB0) at a load in a delay slot. MATCHES over the real extent
 * (checked with the symbol size overridden to 0x134). */
#include "common.h"
#include "structs/menu.h"

extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_80130994;
extern s32 D_80130998;
extern s32 D_801309B4;
extern s32 D_800C2028;
extern u8 D_0043882C[];
extern u8 D_00439190[];
extern u8 D_0043AC80[];
extern u8 D_0043ACA4[];
extern u8 D_0043ACC8[];
extern u8 D_0043ACEC[];
extern u8 D_0043B738[];
extern u8 D_0043BEF4[];

static __inline__ void *TmplForKind(s32 *kindp) {
    /* by address: kind stays in its stack slot, as in the ROM */
    void *t;

    switch (*kindp) {
    case 0:
    default:
        t = D_0043AC80;
        break;
    case 1:
        t = D_0043ACA4;
        break;
    case 2:
        t = D_0043ACC8;
        break;
    case 3:
        t = D_0043ACEC;
        break;
    }
    return t;
}

/* Picks the menu template to open for the current game mode. */
void *func_0041DC5C(Menu *c) {
    void *tmpl = NULL;
    s32 kind;

    if (D_80130984) {
        if (D_801309B4) {
            if (D_80130990) {
                kind = D_800C2028;
            } else {
                kind = c->controller->port;
            }
            tmpl = TmplForKind(&kind);
        } else {
            tmpl = D_0043BEF4;
        }
    } else if (D_80130994) {
        if (D_801309B4) {
            tmpl = D_0043882C;
        } else if (D_80130998) {
            tmpl = D_0043BEF4;
        } else {
            tmpl = D_00439190;
        }
    } else if (D_80130990) {
        tmpl = D_0043B738;
    }
    return tmpl;
}
