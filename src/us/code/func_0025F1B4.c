#include "common.h"

s32 EnemyMode_Move_Update();                            /* extern */

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS EnemyMode_Move_Update receives EnemyMode_TakeCover_Update's own argument
 * registers (pass-through); the host passes them. */
void EnemyMode_TakeCover_Update(void * pt0, void * pt1) {
    EnemyMode_Move_Update(pt0, pt1);
}
#else
void EnemyMode_TakeCover_Update(void) {
    EnemyMode_Move_Update();
}
#endif
