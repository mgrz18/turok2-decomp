#include "common.h"
#include "structs/boss.h"

void ScriptAnimator_Start(ScriptAnimator *arg0, void *owner) {
    arg0->owner = owner;
    arg0->mode = 0;
    arg0->loop = 0;
    arg0->script = 0;
    arg0->alpha = 0;
    arg0->rgb[0] = 0;
    arg0->rgb[1] = 0;
    arg0->rgb[2] = 0;
    arg0->value = 255.0f;
}
