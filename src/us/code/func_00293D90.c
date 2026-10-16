/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/sndpsetpan.c, alSndpSetPan.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
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
struct ParamsRecAt8V2;
typedef struct ParamsRecAt8V2 ParamsRecAt8V2;
struct ParamsRecAt8V2;
struct ParamsRecAt8V2 {
    s16 f0;
    s32 f4;
    s8 f8;
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
extern s32 func_00291034(void *, s16 *, s32);
s32 func_00293D90(struct ObjRecAt40V2 *arg0, s8 arg1) {
    ParamsRecAt8V2 p;
    s32 acc;
    acc = arg0->field40;
    p.f0 = 2;
    p.f8 = arg1;
    acc += arg0->field3C * 0x30;
    p.f4 = acc;
    func_00291034(&arg0->events, (s16 *)&p, 0);
}
