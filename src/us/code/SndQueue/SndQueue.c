/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_6.c, func_80259260_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "common.h"
#include "structs/sound.h"
#include "m2c_macros.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_7.c, func_80259784_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_7.c, func_802598B4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_7.c, func_80259918_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_7.c, func_80259988_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_7.c, func_802599EC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_7.c, func_80259A50_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void SndVoice_SetSoundId(void *arg0, int arg1);
extern s16 SoundEffectInfo_GetGroup(void *);
extern s16 SoundEffectInfo_GetSample(void *);
extern s8 SoundEffectInfo_GetPitch(void *);
extern void SndVoice_Setup(void *, void *, s32);
extern s8 SoundEffectInfo_GetPriority(void *);
extern u8 SoundEffectInfo_GetPan(void *);
extern s16 SoundEffectInfo_GetFlags(void *);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndQueue_Add) */
s32 SndQueue_Add(ManagerRecAtD8V1 *manager, void *desc, Vec3Words *position, s32 volume, s32 extra) {
    extern void CAudioWorld__Unlock(s32 *);
    extern void CAudioWorld__Lock(s32 *);
    NodeRecAtC8 *node;
    NodeRecAtC8 *first;
    NodeRecAtC8 *at;
    s32 *scene;
    s32 priority;
    if (manager->free.next != (NodeRecAtC8 *)&manager->free) {
        CAudioWorld__Lock(manager->scene);
        first = manager->free.next;
        first->prev->next = first->next;
        first->next->prev = first->prev;
        scene = manager->scene;
        node = first;
        node->handle = scene[0x108 / 4];
        scene[0x108 / 4] = node->handle + 1;
        node->volume = volume;
        node->voice = -1;
        node->volume16 = volume;
        node->group = SoundEffectInfo_GetGroup(desc);
        node->priority = SoundEffectInfo_GetPriority(desc);
        node->sample = SoundEffectInfo_GetSample(desc);
        node->pan = SoundEffectInfo_GetPan(desc);
        node->flags = SoundEffectInfo_GetFlags(desc);
        node->pitch = SoundEffectInfo_GetPitch(desc);
        node->gainB = 0x7D00;
        node->gainA = 0x7D00;
        node->time = 0;
        node->frame = manager->scene[0x104 / 4];
        node->position = *position;
        node->pad58 = 0;
        node->padB4 = 0;
        SndVoice_SetSoundId(node->channel, ((struct ContextRecAt2BC0 *)manager->scene)->field2B8C);
        node->mix = manager->scene[0x2BAC / 4];
        SndVoice_Setup(node->channel, desc, extra);
        at = manager->active.next;
        priority = node->priority;
        while (at != (NodeRecAtC8 *)&manager->active) {
            if (at->priority < priority) {
                break;
            }
            at = at->next;
        }
        node->prev = at->prev;
        node->next = at;
        at->prev->next = node;
        at->prev = node;
        CAudioWorld__Unlock(manager->scene);
        return node->handle;
    }
    return -1;
}
#endif

#ifndef COMMON_H
#ifdef TARGET_PORT
typedef __INTPTR_TYPE__ sptr;
typedef __UINTPTR_TYPE__ uptr;
#else
typedef signed int sptr;
typedef unsigned int uptr;
#endif
#endif

struct Glyphs;
struct Glyphs {
    char pad0[0x10];
    s32 bitmaps[1];
};
struct Font;
struct Font {
    char pad0[0xC];
    struct Glyphs *glyphs;
};
struct ContextCode802591C0;
struct ContextCode802591C0 {
    char pad0[0x7C];
    struct Font *font;
    char pad80[4];
    char textures[0xDC - 0x84];
    s16 slots[16];
    char padFC[0x104 - 0xFC];
    s32 frame;
    s32 sequence;
    char pad10C[0x1DB8 - 0x10C];
    char cells[1];
};
struct Cache;
typedef struct Cache Cache;
struct CellCode802591C0;
struct CellCode802591C0 {
    s32 index;
    s32 pad4;
    s32 texture;
    s32 sequence;
    s32 frame;
    char pad14[0x44 - 0x14];
    char data[1];
};
struct Entry_func_80259440_de;
struct Entry_func_80259440_de {
    struct Entry_func_80259440_de *prev;
    struct Entry_func_80259440_de *next;
    char body[0x10];
    s32 frame;
    s32 delay;
    char pad20[0x40 - 0x20];
    s16 glyph;
    s16 font;
    s16 style;
    s16 pad46;
    s32 sequence;
    char pad4C[0xA8 - 0x4C];
    s32 fieldA8;
};
typedef struct CellCode802591C0 CellCode802591C0;
struct ListHead;
struct ListHead {
    struct Entry_func_80259440_de *prev;
    struct Entry_func_80259440_de *next;
};
typedef struct ListHead ListHead;
struct Cache {
    struct ContextCode802591C0 *context;
    ListHead pending;
    char padC[0xD8 - 0xC];
    ListHead done;
};
typedef struct Entry_func_80259440_de Entry_func_80259440_de;
typedef struct Glyphs Glyphs;
typedef struct ContextCode802591C0 ContextCode802591C0;
extern s16 SndVoicePool_FindSlot(char *, s32, s32, s32, s32);
typedef struct Font Font;
extern void CAudioWorld__Lock(s32 *arg0);
extern void CAudioWorld__Unlock(s32 *arg0);
extern s16 alSndpAllocate(char *, sptr);
extern void *func_0029E460(void *destination, const void *source, int count);
extern void CAudioWorld__StartVoice(ContextCode802591C0 *, s32, s32, char *);

static __inline__ void append(ListHead *head, Entry_func_80259440_de *entry) {
    entry->prev = head->prev;
    entry->next = (Entry_func_80259440_de *)head;
    head->prev->next = entry;
    head->prev = entry;
}

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndQueue_Update) */
void SndQueue_Update(Cache *cache) {
    s16 *slots;
    char *textures;
    Glyphs *glyphs;
    Entry_func_80259440_de *entry;
    Entry_func_80259440_de *next;
    s32 slot;
    s16 *cursor;
    s32 free;
    CellCode802591C0 *cell;
    CAudioWorld__Lock(cache->context);
    slots = cache->context->slots;
    textures = cache->context->textures;
    glyphs = cache->context->font->glyphs;
    for (entry = cache->pending.next; entry != (Entry_func_80259440_de *)&cache->pending; entry = next) {
        next = entry->next;
        if (entry->delay != 0) {
            if (--entry->delay == 0) {
                entry->frame = cache->context->frame;
                entry->sequence = cache->context->sequence++;
                entry->fieldA8 = 0;
            }
            continue;
        }
        if (cache->context->frame - entry->frame >= 16) {
            entry->prev->next = next;
            entry->next->prev = entry->prev;
            append(&cache->done, entry);
            continue;
        }
        slot = SndVoicePool_FindSlot(cache->context->cells, entry->style, entry->font, entry->glyph, 0);
        if (slot == -1 || slot == 16) {
            break;
        }
#ifdef TARGET_PORT
        /* round 19: the N64 adds the byte offset to the pointer as a u32; on the host that
         * truncated the 64-bit pointer (h1 found it) */
        cursor = (s16 *)((__UINTPTR_TYPE__)(slot << 1) + (__UINTPTR_TYPE__)slots);
#else
        cursor = (s16 *)((slot << 1) + (u32)slots);
#endif
        free = *cursor;
        if (free != -1) {
            break;
        }
        *cursor = alSndpAllocate(textures, glyphs->bitmaps[entry->font]);
        if (*cursor == free) {
            break;
        }
        cell = (CellCode802591C0 *)(cache->context->cells + 4 + slot * 0xCC);
        func_0029E460(cell, entry->body, 0xCC);
        cell->sequence = entry->sequence;
        cell->texture = *cursor;
        cell->frame = cache->context->frame;
        cell->index = slot;
        CAudioWorld__StartVoice(cache->context, cell->sequence, 0, cell->data);
        entry->sequence = free;
        entry->glyph = free;
        entry->prev->next = entry->next;
        entry->next->prev = entry->prev;
        append(&cache->done, entry);
    }
    CAudioWorld__Unlock(cache->context);
}
#endif

void SndVoice_Construct(SndVoice *voice, CAudioWorld *owner, s32 index);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndQueue_InitNode) */
void SndQueue_InitNode(SndNode *node, CAudioWorld *owner) {
    node->link.next = &node->link;
    node->link.prev = &node->link;
    SndVoice_Construct(&node->body, owner, -1);
}
#endif

typedef struct Node { struct Node *next, *prev; } Node;

Node *SndQueue_Unlink(Node *arg0) {
    arg0->next->prev = arg0->prev;
    arg0->prev->next = arg0->next;
    return arg0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO/80259280_5.CPP, func_802596B4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#ifndef TARGET_PORT
#include "rw/abu_types.h"
#include "structs/sound.h"
void SndVoice_Construct(SndVoice *voice, CAudioWorld *owner, s32 index);
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
    SndVoice_Construct((SndVoice *)&voice->header, (CAudioWorld *)owner, -1);
}
void SndQueue_Construct(ChannelSet *set, sptr owner) {
    s32 i;
    set->owner = owner;
    func_802596D4_linkInit(&set->free);
    SndVoice_Construct((SndVoice *)&set->freeHeader, (CAudioWorld *)owner, -1);
    func_802596D4_linkInit(&set->active);
    SndVoice_Construct((SndVoice *)&set->activeHeader, (CAudioWorld *)owner, -1);
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
void SndVoice_Construct(SndVoice *voice, CAudioWorld *owner, s32 index);
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

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: SndQueue_RequeueVoice) */
void SndQueue_RequeueVoice(ManagerRecAtD8V2 *manager, s32 index, s32 owner, CVector3 *position) {
    Entry_func_80259784_de *entry;
    NodeRecAt4C *node;
    NodeRecAt4C *at;
    NodeRecAt4C *first;
    s32 priority;
    entry = &manager->scene->entries[index];
    if (entry->busy != 0) {
        return;
    }
    first = manager->free.next;
    if (first != (NodeRecAt4C *)&manager->free) {
        first->prev->next = first->next;
        first->next->prev = first->prev;
        node = first;
        func_0029E460(node->pad8, entry, 0xCC);
        node->owner = owner;
        node->position.x = position->x;
        node->position.y = position->y;
        node->position.z = position->z;
        node->timer = 0;
        at = manager->active.next;
        priority = node->priority;
        while (at != (NodeRecAt4C *)&manager->active) {
            if (at->priority < priority) {
                break;
            }
            at = at->next;
        }
        node->prev = at->prev;
        node->next = at;
        at->prev->next = node;
        at->prev = node;
    }
    manager->scene->activeSelection = 0;
    manager->scene->pendingSelection = -1;
    manager->scene->previousSelection = -1;
    manager->scene->selection = -1;
}
#endif

#ifndef TARGET_PORT
extern void SndQueue_CancelHandle(struct ChannelSet *arg0, s32 arg1);
void SndQueue_CancelHandle(ChannelSet *arg0, s32 arg1) {
    Node_func_802598B4_de *node;
    Node_func_802598B4_de *next;
    Node_func_802598B4_de *end;
    Node_func_802598B4_de *initial_end;
    Node_func_802598B4_de *tail_end;
    node = (Node_func_802598B4_de *)arg0->free.prev;
    tail_end = (Node_func_802598B4_de *)&arg0->active;
    initial_end = (Node_func_802598B4_de *)&arg0->free;
    if (node != initial_end) {
        end = initial_end;
        do {
            next = node->next;
            if (node->value == arg1) {
                node->prev->next = next;
                node->next->prev = node->prev;
                node->prev = (Node_func_802598B4_de *)arg0->active.next;
                node->next = tail_end;
                ((Node_func_802598B4_de *)arg0->active.next)->next = node;
                arg0->active.next = (ListLink *)node;
            }
            node = next;
        } while (node != end);
    }
}
#else
#include "structs/sound.h"
/* The port: the same on the typed queue (the view above has N64 offsets; the node's value (+0xC)
 * is body.sound). Matching active requests move to the free list's tail. */
void SndQueue_CancelHandle(SndQueue *q, s32 arg1) {
    SndLink *node = q->active.link.next;
    while (node != &q->active.link) {
        SndLink *next = node->next;
        if (((SndNode *)node)->body.sound == arg1) {
            node->prev->next = next;
            node->next->prev = node->prev;
            node->next = &q->free.link;
            node->prev = q->free.link.prev;
            q->free.link.prev->next = node;
            q->free.link.prev = node;
        }
        node = next;
    }
}
#endif
#ifndef TARGET_PORT
void SndQueue_CancelAllExcept(OwnerRecAtD8 *arg0, s32 arg1) {
    Node_func_80259918_de *node;
    Node_func_80259918_de *next;
    node = arg0->activeNext;
    while (node != (Node_func_80259918_de *) &arg0->activePrev) {
        next = node->next;
        if (arg1 == -1 || node->key != arg1) {
            node->prev->next = next;
            node->next->prev = node->prev;
            do { Node_func_80259918_de *tail_ = (arg0)->otherPrev; (node)->next = (Node_func_80259918_de *) &(arg0)->otherPrev; (node)->prev = tail_; (arg0)->otherPrev->next = (node); (arg0)->otherPrev = (node); } while (0);
        }
        node = next;
    }
}
#else
#include "structs/sound.h"
/* The port: the same on the typed queue (node key = body.soundId; moved to the free list tail). */
void SndQueue_CancelAllExcept(SndQueue *q, s32 arg1) {
    SndLink *node = q->active.link.next;
    while (node != &q->active.link) {
        SndLink *next = node->next;
        if (arg1 == -1 || ((SndNode *)node)->body.soundId != arg1) {
            node->prev->next = next;
            node->next->prev = node->prev;
            node->next = &q->free.link;
            node->prev = q->free.link.prev;
            q->free.link.prev->next = node;
            q->free.link.prev = node;
        }
        node = next;
    }
}
#endif
#ifndef TARGET_PORT
void SndQueue_CancelSoundId(void *arg0, s32 arg1)
{
    OwnerRecAtD8 *owner = arg0;
    Node_func_80259918_de *node = owner->activeNext;
    Node_func_80259918_de *head = (Node_func_80259918_de *)&owner->activePrev;
    Node_func_80259918_de *other = (Node_func_80259918_de *)&owner->otherPrev;
    if (node != head) {
        Node_func_80259918_de *sentinel = head;
        do {
            Node_func_80259918_de *next = node->next;
            if (node->key == arg1) {
                Node_func_80259918_de *tail;
                node->prev->next = next;
                node->next->prev = node->prev;
                tail = owner->otherPrev;
                node->next = other;
                node->prev = tail;
                owner->otherPrev->next = node;
                owner->otherPrev = node;
            }
            node = next;
        } while (node != sentinel);
    }
}
#else
#include "structs/sound.h"
/* The port: the same on the typed queue (the view above has N64 offsets; the node's key (+0xB0)
 * is body.soundId). Matching active requests move to the free list's tail. */
void SndQueue_CancelSoundId(SndQueue *q, s32 arg1) {
    SndLink *node = q->active.link.next;
    while (node != &q->active.link) {
        SndLink *next = node->next;
        if (((SndNode *)node)->body.soundId == arg1) {
            node->prev->next = next;
            node->next->prev = node->prev;
            node->next = &q->free.link;
            node->prev = q->free.link.prev;
            q->free.link.prev->next = node;
            q->free.link.prev = node;
        }
        node = next;
    }
}
#endif
#ifndef TARGET_PORT
void SndQueue_CancelOwner(OwnerRecAtD8 *arg0, s32 arg1) {
    ListNode *node;
    ListNode *next;
    ListNode *sentinel;
    ListNode *loop_sentinel;
    ListNode *append_sentinel;
    ListNode *tail;
    node = (ListNode *)arg0->activeNext;
    sentinel = (ListNode *)&arg0->activePrev;
    append_sentinel = (ListNode *)&arg0->otherPrev;
    if (node != sentinel) {
        loop_sentinel = sentinel;
        do {
            next = node->next;
            if (node->valueBC == arg1) {
                node->prev->next = next;
                node->next->prev = node->prev;
                tail = (ListNode *)arg0->otherPrev;
                node->next = append_sentinel;
                node->prev = tail;
                ((ListNode *)arg0->otherPrev)->next = node;
                arg0->otherPrev = (Node_func_80259918_de *)node;
            }
            node = next;
        } while (node != loop_sentinel);
    }
}
#else
#include "structs/sound.h"
/* The port: the same on the typed queue (the view above has N64 offsets; the node's valueBC (+0xBC)
 * is body.unkB4). Matching active requests move to the free list's tail. */
void SndQueue_CancelOwner(SndQueue *q, s32 arg1) {
    SndLink *node = q->active.link.next;
    while (node != &q->active.link) {
        SndLink *next = node->next;
        if (((SndNode *)node)->body.unkB4 == arg1) {
            node->prev->next = next;
            node->next->prev = node->prev;
            node->next = &q->free.link;
            node->prev = q->free.link.prev;
            q->free.link.prev->next = node;
            q->free.link.prev = node;
        }
        node = next;
    }
}
#endif
#ifndef TARGET_PORT
void SndQueue_CancelFlags(Lists *arg0, s32 arg1) {
    NodeRecAtAC *var_a2;
    NodeRecAtAC *temp_a3;
    NodeRecAtAC *temp_v0;
    NodeRecAtAC *temp_t0;
    NodeRecAtAC *temp_t1;
    var_a2 = arg0->first.next;
    temp_v0 = &arg0->first;
    if (var_a2 != temp_v0) {
        temp_t1 = &arg0->second;
        temp_t0 = temp_v0;
        do {
            temp_a3 = var_a2->next;
            if ((var_a2->flags & arg1) != 0) {
                NodeRecAtAC *temp_tail;
                var_a2->prev->next = temp_a3;
                var_a2->next->prev = var_a2->prev;
                temp_tail = arg0->second.prev;
                var_a2->next = temp_t1;
                var_a2->prev = temp_tail;
                arg0->second.prev->next = var_a2;
                arg0->second.prev = var_a2;
            }
            var_a2 = temp_a3;
        } while (var_a2 != temp_t0);
    }
}
#else
#include "structs/sound.h"
/* The port: the same on the typed queue (the view above has N64 offsets; the node's flags (+0xAC)
 * is body.flags). Matching active requests move to the free list's tail. */
void SndQueue_CancelFlags(SndQueue *q, s32 arg1) {
    SndLink *node = q->active.link.next;
    while (node != &q->active.link) {
        SndLink *next = node->next;
        if ((((SndNode *)node)->body.flags & arg1) != 0) {
            node->prev->next = next;
            node->next->prev = node->prev;
            node->next = &q->free.link;
            node->prev = q->free.link.prev;
            q->free.link.prev->next = node;
            q->free.link.prev = node;
        }
        node = next;
    }
}
#endif

/* A node of a doubly linked list: next at 0, prev at 4. */
typedef struct DLNode {
    /* 0x0 */ struct DLNode *next;
    /* 0x4 */ void *prev;
} DLNode;

/* Insert at the head of the list *arg0. */
void SndQueue_LinkHead(DLNode **arg0, DLNode *arg1) {
    arg1->next = *arg0;
    arg1->prev = arg0;
    (*arg0)->prev = arg1;
    *arg0 = arg1;
}
