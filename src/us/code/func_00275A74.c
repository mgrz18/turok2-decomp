#include "common.h"

void CAudioWorld__UnduckMusic(void *);                   /* extern */
extern u8 D_800F2A00[];

void Music_RestoreVolume(void) {
    CAudioWorld__UnduckMusic(D_800F2A00);
}
