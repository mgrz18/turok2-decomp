/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80243A90_de_10.c, func_80245BC0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_002844F0();
extern Shared_MenuContext *D_800C1BB0;
extern void func_002052D8(void *arg0, void *arg1);
void func_002844F0(void) {
    if (D_800C1BB0->closed == 0) {
        {
            void *temp_a1 = D_800C1BB0->owner;
            if (temp_a1 != 0) {
                func_002052D8(0, temp_a1);
            }
        }
        {
            void *record = D_800C1BB0;
            FuncPtr fn = ((Shared_MenuContext *)(record))->onClose;
            ((Shared_MenuContext *)(record))->owner = 0;
            ((Shared_MenuContext *)(record))->resource = 0;
            ((Shared_MenuContext *)(record))->active = 0;
            ((Shared_MenuContext *)(record))->state3C = 0;
            ((Shared_MenuContext *)(record))->unknown60 = 0;
            if (fn != 0) {
                ((Shared_MenuContext *)(record))->onClose = 0;
                fn();
            }
        }
        D_800C1BB0->closed = 1;
    }
}
