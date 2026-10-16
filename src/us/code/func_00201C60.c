#include "common.h"

/* CDecompressor__Main, after Turok 3's (Drahsid/turok3, src/us/decompressor.c),
 * which also calls func_241BD8() each round; Turok 2 does not. The offsets
 * differ from Turok 3's too, so the structs here keep only what this reads. */

typedef struct CCacheEntry {
    /* 0x00 */ u8 _pad0[0x14];
    /* 0x14 */ void (*decompressCallback)(s32 notifyId, struct CCacheEntry *entry);
    /* 0x18 */ s32 notifyId;
    /* 0x1C */ u8 _pad1[4];
    /* 0x20 */ void *decompressReplyQueue;      /* OSMesgQueue * */
} CCacheEntry;

typedef struct {
    /* 0x0000 */ u8 _pad0[0x230];
    /* 0x0230 */ u8 queue[0x1218];              /* OSMesgQueue, then the thread */
    /* 0x1448 */ s32 lastDecompression;
} CDecompressor;

extern s32 D_800B6D18;                          /* gFrameCount */
s32 osRecvMesg(void *queue, void *msg, s32 flags);
s32 osSendMesg(void *queue, void *msg, s32 flags);

/* The decompressor thread: takes a cache entry from its queue, runs the
 * entry's callback, and sends the entry back to whoever asked. */
void CDecompressor__Main(CDecompressor *thisx) {
    CCacheEntry *entry;

    while (1) {
        osRecvMesg(&thisx->queue, &entry, 1);
        thisx->lastDecompression = D_800B6D18;
        entry->decompressCallback(entry->notifyId, entry);
        osSendMesg(entry->decompressReplyQueue, entry, 1);
    }
}
