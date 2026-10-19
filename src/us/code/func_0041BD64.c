/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80444CF8_de_3.c, func_8044421C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int func_00285628(struct ControllerProfile *object);
extern int func_00285670(struct ControllerProfile *object);
s32 func_002855E8(char *);
extern char D_800F5C78;
s32 func_0041BD64(void) {
    extern u32 D_8011AAE0[];
 if(((s32 (*)(char *))func_00285628)(&D_800F5C78)) {
 switch(D_8011AAE0[0]) {
 case 8: D_8011AAE0[0]=32; break;
 case 16: D_8011AAE0[0]=8; break;
 case 32: D_8011AAE0[0]=16; break;
 }
 }
 if(((s32 (*)(char *))func_00285670)(&D_800F5C78) || func_002855E8(&D_800F5C78)) {
 switch(D_8011AAE0[0]) {
 case 8: D_8011AAE0[0]=16; break;
 case 16: D_8011AAE0[0]=32; break;
 case 32: D_8011AAE0[0]=8; break;
 }
 }
 return 0;
}
