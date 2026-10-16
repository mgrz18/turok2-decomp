/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80256280_de_15.c, func_80257A14_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct OSMesgQueue;
typedef struct OSMesgQueue OSMesgQueue;
struct OSMesgQueue {
    void **mtqueue;
    void **fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    void **msg;
};
struct OSMesgQueue;
extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
struct ListLink;
struct ListLink {
    struct ListLink *next;
    struct ListLink *prev;
};
struct ListLink;
typedef struct ListLink ListLink;
struct OSMesgQueue;
struct MenuRules;
typedef struct MenuRules MenuRules;
struct OSMesgQueue;
struct MenuRules;
struct MenuRules {
    char pad0[0x1C];
    s32 locked;
};
struct Header;
struct Header {
    u8 data[0xCC];
};
struct Header;
typedef struct Header Header;
struct Voice;
typedef struct Voice Voice;
struct Voice;
struct Voice {
    s32 unk0;
    s32 sound;
    s32 unk8;
    s32 unkC;
    char pad10[0x28];
    s16 priority;
    s16 channel;
    char pad3C[0x64];
    s32 pending;
    s32 flags;
    char padA8[0x24];
};
struct Slots;
struct Slots {
    char pad0[0x60];
    s16 ids[16];
};
struct Slots;
typedef struct Slots Slots;
struct ContextCode8025A3EC;
struct ContextCode8025A3EC {
    char pad0[0x7C];
    Slots slots;
    char padFC[0x104 - 0xFC];
    s32 frame;
    char pad108[0x134 - 0x108];
    s32 track;
    char pad138[0x2B98 - 0x138];
    s32 result;
};
struct ContextCode8025A3EC;
struct ContextCode8025A3EC;
typedef struct ContextCode8025A3EC ContextCode8025A3EC;
struct OSMesgQueue;
struct OSMesgQueue;
struct MenuRules;
struct MenuRules;
struct ContextRecAt2BC0;
typedef struct ContextRecAt2BC0 ContextRecAt2BC0;
struct Font_func_80257360_de;
typedef struct Font_func_80257360_de Font_func_80257360_de;
struct Font_func_80257360_de;
struct Font_func_80257360_de {
    char pad0[4];
    struct Font_func_80257360_de *next;
    char pad8[4];
    struct Font_func_80257360_de *child;
};
struct ContextRecAt2BC0;
struct Font_func_80257360_de;
struct ContextRecAt2BC0 {
    char pad0[0x24];
    s32 field24;
    s32 field28;
    char *field2C;
    char pad30[0x7C - 0x30];
    struct Font_func_80257360_de *font;
    struct Font_func_80257360_de *resource;
    char list[0x104 - 0x84];
    s32 frame;
    u8 reserved108[8];
    MenuRules lock;
    u8 pad130[4];
    s32 track;
    char view[0x1D64 - 0x138];
    char panel[0x1D6C - 0x1D64];
    s32 field1D6C;
    char pad1D70[0x1DA8 - 0x1D70];
    char frames[0x10];
    char menu[4];
    Voice voices[17];
    char pad2B48[0x28];
    char *table2B70;
    char *table2B74;
    char *table2B78;
    char pad2B7C[0xC];
    s32 field2B88;
    s16 field2B8C;
    char pad2B8E[2];
    s32 field2B90;
    u8 field2B94;
    char pad2B95[3];
    s32 field2B98;
    s32 field2B9C;
    f32 red;
    f32 green;
    f32 blue;
    s32 field2BAC;
    s32 field2BB0;
    s32 field2BB4;
    s32 field2BB8;
    f32 alpha;
    char dialog[1];
};
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
extern void func_002765F4(struct ChannelSet *arg0, s32 arg1);
struct ChannelSet;
typedef struct ChannelSet ChannelSet;
extern u32 func_0029DFF0(void);
extern void func_0029E010(u32);
extern void osSendMesg(void *, s32, s32);
extern void func_00273DD8(void *, s16);
s32 func_002708C0(void *arg0, s32 id) {
    void *temp_s0;
    void *var_a0;
    s32 temp_v1;
    u32 temp_a0;
    u32 temp_v0;
    s32 found;
    s32 i;
    Voice *voice;
    found = 0;
    if (id == -1) {
        return found;
    }
    temp_s0 = &((ContextRecAt2BC0 *)(arg0))->lock;
    temp_a0 = func_0029DFF0();
    temp_v1 = ((MenuRules *)(temp_s0))->locked + 1;
    ((MenuRules *)(temp_s0))->locked = temp_v1;
    if (temp_v1 != 1) {
        func_0029E010(temp_a0);
        osRecvMesg(temp_s0, 0, 1);
        var_a0 = &((ContextRecAt2BC0 *)(arg0))->view[0];
    } else {
        func_0029E010(temp_a0);
        var_a0 = &((ContextRecAt2BC0 *)(arg0))->view[0];
    }
    func_002765F4(var_a0, id);
    for (i = 0; i < 17; i++) {
        if (((ContextRecAt2BC0 *)arg0)->voices[i].sound == id) {
            found++;
            voice = &((ContextRecAt2BC0 *)arg0)->voices[i];
            if (i == 16) {
                voice->sound = -1;
                voice->priority = 0;
                voice->channel = -1;
                voice->unkC = -1;
                voice->unk8 = -1;
            } else {
                if (voice->pending > 0) {
                    voice->pending = 0;
                    voice->flags &= ~0x10;
                }
                if ((&((ContextCode8025A3EC *)arg0)->slots)[0].ids[i] != -1) {
                    func_00273DD8(&((ContextRecAt2BC0 *)(arg0))->menu[0], i);
                }
            }
        }
    }
    temp_s0 = &((ContextRecAt2BC0 *)(arg0))->lock;
    temp_v0 = func_0029DFF0();
    temp_v1 = ((MenuRules *)(temp_s0))->locked - 1;
    ((MenuRules *)(temp_s0))->locked = temp_v1;
    if (temp_v1 != 0) {
        func_0029E010(temp_v0);
        osSendMesg(temp_s0, 0, 1);
    } else {
        func_0029E010(temp_v0);
    }
    return found;
}
extern u32 func_0029DFF0(void);
extern void func_0029E010(u32);
extern void osSendMesg(void *, s32, s32);
