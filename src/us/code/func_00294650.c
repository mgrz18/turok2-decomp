/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/synstopvoice.c, alSynStopVoice.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef struct IntrusiveList IntrusiveList;
struct IntrusiveList {
    void *head;
    void *tail;
    s32 prevOffset;
    s32 nextOffset;
    s32 count;
};
struct IntrusiveList;
union ObjectLinks4_3;
typedef union ObjectLinks4_3 ObjectLinks4_3;
union ObjectLinks4_3;
union ObjectLinks4_3 {
    void * v0;
    char * v1;
};
struct ObjectStateC_2;
struct ObjectStateC_2 {
    char pad0[0x8];
    ObjectLinks4_3 unk_8;
};
struct CallbackStateC_4;
typedef struct CallbackStateC_4 CallbackStateC_4;
struct Draw;
typedef struct Draw Draw;
struct Shape_typemap_71;
struct Shape_typemap_71 {
    unsigned char padding_0[28];
    int field_1C;
};
struct IntegerStateDC_2;
typedef struct IntegerStateDC_2 IntegerStateDC_2;
struct Draw;
struct Draw {
    s32 drawModel;
    s32 updateAnimation;
    s32 allocationMode;
    void **model;
    char reserved10[0x10];
    IntrusiveList recycled;
};
struct ObjectStateC_2;
typedef struct ObjectStateC_2 ObjectStateC_2;
struct CallbackStateC_4;
struct CallbackStateC_4 {
    char pad0[0x8];
    void (*callback)(void *, s32, void *);
};
struct IntegerStateDC_2;
struct IntegerStateDC_2 {
    unsigned char padding[216];
    s32 state;
};
extern void alSynStopVoice(struct Shape_typemap_71 *arg0, void *arg1);
typedef void ( *SharedRecV12)(void *, signed int, void *);
struct ObjectStateC_3;
typedef struct ObjectStateC_3 ObjectStateC_3;
struct ObjectStateC_3;
struct ObjectStateC_3 {
    s32 unk_0;
    char pad0[0x4 - 0x0 - sizeof(s32)];
    s32 unk_4;
    char pad4[0x8 - 0x4 - sizeof(s32)];
    s16 unk_8;
};
extern void *func_00294B58(void);
void alSynStopVoice(struct Shape_typemap_71 *arg0, void *arg1)
{
  int new_var;
  void *Shared_func_802B36F0_de_v0;
  void *temp_a0;
  SharedRecV12 fn;
  if ((((ObjectStateC_2 *)(arg1))->unk_8.v0) != 0)
  {
    Shared_func_802B36F0_de_v0 = func_00294B58();
    if (Shared_func_802B36F0_de_v0 != 0)
    {
      new_var = (arg0->field_1C) + (((struct IntegerStateDC_2 *) ((ObjectStateC_2 *) arg1)->unk_8.v1)->state);
      ((ObjectStateC_3 *)(Shared_func_802B36F0_de_v0))->unk_8 = 0xF;
      ((ObjectStateC_3 *)(Shared_func_802B36F0_de_v0))->unk_0 = 0;
      ((ObjectStateC_3 *)(Shared_func_802B36F0_de_v0))->unk_4 = new_var;
      temp_a0 = ((struct Draw *) ((ObjectStateC_2 *) arg1)->unk_8.v1)->model;
      fn = ((CallbackStateC_4 *)(temp_a0))->callback;
      fn(temp_a0, 3, Shared_func_802B36F0_de_v0);
    }
  }
}
