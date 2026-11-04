#include "common.h"
#include "structs/menu.h"

void func_00285A68(s32 port);

extern s32 D_800C2020;
extern s32 D_800C2028;
extern s32 D_80130980;
extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_80130998;
extern s32 D_801309AC;
extern s32 D_801309B4;

/* Takes the port by address: the ROM keeps it in a stack slot (as func_0041DC5C does with its kind). */
static __inline__ void SelectPort(s32 *port) {
    D_800C2020 = 0;
    func_00285A68(*port);
}

void func_00422FC0(Menu *event) {
    s32 port;

    D_801309AC = 0;
    D_80130998 = 1;
    D_80130984 = 1;
    D_801309B4 = 0;
    D_80130980 = 0;
    if (D_80130990 != 0) {
        port = D_800C2028;
    } else {
        port = event->controller->port;
    }
    SelectPort(&port);
}
