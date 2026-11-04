#include "common.h"

typedef struct {
    /* 0x00 */ u16 id;
    /* 0x02 */ u16 flags;
} Handle;

extern s32 func_0026C934(void *, u16);

void func_0026C590(Handle *arg0) {
    if ((arg0 != NULL) && (arg0->flags & 2)) {
        func_0026C934(arg0, arg0->id);
    }
}
