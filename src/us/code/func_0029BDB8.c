/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BB6C0_de_18.c, func_802BB9F8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"

extern void __osTimerServicesInit();
void __osTimerServicesInit(void) {
    NodeRecAt1C *node = (NodeRecAt1C *)__osTimerList;
    __osCurrentTime = 0;
    __osBaseCounter = 0;
    __osViIntrCount = 0;
    node->field10 = 0;
    node->field8 = 0;
    node->prev = node;
    node->next = node;
    node->field18 = 0;
    node->field1C = 0;
}
