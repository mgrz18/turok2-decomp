#include "common.h"

void alUnlink(s32);                     /* extern */
void alLink(s32, s32);                /* extern */

void _freePVoice(s32 arg0, s32 arg1) {
    alUnlink(arg1);
    alLink(arg1, arg0 + 0x14);
}
