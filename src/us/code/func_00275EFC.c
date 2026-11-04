#include "common.h"
#include "structs/camera.h"

/* A sound slot: linked at 4, a handle at 8 (released by func_00275864) and a voice at 0xC. */
typedef struct SoundSlot {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ struct SoundSlot *next;
    /* 0x08 */ s32 handle;
    /* 0x0C */ s32 voice;
} SoundSlot;

/* The free list at 0, the used list at 0x14. */
typedef struct {
    /* 0x00 */ CList m_Free;
    /* 0x14 */ CList m_Used;
} SoundSlotPool;

s32 CList__AddTail(void *, void *);              /* extern */
s32 CList__Remove(void *, void *);              /* extern */
s32 func_00275864(s32);                         /* extern */

void func_00275EFC(SoundSlotPool *arg0) {
    SoundSlot *temp_s0;
    SoundSlot *var_s1;

    var_s1 = (SoundSlot *) arg0->m_Used.m_pHead;
    if (var_s1 != NULL) {
        do {
            temp_s0 = var_s1->next;
            func_00275864(var_s1->handle);
            var_s1->voice = -1;
            var_s1->handle = -1;
            CList__Remove(&arg0->m_Used, var_s1);
            CList__AddTail(arg0, var_s1);
            var_s1 = temp_s0;
        } while (var_s1 != NULL);
    }
}
