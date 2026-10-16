/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BB6C0_de_21.c, func_802BBC20_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned long long u64;
struct OSThread_s;
typedef struct OSThread_s OSThread_s;
struct OSThreadContext;
typedef struct OSThreadContext OSThreadContext;
struct OSThreadContext;
struct OSThreadContext {
    u64 at;
    u64 v0;
    u64 v1;
    u64 a0;
    u64 a1;
    u64 a2;
    u64 a3;
    u64 t0;
    u64 t1;
    u64 t2;
    u64 t3;
    u64 t4;
    u64 t5;
    u64 t6;
    u64 t7;
    u64 s0;
    u64 s1;
    u64 s2;
    u64 s3;
    u64 s4;
    u64 s5;
    u64 s6;
    u64 s7;
    u64 t8;
    u64 t9;
    u64 gp;
    u64 sp;
    u64 s8;
    u64 ra;
    u64 lo;
    u64 hi;
    u32 sr;
    u32 pc;
    u32 cause;
    u32 badvaddr;
    u32 rcp;
    u32 fpcsr;
};
struct OSThread_s;
struct OSThread_s {
    struct OSThread_s *next;
    s32 priority;
    struct OSThread_s **queue;
    struct OSThread_s *tlnext;
    u16 state;
    u16 flags;
    s32 id;
    int fp;
    OSThreadContext context;
};
struct OSThread_s;
struct OSThread_s;
extern u32 func_0029DFF0(void);
extern void func_0029E010(u32 mask);
extern void func_0029DF70();
s32 func_0029C6DC(s32 *, s32);
extern s32 D_800B8918;
extern void *D_800B8920;
void func_0029DF70(void)
{
  void *temp_s0;
  s32 *new_var2;
  s8 *new_var;
  int new_var3;
  temp_s0 = func_0029DFF0();
  ((OSThread_s *)(new_var = (s8 *) D_800B8920))->state = (new_var3 = 2);
  new_var2 = &D_800B8918;
  func_0029C6DC(new_var2, new_var3);
  func_0029E010(temp_s0);
}
