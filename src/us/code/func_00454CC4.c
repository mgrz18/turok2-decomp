#include "structs/vector.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_8044421C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/controller.h"
#include "structs/list.h"

extern int Controller_MenuLeft(ControllerProfile *object);
extern int Controller_MenuRight(ControllerProfile *object);
s32 Controller_ConfirmPressed(char *);
extern u32 gEngineApp_m_Settings_difficulty[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
s32 OptionsMenu_CycleDifficulty_2(void) {
 if(((s32 (*)(char *))Controller_MenuLeft)((char *)&gMergedController)) {
 switch(gEngineApp_m_Settings_difficulty[0]) {
 case 8: gEngineApp_m_Settings_difficulty[0]=32; break;
 case 16: gEngineApp_m_Settings_difficulty[0]=8; break;
 case 32: gEngineApp_m_Settings_difficulty[0]=16; break;
 }
 }
 if(((s32 (*)(char *))Controller_MenuRight)((char *)&gMergedController) || Controller_ConfirmPressed((char *)&gMergedController)) {
 switch(gEngineApp_m_Settings_difficulty[0]) {
 case 8: gEngineApp_m_Settings_difficulty[0]=16; break;
 case 16: gEngineApp_m_Settings_difficulty[0]=32; break;
 case 32: gEngineApp_m_Settings_difficulty[0]=8; break;
 }
 }
 return 0;
}
