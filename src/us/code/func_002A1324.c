#include "common.h"

s32 func_002A31E4();                                /* extern */
s32 func_002A31F0(s32);                         /* extern */

void HostIo_RestoreInt(s32 arg0) {
    func_002A31F0(func_002A31E4() | arg0);
}
