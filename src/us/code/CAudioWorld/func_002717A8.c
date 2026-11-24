#include "common.h"
#include "structs/sound.h"

/* The arg1-th 0xC-byte entry of table2B78. */
u8 *CAudioWorld__GetPanEnvelope(CAudioWorld *m, s32 arg1) {
    return m->table2B78 + (arg1 * 0xC);
}
