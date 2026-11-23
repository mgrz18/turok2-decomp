/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/FRONTEND/8040B45C_18.CPP, func_8040B3DC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/menu_text.h"
#include "globals/menu.h"
typedef struct MissionTitleField {
    char pad[0x14];
    char *text;
} MissionTitleField;

s32 MissionMenu_UpdateTitle_2(MissionTitleField *field) {
    if (sMissionFailed != 0) {
        field->text = sMissionMenuText_MissionFailed;
    } else {
        field->text = sMissionMenuText_MissionStatus;
    }
    return 1;
}
