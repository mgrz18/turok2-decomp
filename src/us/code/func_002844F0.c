/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80243A90_de_10.c, func_80245BC0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/cinema.h"

extern void Cinema_Close();
extern void CCache__Deallocate(void *arg0, void *arg1);
void Cinema_Close(void) {
    if (gCinemaPlayerP->closed == 0) {
        {
            void *temp_a1 = gCinemaPlayerP->owner;
            if (temp_a1 != 0) {
                CCache__Deallocate(0, temp_a1);
            }
        }
        {
            void *record = gCinemaPlayerP;
            CinemaCallback fn = ((Cinema *)(record))->onClose;
            ((Cinema *)(record))->owner = 0;
            ((Cinema *)(record))->resource = 0;
            ((Cinema *)(record))->active = 0;
            ((Cinema *)(record))->finished = 0;
            ((Cinema *)(record))->fading = 0;
            if (fn != 0) {
                ((Cinema *)(record))->onClose = 0;
                fn();
            }
        }
        gCinemaPlayerP->closed = 1;
    }
}
