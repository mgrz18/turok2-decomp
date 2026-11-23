#include "common.h"
#include "structs/game_object.h"

/* Calls the AI's hurt callback, if any. */
/* ABI (docs/port/ABI.md section 5, abicheck indirect missing_args): every caller passes (actor, ai,
 * damage event) and MIPS hands the hurt callback those three registers (pass-through); the host
 * passes them, with the actor as a full pointer. */
void CAI__CallHurt(void *arg0, CAI *arg1, void *arg2) {
    void (*cb)(void *, CAI *, void *) = (void (*)(void *, CAI *, void *))arg1->m_pfHurt;

    if (cb != NULL) {
        cb(arg0, arg1, arg2);
    }
}
