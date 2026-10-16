/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/sndpstop.c, alSndpStop.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
struct ALEventQueue;
typedef struct ALEventQueue ALEventQueue;
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
struct Buf16;
typedef struct Buf16 Buf16;
struct Buf16;
struct Buf16 {
    s16 count;
    s16 _pad2;
    s32 value;
    s32 _pad8;
    s32 _padC;
};
struct ObjRecAt40V2;
struct ObjRecAt40V2 {
    u8 padding000[0x14];
    ALEventQueue events;
    u8 padding028[0x14];
    s32 field3C;
    s32 field40;
};
struct ObjRecAt40V2;
typedef struct ObjRecAt40V2 ObjRecAt40V2;
extern void func_00291034(ALEventQueue *, void *, s32);
void func_00293EC0(ObjRecAt40V2 *arg0) {
    s32 new_var;
    Buf16 sp10;
    new_var = arg0->field40;
    sp10.count = 1;
    sp10.value = new_var + (arg0->field3C * 0x30);
    func_00291034(&arg0->events, &sp10, 0);
}
