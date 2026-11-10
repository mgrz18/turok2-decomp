#include "common.h"
#include "structs/sound.h"

extern void *alMainBusPull(void *, s16 *, s32, s32, void *); /* alMainBusPull */
extern s32 alMainBusParam(s32, s32, s32);
extern void alFilterNew(void *, void *, void *, s32);

/* alMainBusNew */
#ifdef TARGET_PORT
typedef sptr BusSources; /* the source list is a host pointer (s32 truncated it above 4 GB) */
#else
typedef s32 BusSources;
#endif

void alMainBusNew(ALMainBus *arg0, BusSources arg1, s32 arg2) {
    alFilterNew(arg0, alMainBusPull, alMainBusParam, 7);
    arg0->sourceCount = 0;
    arg0->maxSources = arg2;
    arg0->sources = arg1;
}
