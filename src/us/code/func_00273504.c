/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025B1E8.c, func_0025B1E8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed int s32;
typedef float f32;
typedef void *OSMesg;
typedef s32 OSPri;
typedef s32 OSId;
typedef union {
    struct { f32 f_odd; f32 f_even; } f;
} __OSfp;
typedef struct {
               u64 at, v0, v1, a0, a1, a2, a3;
               u64 t0, t1, t2, t3, t4, t5, t6, t7;
               u64 s0, s1, s2, s3, s4, s5, s6, s7;
               u64 t8, t9, gp, sp, s8, ra;
                u64 lo, hi;
                u32 sr, pc, cause, badvaddr, rcp;
                u32 fpcsr;
    __OSfp fp0, fp2, fp4, fp6, fp8, fp10, fp12, fp14;
    __OSfp fp16, fp18, fp20, fp22, fp24, fp26, fp28, fp30;
} __OSThreadContext;
typedef struct {
    u32 flag;
    u32 count;
    u64 time;
} __OSThreadprofile_s;
typedef struct OSThread_s {
               struct OSThread_s *next;
               OSPri priority;
               struct OSThread_s **queue;
               struct OSThread_s *tlnext;
               u16 state;
               u16 flags;
               OSId id;
               int fp;
               __OSThreadprofile_s *thprof;
               __OSThreadContext context;
} OSThread;
typedef struct OSMesgQueue_s {
               OSThread *mtqueue;
               OSThread *fullqueue;
               s32 validCount;
               s32 first;
               s32 msgCount;
               OSMesg *msg;
} OSMesgQueue;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef union { s32 i; f32 f; } IF32;
typedef struct SndEnv SndEnv;
typedef struct SndSeq SndSeq;
typedef struct SndFx SndFx;
typedef struct SndInfo SndInfo;
typedef struct SndLink SndLink;
typedef struct SndMgr SndMgr;
typedef struct SndVoice SndVoice;
typedef struct SndNode SndNode;
typedef struct SndQueue SndQueue;
typedef struct SndVoices SndVoices;
typedef struct SndLock SndLock;
struct SndLink {
                 SndLink* prev;
                 SndLink* next;
};
struct SndEnv {
                 u16 w0;
                 u16 a;
                 u16 b;
                 s16 c;
                 u16 w8;
                 u16 wA;
};
struct SndVoice {
                 s32 idx;
                 s32 unk4;
                 s32 unk8;
                 s32 unkC;
                 s32 unk10;
                 s32 unk14;
                 s32 unk18;
                 s32 unk1C;
                 s16 unk20;
                 u8 pad22[0x2];
                 s32 unk24;
                 s16 pan;
                 u8 pad2A[0x2];
                 f32 unk2C;
                 u8 pad30[0x4];
                 f32 unk34;
                 s16 unk38;
                 s16 unk3A;
                 s16 unk3C;
                 u8 pad3E[0x2];
                 s32 unk40;
                 Vec3f pos;
                 s32 unk50;
                 u8 unk54;
                 u8 pad55[0x3];
                 s32 unk58;
                 s32 unk5C;
                 u8 pad60[0x28];
                 s32 unk88;
                 SndEnv env;
                 f32 unk98;
                 f32 unk9C;
                 s32 unkA0;
                 s32 unkA4;
                 s32 tag;
                 s32 unkAC;
                 SndMgr* mgr;
                 SndMgr* unkB4;
                 f32 unkB8;
                 s32 unkBC;
                 s32 unkC0;
                 s32 unkC4;
                 u8 padC8[0x4];
};
struct SndNode {
                 SndLink link;
                 SndVoice body;
};
struct SndQueue {
                 SndMgr* owner;
                 SndNode active;
                 SndNode free;
                 SndNode nodes[32];
};
struct SndVoices {
                 SndMgr* owner;
                 SndVoice v[17];
};
struct SndLock {
                 OSMesgQueue q;
                 u8 pad18[0x4];
                 s32 count;
};
struct SndSeq {
                 SndMgr* owner;
                 s32 unk4;
                 s32* unk8;
                 s32 unkC;
                 void* unk10;
                 void* csp;
                 s32 unk18;
                 s32 unk1C;
                 s32 unk20;
                 s32 unk24;
                 s32 unk28;
                 f32 vol;
                 f32 unk30;
                 f32 unk34;
                 s32 unk38;
                 f32 unk3C;
                 IF32 unk40;
};
struct SndFx {
                 SndMgr* owner;
                 s32 unk4;
                 s32 unk8;
                 s32 unkC;
                 u8 pad10[0x4];
                 s32 unk14;
                 s32 unk18;
                 s32 sndId;
                 u8 pad20[0x20];
};
struct SndMgr {
                 u8 pad0[0x7C];
                 s32* unk7C;
                 u8 pad80[0x4];
                 u8 player[84];
                 u8 padD8[0x4];
                 s16 snd[16];
                 u8 padFC[0x6];
                 s16 unk102;
                 s32 unk104;
                 s32 unk108;
                 s32 unk10C;
                 SndLock lock;
                 s32 unk130;
                 s32 unk134;
                 SndQueue q;
                 SndSeq seq;
                 u8 heap[16];
                 SndVoices pool;
                 void* root2B48;
                 void* tree2B4C;
                 void* node2B50;
                 s32 unk2B54;
                 void* unk2B58;
                 s32* unk2B5C;
                 void* unk2B60;
                 s32 unk2B64;
                 u8 pad2B68[0x8];
                 u8* tab70;
                 u8* tab74;
                 u8* tab78;
                 s32 unk2B7C;
                 s32 unk2B80;
                 s32 unk2B84;
                 void* unk2B88;
                 s16 unk2B8C;
                 u8 pad2B8E[0x2];
                 s32 unk2B90;
                 u8 unk2B94;
                 u8 pad2B95[0x3];
                 s32 unk2B98;
                 s32 unk2B9C;
                 f32 unk2BA0;
                 f32 unk2BA4;
                 f32 unk2BA8;
                 s32 unk2BAC;
                 s32 unk2BB0;
                 s32 unk2BB4;
                 s32 unk2BB8;
                 u8 pad2BBC[0x4];
                 SndFx obj2BC0;
};
struct SndInfo {
                 s16 w0;
                 s16 w2;
                 s8 b4;
                 s8 b5;
                 u16 bits6;
                 s16 w8;
                 s16 wA;
                 s16 wC;
                 s8 bE;
                 s8 bF;
                 s16 w10;
                 s16 w12;
                 s16 w14;
};
typedef struct M04Vec3 {
    f32 x, y, z;
} M04Vec3;
typedef union M04IF {
    s32 i;
    f32 f;
} M04IF;
typedef struct M04SortTab {
               u8 pad0[0xE];
               s16 n;
               u32 v[1];
} M04SortTab;
typedef struct M04Rec12 {
    u16 w0, w1, w2;
    s16 w3;
    u16 w4, w5;
} M04Rec12;
typedef struct M04Tgt {
                u8 pad0[0x128];
                f32 x;
                f32 y;
                f32 z;
                u8 pad134[0x528 - 0x134];
                f32 f528;
} M04Tgt;
typedef struct M04Ctx {
                 u8 pad0[0x84];
                 u8 player[0x58];
                 s16 snd[16];
                 u8 padFC[8];
                 s32 unk104;
                 u8 pad108[0x2B88 - 0x108];
                 u8 *tab2B88;
                 u8 pad2B8C[8];
                 s8 defaultPan;
                 u8 pad2B95[3];
                 M04Tgt *tgt;
                 u8 pad2B9C[4];
                 f32 volA;
                 f32 volB;
                 f32 volC;
                 u8 pad2BAC[0xC];
                 s32 unk2BB8;
} M04Ctx;
typedef struct M04VP {
               s32 w0;
               s32 w4;
               s32 vol;
               s32 wC;
               s16 s10;
               u8 pad12[2];
               M04IF v14;
               s16 pan;
               u8 pad1A[2];
               f32 scale;
               f32 f20;
               f32 f24;
               s16 s28;
               s16 s2A;
               s16 s2C;
               u8 pad2E[6];
               M04Vec3 pos;
               s32 f40;
               s8 mode;
               u8 pad45[3];
               f32 f48;
               f32 f4C;
} M04VP;
typedef struct M04Emitter {
               s32 w0;
               s32 w4;
               s32 w8;
               s32 id;
               M04VP vp;
               M04Rec12 env;
               f32 envA;
               f32 envSlope0;
               M04Rec12 env2;
               f32 env2A;
               f32 env2Slope;
               s32 hasEnv;
               M04Rec12 rec;
               f32 env0;
               f32 envSlope;
               s32 unkA0;
               u32 flags;
               s32 kind;
               s32 unkAC;
               M04Ctx *ctx;
               s32 unkB4;
               f32 unkB8;
               s32 unkBC;
               s32 resetFlag;
               s32 unkC4;
               f32 unkC8;
} M04Emitter;
extern f32 D_800A8160;
extern f32 D_800A8164;
extern f32 D_800A8168;
extern s32 D_800B2A34[];
extern s32 D_800B2A44[];
s16 func_00293470(void *p, s32 a, void *owner);
void func_00293E30(void *p, s16 id, s32 pri);
void func_002714B0(SndMgr *m);
void func_00271514(SndMgr *m);
s16 func_00273370(SndVoices *p, s16 a, s16 b, s16 c, s32 d);
s16 func_00276820(SndInfo *i);
s8 func_00276838(SndInfo *i);
s8 func_00276844(SndInfo *i);
s8 func_00276868(SndInfo *i);
s16 func_00276898(SndInfo *i);
s32 func_00211420();
typedef struct M04Bank2 {
    u8 pad0[0xC];
    M04SortTab *tab;
} M04Bank2;
M04Emitter *func_00273504(SndVoices *p, SndInfo *info, int tag, s32 a3)
{
  s16 *snd;
  SndMgr *m3;
  SndMgr *m;
  s32 i;
  s16 idx;
  u8 *core;
  s32 pr;
  M04VP *vp;
  s32 pri;
  unsigned int sv;
  u16 flag;
  f32 f;
  u8 *new_var2;
  SndMgr *m2;
  s16 *new_var;
  s32 r;
  M04Emitter *v;
  pri = func_00276820(info);
  flag = func_00276838(info);
  m = p->owner;
  snd = m->snd;
  if (((M04Bank2 *) m->unk7C)->tab->n < pri)
  {
    return 0;
  }
  func_002714B0(m);
  new_var = &(*((s16 *) (((u8 *) info) + 2)));
  if ((func_00276844(info) == 0x64) || ((func_00211420() % 100) < func_00276844(info)))
  {
    idx = func_00273370(p, (s16) tag, pri, (s16) flag, func_00276898(info));
    if (idx != (-1))
    {
      v = (M04Emitter *) (&p->v[idx]);
      if (idx == 0x10)
      {
        v->w4 = a3;
        v->id = p->owner->unk108++;
        v->w8 = 0x63;
        goto blk16;
      }
      p->owner->unk102 = idx;
      m2 = p->owner;
      new_var2 = m2->player;
      core = ((u8 *) m2) + 0x7C;
      snd[idx] = func_00293470(new_var2, ((M04Bank2 *) m2->unk7C)->tab->v[pri], m2);
      sv = snd[idx];
      if (sv == (-1))
      {
        v->id = sv;
        v->w8 = sv;
        v->vp.s28 = 0;
        p->owner->unk102 = sv;
        fail:
        func_00271514(p->owner);
        return 0;
      }
      f = ((f32) ((s16) flag)) * D_800A8160;
      (*((u16 *) (core + 0x80)))++;
      v->w4 = a3;
      m3 = p->owner;
      v->id = m3->unk108++;
      v->w8 = snd[idx];
      v->unkA0 = *new_var;
      if (!(D_800A8164 <= f))
      {
        pr = (s32) f;
      }
      else
      {
        pr = (s32) (f - D_800A8164);
        pr = pr | 0x80000000;
      }
      func_00293E30(new_var2, snd[idx], pr & 0xFF);
      blk16:
      v->kind = p->owner->unk2B8C;
      v->resetFlag = p->owner->unk2BAC;
      v->unkBC = 0;
      for (i = 0; i < 4; i++) { if (v->kind == D_800B2A34[i]) { v->unkBC = 1; break; } }
      v->unkC4 = 1;
      for (i = 0; i < 6; i++) { if (v->kind == D_800B2A44[i]) { v->unkC4 = 0; break; } }
      v->unkB4 = p->owner->unk2B90;
      v->unkAC = 0;
      vp = &v->vp;
      v->unkB8 = *((f32 *) (((u8 *) p->owner) + 0x2BBC));
      vp->s28 = flag;
      vp->vol = (vp->wC = 0x7D00);
      vp->v14.i = 0;
      vp->s10 = *((s8 *) (((u8 *) info) + 0xE));
      vp->pan = 0x40;
      vp->s2A = pri;
      vp->s2C = tag;
      vp->w0 = p->owner->unk104;
      vp->f24 = D_800A8168;
      vp->mode = func_00276868(info);
      v->flags = func_00276898(info);
      vp->f40 = 0;
      func_00271514(p->owner);
      return v;
    }
  }
  goto fail;
}
