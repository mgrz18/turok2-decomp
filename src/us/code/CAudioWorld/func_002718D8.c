#include "common.h"
#include "structs/sound.h"

void CAudioWorld__SetListener(CAudioWorld *arg0, struct CCamera *arg1) {
    arg0->listener = arg1;
}
