#include "common.h"
#include "protos/enemy_ai.h"


/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS EnemyMode_Move_Update receives EnemyMode_TakeCover_Update's own argument
 * registers (pass-through); the host passes them. */
void EnemyMode_TakeCover_Update(void * pt0, void * pt1) {
    EnemyMode_Move_Update(pt0, pt1);
}
