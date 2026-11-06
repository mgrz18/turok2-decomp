/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802636B0_de_7.c, func_80263C24_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int Controller_IsPakWatched(int arg0);
extern int gPakChanged[];
extern void Controller_UpdatePakInsertion();
extern int Controller_PakInserted(int arg0);
extern s32 gPakInserted[];
extern s32 gPakRemoved[];
void Controller_UpdatePakInsertion(void)
{
    s32 i;
    for (i = 0; i < 4; i++) {
        if (Controller_IsPakWatched(i) != 0) {
            if (Controller_PakInserted(i) != 0) {
                gPakInserted[i] = 1;
                if (gPakRemoved[i] != 0) {
                    gPakChanged[i] = 1;
                    gPakRemoved[i] = 0;
                }
            } else if (gPakInserted[i] != 0) {
                gPakRemoved[i] = 1;
            }
        } else {
            gPakChanged[i] = 0;
            gPakRemoved[i] = 0;
            gPakInserted[i] = 0;
        }
    }
}
