/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028F754_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0028A020(void *sc);
extern void func_0029B030(char *row, char *slots, s32 count);
extern void func_0029B950(s32 slot, char *row, s32 text);
extern void func_0029AA20(char *row, s32 text, s32 style);
extern void func_0029B060(char *list, s32 count, void *template, Panel *owner, s32 arg4, s32 arg5);
extern void func_0029BB10(char *list);
void func_0028ADF0(Panel *panel, s32 arg1, s32 arg2, s32 arg3, unsigned char style) {
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
    func_0029B030(panel->row0, panel->row0Slots, 8);
    func_0029B030(panel->row1, panel->row1Slots, 8);
    func_0029B950(4, panel->row0, 0x29B);
    func_0029B950(9, panel->row0, 0x29C);
    func_0029B950(0xE, panel->row0, 0x29D);
    func_0029AA20(panel->row0, 0x29A, style);
    func_0029B060(panel->elements, 7, (void *) func_0028A020, panel, arg1, arg2);
    func_0029BB10(panel->elements);
}
