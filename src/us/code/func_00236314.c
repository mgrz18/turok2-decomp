#include "common.h"
#include "structs/boss.h"

void ScriptAnimator_SetScript(ScriptAnimator *arg0, f32 *script) {
    arg0->mode = 0;
    arg0->loop = script;
    arg0->script = script;
}
