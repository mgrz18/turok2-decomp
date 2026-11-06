#include "common.h"

extern char D_800F2A00[];
extern void CAudioWorld__Unlock(void *);

void Sound__Unlock(void) {
    CAudioWorld__Unlock(D_800F2A00);
}
