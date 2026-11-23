#include "common.h"
#include "structs/game_object.h"

/* ABI (docs/port/ABI.md section 5, abicheck indirect missing_args): MIPS calls the mode callback
 * with this function's own a0/a1 (pass-through: the actor and its AI); the host passes them, and
 * the actor stays a full pointer (the callbacks take CGameObjectInstance *). */
void Turret_Step(void *arg0, CAI *arg1) {
    s32 (*cb)(void *, CAI *);

    if (arg1->m_pModeCurrent != NULL) {
        cb = (s32 (*)(void *, CAI *))arg1->m_pModeCurrent->m_pfCallback;
        if (cb != NULL) {
            cb(arg0, arg1);
        }
    }
}
