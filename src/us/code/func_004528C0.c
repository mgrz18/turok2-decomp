/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8044524C_us_rev1_4.c, func_80444E30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void Music_SetTrack(s32 a);
extern int QuitMenu_Yes_2(void);
extern void Music_Stop();
extern void Music_RestoreVolume();
extern char gEngineApp[];
extern void func_00430250(void *);
s32 QuitMenu_Yes_2(void) {
    Music_SetTrack(-1);
    Music_RestoreVolume();
    Music_Stop();
    func_00430250(gEngineApp);
    return 1;
}
