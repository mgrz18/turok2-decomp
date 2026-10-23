/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO/80259280_5.CPP, func_802596B4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
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
extern void func_00273A34(Header *header, s32 owner, s32 arg2);
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
    func_00273A34(&voice->header, owner, -1);
}
void func_002763F4(ChannelSet *set, s32 owner) {
    s32 i;
    set->owner = owner;
    func_802596D4_linkInit(&set->free);
    func_00273A34(&set->freeHeader, owner, -1);
    func_802596D4_linkInit(&set->active);
    func_00273A34(&set->activeHeader, owner, -1);
    for (i = 0; i < 32; i++) {
        func_802596D4_voiceInit(&set->voices[i], owner);
        func_802596D4_insertAfter(set->active.prev, &set->voices[i].link);
    }
}
