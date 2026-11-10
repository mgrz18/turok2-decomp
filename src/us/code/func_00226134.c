#include "common.h"
#include "structs/world.h"
#include "protos/iset.h"


#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
void * func_00226134(CScene *arg0, s32 arg1) {
    return CISet__GetBlock(CISet__GetBlock(arg0->unk_68, arg1), 0);
}
#else
void func_00226134(CScene *arg0, s32 arg1) {
    CISet__GetBlock(CISet__GetBlock(arg0->unk_68, arg1), 0);
}
#endif
