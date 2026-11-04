#include "common.h"

typedef struct {
    /* 0x0 */ u16 id;
    /* 0x2 */ u16 flags;
} Entry;

extern s32 func_0026C8B0(void *, u16);

void func_0026C55C(Entry *arg0) {
    if ((arg0 != NULL) && (arg0->flags & 2)) {
        func_0026C8B0(arg0, arg0->id);
    }
}
