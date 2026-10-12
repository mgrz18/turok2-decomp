#include "common.h"
#include "m2c_macros.h"

void __osSpSetStatus(u32 arg0) {
    *(volatile u32 *)0xA4040010 = arg0;
}
