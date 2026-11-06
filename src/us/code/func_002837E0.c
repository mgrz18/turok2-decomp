#include "common.h"
#include "structs/cinema.h"

extern Cinema *D_800C1BB0;

void Cinema__Initialize(void) {
    Cinema *p = D_800C1BB0;

    p->owner = 0;
    p->resource = 0;
    p->unk8 = 0;
    p->onClose = 0;
    p->onEnd = 0;
    p->unk14 = 0;
    p->target = 0;
    *(s32 *)&p->time = 0;
    p->prevTime = 0;
    p->time24 = 0;
    p->clipStart = 0;
    p->clipEnd = 0;
    p->startTime = 0;
    p->endTime = 0;
    p->active = 0;
    p->finished = 0;
    p->started = 0;
    p->ended = 0;
    p->closed = 0;
    p->restored = 0;
    p->unk50 = 0;
    p->resetPlayer = 0;
    p->resetPlayerNext = 0;
    p->restoreAtEnd = 0;
    p->fading = 0;
    p->fade = 0;
    p->stopAtEnd = 0;
    p->saveStart = 0;
    p->startLevel = -1;
    p->startWarp = -1;
    p->startPos.x = 0;
    p->startPos.y = 0;
    p->startPos.z = 0;
    p->startRotY = 0;
    p->startRegion = 0;
    p->warpId = -1;
    p->entryIndex = -1;
    p->scriptId = -1;
    p->nextOnClose = 0;
    p->nextOnEnd = 0;
    p->unkEC = 0;
    p->position.x = 0;
    p->position.y = 0;
    p->position.z = 0;
    p->yaw = 0;
}
