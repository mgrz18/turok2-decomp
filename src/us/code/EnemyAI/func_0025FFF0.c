#include "common.h"
#include "protos/ai.h"
#include "globals/controller.h"


/* Three override slots, 6 bytes apart (see EnemyAnim_JoypadTestPlayAnim). */

void EnemyMode_JoypadTestIdle_Update(sptr arg0, sptr arg1) {
    if ((gContPadData[1].button != 0) || (gContPadData[2].button != 0) || (gContPadData[3].button != 0)) {
        CAI__SetMode(arg0, arg1, 0x32);
    }
}
