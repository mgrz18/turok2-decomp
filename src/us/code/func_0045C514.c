/* Second-module copy of func_004235F4 (first module; this one at link address 0x00423514), calling this module's own functions at their link addresses. */
#include "common.h"
#include "structs/menu.h"

s32 func_004160F0(void *item);
s32 func_0029E3E0(char *dst, const char *fmt, ...); /* sprintf */

extern char D_800B50B4[];
extern char D_800B50E4[];
extern char D_800C1430[];
extern s32 D_800C2028;
extern s32 D_80130964;
extern s32 D_80130990;

static __inline__ s32 GetPort(Menu *event) {
    if (D_80130990 != 0) {
        return D_800C2028;
    }
    return event->controller->port;
}

s32 func_0045C514(MenuItem *item, Menu *event) {
    s32 port = GetPort(event);

    if (D_80130964 != 0) {
        item->text = D_800B50B4;
    } else {
        item->text = D_800B50E4;
        func_0029E3E0(item->text + (func_004160F0(item) - 2), D_800C1430, port + 1);
    }
    return 0;
}
