#include "common.h"
#include "structs/game_object.h"

/* Calls the AI's hurt callback, if any. */
void func_002465C8(s32 arg0, CAI *arg1) {
    s32 (*temp_v0)();

    temp_v0 = arg1->m_pfHurt;
    if (temp_v0 != NULL) {
        temp_v0();
    }
}
