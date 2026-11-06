#include "common.h"

typedef struct {
    /* 0x0 */ u16 id;
    /* 0x2 */ u16 flags;
} Entry;

extern s32 CGameRegion__FloodSetFlag4Same(void *, u16);

void CGameRegion__FloodSetFlag4(Entry *arg0) {
    if ((arg0 != NULL) && (arg0->flags & 2)) {
        CGameRegion__FloodSetFlag4Same(arg0, arg0->id);
    }
}
