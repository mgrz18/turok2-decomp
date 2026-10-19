/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043F52C_de_6.c, func_804423BC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int func_00285628(struct ControllerProfile *object);
extern int func_00285670(struct ControllerProfile *object);
s32 func_002855E8(s32);
s32 func_0041648C(func_8022A404_S1 *menu,s32 value,s32 step,s32 minimum,s32 maximum,s32 wrap) {
 if(((s32 (*)(s32))func_00285628)(menu->unk20))value-=step;
 if(((s32 (*)(s32))func_00285670)(menu->unk20)||func_002855E8(menu->unk20))value+=step;
 if(value<minimum) {
  value=minimum;if(wrap)value=maximum;
 } else if(value>maximum) {
  value=maximum;if(wrap)value=minimum;
 }
 return value;
}
