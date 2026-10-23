/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/TEXTURE/80254CE4.CPP, func_80254ED0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

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
extern s32 func_00203068(s32 unused0, s32 unused1, s32 keep_recent);
struct func_8022BC04_S3;
typedef struct func_8022BC04_S3 func_8022BC04_S3;
struct func_8022BC04_S3;
struct func_8022BC04_S3 {
    char pad0[0x10];
    s32 unk10;
};
extern char D_800D8D7C;
void CList__AddHead(void *list, void *node);
extern IntrusiveList D_800D8D68;
extern void CList__Remove(void *list, void *node);
static __inline__ func_8022BC04_S3 *take(s32 active) {
 func_8022BC04_S3 *node=D_800D8D68.head;
 if(node) {CList__Remove(&D_800D8D68,node); node->unk10=active;CList__AddHead(&D_800D8D7C,node);}
 return node;
}
void *func_002066F8(s32 unused,s32 flags) {
 func_8022BC04_S3 *node=take(1);
 if (!node) {
  s32 active=1;
  flags &= 0x10;
  while(func_00203068(0,2,flags==0)) {node=take(active);if(node)break;}
 }
 return node;
}
