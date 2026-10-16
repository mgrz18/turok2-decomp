/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B8370_4.c, func_802B3540_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
struct IntrusiveList;
struct IntrusiveList {
    void *head;
    void *tail;
    s32 prevOffset;
    s32 nextOffset;
    s32 count;
};
typedef struct IntrusiveList IntrusiveList;
union ObjectLinks4_3;
typedef union ObjectLinks4_3 ObjectLinks4_3;
union ObjectLinks4_3;
union ObjectLinks4_3 {
    void * v0;
    char * v1;
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
    char pad0[8];
    s32 allocationMode;
    void *model;
    char reserved10[0x10];
    IntrusiveList recycled;
};
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
struct ObjectState10_5;
typedef struct ObjectState10_5 ObjectState10_5;
typedef void ( *SharedRecV3)(void *, signed int, void *);
struct ObjectState1C;
typedef struct ObjectState1C ObjectState1C;
struct ObjectState1C;
struct ObjectState1C {
    char pad0[0x8];
    ObjectLinks4_3 unk_8;
    char pad8[0x1A - 0x8 - sizeof(ObjectLinks4_3)];
    u16 unk_1A;
};
extern void func_002944A0(void *arg0, void *arg1, s32 arg2);
struct ObjectState10_5;
struct ObjectState10_5 {
    s32 unk_0;
    char pad0[0x4 - 0x0 - sizeof(s32)];
    s32 unk_4;
    char pad4[0x8 - 0x4 - sizeof(s32)];
    s16 unk_8;
    char pad8[0xA - 0x8 - sizeof(s16)];
    u16 unk_A;
    char padA[0xC - 0xA - sizeof(u16)];
    s32 unk_C;
};
extern void *func_00294B58(void);
void func_002944A0(void *arg0, void *arg1, s32 arg2) {
    s32 new_var;
    void *Shared_func_802B3540_de_v0;
    void *temp_a0;
    SharedRecV3 fn;
    if ((((ObjectState1C *)(arg1))->unk_8.v0) != 0) {
        Shared_func_802B3540_de_v0 = func_00294B58();
        if (Shared_func_802B3540_de_v0 != 0) {
            new_var = (((struct Shape_typemap_71 *)(arg0))->field_1C) + (((struct IntegerStateDC_2 *) ((ObjectState1C *) arg1)->unk_8.v1)->state);
            ((ObjectState10_5 *)(Shared_func_802B3540_de_v0))->unk_8 = 0xE;
            ((ObjectState10_5 *)(Shared_func_802B3540_de_v0))->unk_C = arg2;
            ((ObjectState10_5 *)(Shared_func_802B3540_de_v0))->unk_0 = 0;
            ((ObjectState10_5 *)(Shared_func_802B3540_de_v0))->unk_4 = new_var;
            ((ObjectState10_5 *)(Shared_func_802B3540_de_v0))->unk_A = ((ObjectState1C *)(arg1))->unk_1A;
            temp_a0 = ((struct Draw *) ((ObjectState1C *) arg1)->unk_8.v1)->model;
            fn = ((CallbackStateC_4 *)(temp_a0))->callback;
            fn(temp_a0, 3, Shared_func_802B3540_de_v0);
        }
    }
}
