/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AFE30_de_20.c, func_802B00D4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
extern void func_00291270(unsigned char *source, unsigned char *destination, int count);
struct ALEventQueue;
typedef struct ALEventQueue ALEventQueue;
struct ALNoteEvent;
struct ALOscEvent;
struct ALVolumeEvent;
struct ALEndEvent;
struct ALTempoEvent;
struct ALEvent;
typedef struct ALNoteEvent ALNoteEvent;
typedef struct ALOscEvent ALOscEvent;
typedef struct ALVolumeEvent ALVolumeEvent;
typedef struct ALEndEvent ALEndEvent;
typedef struct ALTempoEvent ALTempoEvent;
typedef struct ALEvent ALEvent;
struct ListLink;
struct ListLink {
    struct ListLink *next;
    struct ListLink *prev;
};
struct ListLink;
typedef struct ListLink ListLink;
struct ALEventQueue;
struct ALEventQueue {
    ListLink freeList;
    ListLink allocList;
    s32 eventCount;
};
struct ALEvent;
struct ALVoice_s;
typedef struct ALVoice_s ALVoice_s;
struct ALVoice_s;
struct ALVoice_s {
    ListLink node;
    void *pvoice;
    void *table;
    void *clientPrivate;
    s16 state;
    s16 priority;
    s16 fxBus;
    s16 unityPitch;
};
struct ALMIDIEvent;
typedef struct ALMIDIEvent ALMIDIEvent;
struct InventorySlot;
struct InventorySlot {
    u8 available;
    u8 slot;
};
struct InventorySlot;
typedef struct InventorySlot InventorySlot;
struct ALMIDIEvent;
struct ALMIDIEvent {
    s32 ticks;
    u8 status;
    u8 byte1;
    u8 byte2;
    u32 duration;
};
struct ALEventListItem;
typedef struct ALEventListItem ALEventListItem;
struct Request;
struct Request {
    s16 type;
};
struct func_80284AF4_G2;
typedef struct func_80284AF4_G2 func_80284AF4_G2;
struct Request;
typedef struct Request Request;
struct func_80284AF4_G2;
struct func_80284AF4_G2 {
    void * unk0;
};
struct ALEndEvent {
    s32 ticks;
    u8 status;
    u8 type;
    u8 len;
};
struct ALTempoEvent {
    s32 ticks;
    u8 status;
    u8 type;
    u8 len;
    u8 byte1;
    u8 byte2;
    u8 byte3;
};
struct ALNoteEvent {
    struct ALVoice_s *voice;
};
struct ALVolumeEvent {
    struct ALVoice_s *voice;
    s32 delta;
    u8 vol;
};
struct ALOscEvent {
    struct ALVoiceState *vs;
    void *oscState;
    u8 chan;
};
struct ALEvent {
    s16 type;
    union {
        ALMIDIEvent midi;
        ALTempoEvent tempo;
        ALEndEvent end;
        ALNoteEvent note;
        ALVolumeEvent vol;
        Request spvol;
        InventorySlot sppriority;
        func_80284AF4_G2 spseq;
        func_80284AF4_G2 spbank;
        ALOscEvent osc;
    } msg;
};
struct ALEventListItem {
    ListLink node;
    s32 delta;
    ALEvent evt;
};
struct ALEvent;
extern s32 func_0029E230(s32);
void func_00291034(ALEventQueue *evtq, ALEvent *evt, s32 delta)
{
    extern void func_002933E0(ListLink *, ListLink *);
    extern void func_002933B0(ListLink *);
    ALEventListItem *item;
    ALEventListItem *nextItem;
    ListLink *node;
    s32 postAtEnd = 0;
    s32 mask;
    mask = func_0029E230(1);
    item = (ALEventListItem *)evtq->freeList.next;
    if (!item) {
        func_0029E230(mask);
        return;
    }
    func_002933B0((ListLink *)item);
    ((void (*)(void *, void *, s32))func_00291270)(evt, &item->evt, sizeof(*evt));
    if (delta == 0x7FFFFFFF)
        postAtEnd = -1;
    for (node = &evtq->allocList; node != 0; node = node->next) {
        if (!node->next) {
            if (postAtEnd)
                item->delta = 0;
            else
                item->delta = delta;
            func_002933E0((ListLink *)item, node);
            break;
        } else {
            nextItem = (ALEventListItem *)node->next;
            if (delta < nextItem->delta) {
                item->delta = delta;
                nextItem->delta -= delta;
                func_002933E0((ListLink *)item, node);
                break;
            }
            delta -= nextItem->delta;
        }
    }
    func_0029E230(mask);
}
