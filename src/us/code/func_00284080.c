/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80243A90_de_8.c, func_802456A0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void Cinema_RunEnd(s32 unused);
extern void Cinema_Stop();
extern int Cinema_IsActive(void);
extern void Cinema_Close(void);
extern void CCache__Deallocate(void *arg0, void *arg1);
void Cinema_Stop(void) {
    extern void *gCinemaPlayerP;
    if (Cinema_IsActive() != 0) {
        s32 temp_a1;
        Shared_MenuContext *record;
        Cinema_RunEnd(1);
        Cinema_Close();
        temp_a1 = (s32)((Shared_MenuContext *)gCinemaPlayerP)->owner;
        if (temp_a1 != 0) {
            CCache__Deallocate(0, temp_a1);
        }
        record = gCinemaPlayerP;
        record->owner = 0;
        record->resource = 0;
        record->active = 0;
        record->state3C = 0;
        record->unknown60 = 0;
    }
}
int Cinema_IsActive(void);
