/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_13.c, func_8023C78C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
struct IntrusiveList;
struct IntrusiveList {
    void *head;
    void *tail;
    s32 prevOffset;
    s32 nextOffset;
    s32 count;
};
typedef struct IntrusiveList IntrusiveList;
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
struct Draw;
typedef struct Draw Draw;
struct OSMesgQueue;
struct Draw;
struct Draw {
    char pad0[8];
    s32 allocationMode;
    void *model;
    char reserved10[0x10];
    IntrusiveList recycled;
};
struct Block10;
typedef struct Block10 Block10;
struct Block10;
struct Block10 {
    s16 f28;
    s16 pad;
    s32 f2C;
    void *f30;
    void **f34;
};
struct OSMesgQueue;
extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
struct OSMesgQueue;
struct OSMesgQueue;
extern s32 D_801206C4;
extern s32 D_800AA210;
void func_0029B030(void *arg0, s32 arg1, s32 arg2);
void **func_00204EDC(s32 arg0, s32 arg1, s32 arg2, void *arg3);
void osSendMesg(void *arg0, void *arg1, s32 arg2);
extern void func_0029B030(void *arg0, s32 arg1, s32 arg2);
extern void osSendMesg(void *arg0, void *arg1, s32 arg2);
s32 func_0028E60C(s32 arg0, s32 arg1_unused, s32 arg2)
{
  char sp10[0x18];
  int new_var2;
  Block10 blk;
  s32 sp38;
  u32 temp_s1;
  s32 sp3C;
  u32 rounded;
  void **temp_v0;
  Draw *temp_v1;
  int new_var;
  temp_s1 = ((u32) (arg0 + 0xFFF)) >> 0xC;
  new_var = (temp_s1 + 1) & (~1);
  if (1)
  {
    func_0029B030(sp10, (s32) (&sp38), 1);
    blk.f28 = 3;
    blk.f2C = arg0;
    blk.f34 = (void **) sp10;
    new_var2 = temp_s1 * 4;
    rounded = new_var + 0x18;
    temp_v0 = func_00204EDC(0, new_var2 + rounded, 3, &D_800AA210);
    temp_v1 = *temp_v0;
    blk.f30 = temp_v1;
    temp_v1->model = temp_v0;
  }
  ((Draw *)blk.f30)->allocationMode = arg2;
  osSendMesg(&D_801206C4, &blk.f28, 1);
  osRecvMesg(sp10, &sp3C, 1);
  return sp3C;
}
