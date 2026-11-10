#include "common.h"
#include "structs/game_object.h"

/* Calls the current AI mode's callback, if any. */
#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md section 5, abicheck indirect missing_args): MIPS calls the mode callback
 * with this function's own a0/a1 (pass-through: the actor and its AI); the host passes them, and
 * the actor stays a full pointer (the callbacks take CGameObjectInstance *). */
void Door_Step(void *arg0, CAI *arg1) {
    s32 (*cb)(void *, CAI *);

    if (arg1->m_pModeCurrent != NULL) {
        cb = (s32 (*)(void *, CAI *))arg1->m_pModeCurrent->m_pfCallback;
        if (cb != NULL) {
            cb(arg0, arg1);
        }
    }
}
#else
void Door_Step(s32 arg0, CAI *arg1) {
    s32 (*temp_v0_2)();
    CAIModeInfo *temp_v0;

    temp_v0 = arg1->m_pModeCurrent;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pfCallback;
        if (temp_v0_2 != NULL) {
            temp_v0_2();
        }
    }
}
#endif
