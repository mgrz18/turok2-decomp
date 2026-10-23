/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SCENE/80286050_8.CPP, func_8028B274_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
struct TableRecAt8;
typedef struct TableRecAt8 TableRecAt8;
struct BankRecAt94;
typedef struct BankRecAt94 BankRecAt94;
struct TableRecAt8;
struct TableRecAt8 {
    s32 unused;
    s32 count;
    u16 entries[1];
};
struct BankRecAt94;
struct TableRecAt8;
/* Turok 2 offsets: 0x14 / 0x40 / table 0x74 (Rage Wars 0x24 / 0x54 / 0x94). */
struct BankRecAt94 {
    char pad0[0x14];
    s32 field14;
    char pad18[0x28];
    s32 field40;
    char pad44[0x30];
    struct TableRecAt8 *table;
};
struct Object_func_8028B274_de;
/* Turok 2: index at 0x2, type id at 0xB8 (Rage Wars 0x4 / 0xE4). */
struct Object_func_8028B274_de {
    char pad0[2];
    s16 index;
    char pad4[0xB4];
    u16 id;
};
struct Object_func_8028B274_de;
typedef struct Object_func_8028B274_de Object_func_8028B274_de;
struct BankRecAt94;
struct Object_func_8028B274_de;
extern void func_002241D8(struct BankRecAt94 *, struct Object_func_8028B274_de *, s32, s32);
extern void func_0021A150(Object_func_8028B274_de *,s32,s32,s32);
static __inline__ s32 find(TableRecAt8 *table,u16 id) {
 s32 index=0,count; u16 *entry;entry=table->entries;count=table->count;
 if(count>0) { do {if(*entry==id) return index; index++;entry++;} while(index<count); }
 return -1;
}
void func_002241D8(BankRecAt94 *arg0,Object_func_8028B274_de *arg1,s32 arg2,s32 arg3) {
 s32 index=0;
 if(arg1->id!=arg2) {
 index=find(arg0->table,arg2);
 if(index!=-1){arg1->index=index;arg1->id=arg2;func_0021A150(arg1,arg0->field40,arg0->field14,arg3);}
 }
}
