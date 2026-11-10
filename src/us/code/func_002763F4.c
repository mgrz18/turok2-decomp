/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO/80259280_5.CPP, func_802596B4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#ifndef TARGET_PORT
typedef unsigned char u8;
typedef signed int s32;
#ifndef COMMON_H
#ifdef TARGET_PORT
typedef __INTPTR_TYPE__ sptr;
typedef __UINTPTR_TYPE__ uptr;
#else
typedef signed int sptr;
typedef unsigned int uptr;
#endif
#endif
struct ListLink;
struct ListLink {
    struct ListLink *next;
    struct ListLink *prev;
};
struct ListLink;
typedef struct ListLink ListLink;
struct Header;
struct Header {
    u8 data[0xCC];
};
struct Header;
typedef struct Header Header;
struct VoiceRecAt8;
typedef struct VoiceRecAt8 VoiceRecAt8;
struct VoiceRecAt8;
struct VoiceRecAt8 {
    ListLink link;
    Header header;
};
struct ChannelSet;
struct ChannelSet {
    s32 owner;
    ListLink free;
    Header freeHeader;
    ListLink active;
    Header activeHeader;
    VoiceRecAt8 voices[32];
};
struct ChannelSet;
typedef struct ChannelSet ChannelSet;
extern void SndVoice_Construct(Header *header, sptr owner, s32 arg2);
static __inline__ void func_802596D4_linkInit(ListLink *link) {
    link->prev = link;
    link->next = link;
}
static __inline__ void func_802596D4_insertAfter(ListLink *at, ListLink *link) {
    link->next = at->next;
    link->prev = at;
    at->next->prev = link;
    at->next = link;
}
static __inline__ void func_802596D4_voiceInit(VoiceRecAt8 *voice, s32 owner) {
    func_802596D4_linkInit(&voice->link);
    SndVoice_Construct(&voice->header, owner, -1);
}
void SndQueue_Construct(ChannelSet *set, s32 owner) {
    s32 i;
    set->owner = owner;
    func_802596D4_linkInit(&set->free);
    SndVoice_Construct(&set->freeHeader, owner, -1);
    func_802596D4_linkInit(&set->active);
    SndVoice_Construct(&set->activeHeader, owner, -1);
    for (i = 0; i < 32; i++) {
        func_802596D4_voiceInit(&set->voices[i], owner);
        func_802596D4_insertAfter(set->active.prev, &set->voices[i].link);
    }
}
#else
#include "common.h"
#include "structs/sound.h"
/* The port: the same on the typed queue (the view above: s32 owner, 0xCC-byte voices, links
 * named next/prev the other way round). Field for field: link word 0 = prev, word 1 = next. */
extern void SndVoice_Construct(SndVoice *v, CAudioWorld *m, s32 arg2);
void SndQueue_Construct(SndQueue *q, CAudioWorld *m) {
    s32 i;
    q->owner = m;
    q->active.link.prev = q->active.link.next = &q->active.link;
    SndVoice_Construct(&q->active.body, m, -1);
    q->free.link.prev = q->free.link.next = &q->free.link;
    SndVoice_Construct(&q->free.body, m, -1);
    for (i = 0; i < 32; i++) {
        SndLink *at = q->free.link.next;
        SndLink *l = &q->nodes[i].link;
        l->prev = l->next = l;
        SndVoice_Construct(&q->nodes[i].body, m, -1);
        l->prev = at->prev;
        l->next = at;
        at->prev->next = l;
        at->prev = l;
    }
}
#endif
