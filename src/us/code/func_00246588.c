#include "common.h"

/* Two words set to the same value (a current and an initial value). */
typedef struct {
    /* 0x00 */ s32 value;
    /* 0x04 */ s32 initial;
} ValuePair;

void ValuePair_Set(ValuePair *arg0, s32 arg1) {
    arg0->value = arg1;
    arg0->initial = arg1;
}
