/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80446C5C_de_8.c, func_80446EE4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"

extern void __osPfsGetOneChannelData(int channel, EntryRecAt3 *data);
void __osPfsGetOneChannelData(s32 channel, EntryRecAt3 *data) {
    u8 *ptr;
    __OSContRequesFormatShort requestformat;
    s32 i;
    ptr = (u8 *)&__osPfsPifRam;
    for (i = 0; i < channel; i++) {
        ptr++;
    }
    requestformat = *(__OSContRequesFormatShort *)ptr;
    data->slot = (requestformat.rxsize & 0xC0) >> 4;
    if (data->slot == 0) {
        data->id = (requestformat.typel << 8) | requestformat.typeh;
        data->team = requestformat.status;
    }
}
