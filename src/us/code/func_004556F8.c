/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030.CPP, func_80444C68_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/engine.h"

struct RecordRecAt14;
struct RecordRecAt14 {
    char pad[8];
    s32 flags;
    char padC[0x14 - 0xC];
    s32 value;
};
extern f32 sResolutionToggleDelay;
extern s32 GetMemSize(void);
extern s32 Video_GetResolutionText_2();
s32 OptionsMenu_ResolutionText_2(struct RecordRecAt14 *record) {
    sResolutionToggleDelay -= gFrameIncrement;
    if (sResolutionToggleDelay <= 0.0f) {
        sResolutionToggleDelay = 0.0f;
    }
    if (GetMemSize() != 0x400000) {
        record->flags |= 0x1000000;
    } else {
        record->flags &= ~0x1000000;
    }
    record->value = Video_GetResolutionText_2();
    return 0;
}
