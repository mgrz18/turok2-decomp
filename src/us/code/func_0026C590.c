#include "common.h"

typedef struct {
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 flags;
} Handle;

extern s32 CGameRegion__FloodClearFlag4Same(void *, u16);

void CGameRegion__FloodClearFlag4(Handle *arg0) {
    if ((arg0 != NULL) && (arg0->flags & 2)) {
        CGameRegion__FloodClearFlag4Same(arg0, arg0->id);
    }
}
