/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO/80259280_4.CPP, func_80259440_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
struct Glyphs;
struct Glyphs {
    char pad0[0x10];
    s32 bitmaps[1];
};
struct Font;
struct Glyphs;
struct Font {
    char pad0[0xC];
    struct Glyphs *glyphs;
};
struct ContextCode802591C0;
struct Font;
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
struct CellCode802591C0;
typedef struct CellCode802591C0 CellCode802591C0;
struct Entry_func_80259440_de;
struct ListHead;
struct ListHead {
    struct Entry_func_80259440_de *prev;
    struct Entry_func_80259440_de *next;
};
struct ListHead;
typedef struct ListHead ListHead;
struct Cache;
struct ContextCode802591C0;
struct Cache {
    struct ContextCode802591C0 *context;
    ListHead pending;
    char padC[0xD8 - 0xC];
    ListHead done;
};
struct Entry_func_80259440_de;
typedef struct Entry_func_80259440_de Entry_func_80259440_de;
struct Glyphs;
typedef struct Glyphs Glyphs;
struct ContextCode802591C0;
typedef struct ContextCode802591C0 ContextCode802591C0;
extern s16 func_00273370(char *, s32, s32, s32, s32);
struct Font;
typedef struct Font Font;
extern void func_002714B0(s32 *arg0);
extern void func_00271514(s32 *arg0);
extern s16 func_00293470(char *, s32);
extern void *func_0029E460(void *destination, const void *source, int count);
extern void func_002706A0(ContextCode802591C0 *, s32, s32, char *);
static __inline__ void append(ListHead *head, Entry_func_80259440_de *entry) {
    entry->prev = head->prev;
    entry->next = (Entry_func_80259440_de *)head;
    head->prev->next = entry;
    head->prev = entry;
}
void func_00276180(Cache *cache) {
    s16 *slots;
    char *textures;
    Glyphs *glyphs;
    Entry_func_80259440_de *entry;
    Entry_func_80259440_de *next;
    s32 slot;
    s16 *cursor;
    s32 free;
    CellCode802591C0 *cell;
    func_002714B0(cache->context);
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
        slot = func_00273370(cache->context->cells, entry->style, entry->font, entry->glyph, 0);
        if (slot == -1 || slot == 16) {
            break;
        }
        cursor = (s16 *)((slot << 1) + (u32)slots);
        free = *cursor;
        if (free != -1) {
            break;
        }
        *cursor = func_00293470(textures, glyphs->bitmaps[entry->font]);
        if (*cursor == free) {
            break;
        }
        cell = (CellCode802591C0 *)(cache->context->cells + 4 + slot * 0xCC);
        func_0029E460(cell, entry->body, 0xCC);
        cell->sequence = entry->sequence;
        cell->texture = *cursor;
        cell->frame = cache->context->frame;
        cell->index = slot;
        func_002706A0(cache->context, cell->sequence, 0, cell->data);
        entry->sequence = free;
        entry->glyph = free;
        entry->prev->next = entry->next;
        entry->next->prev = entry->prev;
        append(&cache->done, entry);
    }
    func_00271514(cache->context);
}
