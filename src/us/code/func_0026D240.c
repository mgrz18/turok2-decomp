/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80279198_de_8.c, func_802794C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed int s32;
struct Shape_typemap_3;
struct Shape_typemap_3 {
    int field_0;
};
struct Field_void_4;
struct Field_void_4 {
    char pad[0x4];
    void * value;
};
struct func_80284AF4_G2;
typedef struct func_80284AF4_G2 func_80284AF4_G2;
struct func_80284AF4_G2;
struct func_80284AF4_G2 {
    void * unk0;
};
struct Field_void_4;
typedef struct Field_void_4 Field_void_4;
struct func_80203E78_S1;
struct func_80203E78_S1 {
    char pad0[0x4];
    s32 unk4;
};
struct func_80203E78_S1;
typedef struct func_80203E78_S1 func_80203E78_S1;
struct ListHeader;
typedef struct ListHeader ListHeader;
struct ListHeader;
struct ListHeader {
    void *head;
    void *tail;
    s32 count;
};
void func_0026D240(void *arg0, void *arg1) {
    if ((((struct ListHeader *) ((s8 *) arg0))->count) == 0) {
        (((struct ListHeader *) ((s8 *) arg0))->head) = arg1;
        (((struct ListHeader *) ((s8 *) arg0))->tail) = arg1;
        (((struct Shape_typemap_3 *) ((s8 *) arg1))->field_0) = 0;
        (((struct func_80203E78_S1 *) ((s8 *) arg1))->unk4) = 0;
    } else {
        (((struct Field_void_4 *) ((s8 *) arg1))->value) = (((struct ListHeader *) ((s8 *) arg0))->head);
        (((struct Shape_typemap_3 *) ((s8 *) arg1))->field_0) = 0;
        (((struct func_80284AF4_G2 *) ((s8 *) ((struct ListHeader *) ((s8 *) arg0))->head))->unk0) = arg1;
        (((struct ListHeader *) ((s8 *) arg0))->head) = arg1;
    }
    (((struct ListHeader *) ((s8 *) arg0))->count) = (((struct ListHeader *) ((s8 *) arg0))->count) + 1;
}
