/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80256280_de_14.c, func_802571F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/audio.h"

extern u32 __amDMA(s32 offset, s32 length);
extern int gAudioRomBase;
extern Window *gAudioDmaState_firstFree;
extern char gLoader;
extern void CLoader__LoadAsync(void *, u32, s32, s32, void *, s32, s32);
u32 __amDMA(s32 offset, s32 length) {
    extern u32 osVirtualToPhysical(s32);
    extern void alLink(Window *, Window *);
    extern void alUnlink(Window *);
    extern Window *gAudioDmaState_firstUsed;
    extern char gAudioDmaMessageQ;
    Window *window;
    Window *after;
    Window **pool;
    u32 address;
    s32 end;
    s32 top;
    s32 odd;
    s32 word;
    after = 0;
    address = offset + gAudioRomBase;
    end = address + length;
    for (window = gAudioDmaState_firstUsed; window != 0; window = window->next) {
        top = window->base + 0x300;
        word = window->base;
        if (address < word) {
            break;
        }
        after = window;
        if (top >= end) {
            window->frame = audFrameCt;
            return osVirtualToPhysical(window->buffer + address - window->base);
        }
    }
    pool = &gAudioDmaState_firstFree;
    window = *pool;
    *pool = window->next;
    alUnlink(window);
    if (after != 0) {
        alLink(window, after);
    } else if (pool[-1] != 0) {
        after = pool[-1];
        pool[-1] = window;
        window->next = after;
        window->prev = 0;
        after->prev = window;
    } else {
        pool[-1] = window;
        window->next = 0;
        window->prev = 0;
    }
    odd = address & 1;
    address -= odd;
    window->base = address;
    window->frame = audFrameCt;
    word = window->buffer;
    nextDMA++;
    CLoader__LoadAsync(&gLoader, address, 0x300, word, &gAudioDmaMessageQ, 0, 1);
    return osVirtualToPhysical(word) + odd;
}
