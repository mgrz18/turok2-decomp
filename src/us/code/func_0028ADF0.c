/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028F754_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "protos/ultra.h"

extern void __scMain(void *sc);
void osCreateScheduler(Panel *panel, s32 arg1, s32 arg2, s32 arg3, unsigned char style) {
    panel->active = 1;
    panel->field2F4 = 0;
    panel->field2F8 = 0;
    panel->field2E0 = 0;
    panel->field2FC = 0;
    panel->field2E4 = 0;
    panel->field2E8 = 0;
    panel->field2EC = 0;
    panel->field2F0 = 0;
    panel->field300 = 0;
    panel->mode = 4;
    osCreateMesgQueue(panel->row0, panel->row0Slots, 8);
    osCreateMesgQueue(panel->row1, panel->row1Slots, 8);
    osSetEventMesg(4, panel->row0, 0x29B);
    osSetEventMesg(9, panel->row0, 0x29C);
    osSetEventMesg(0xE, panel->row0, 0x29D);
    osViSetEvent(panel->row0, 0x29A, style);
    osCreateThread(panel->elements, 7, (void *) __scMain, panel, arg1, arg2);
    osStartThread(panel->elements);
}
