#include "common.h"
#include "protos/ai.h"


/* Three override slots, 6 bytes apart (see EnemyAnim_JoypadTestPlayAnim). */
extern u16 D_800F5C66[];

void EnemyMode_JoypadTestIdle_Update(sptr arg0, sptr arg1) {
    if ((D_800F5C66[0] != 0) || (D_800F5C66[3] != 0) || (D_800F5C66[6] != 0)) {
        CAI__SetMode(arg0, arg1, 0x32);
    }
}
