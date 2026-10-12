#include "common.h"
#include "m2c_macros.h"

u32 osAiGetStatus(void) {
    return *(volatile u32 *)0xA450000C;
}
