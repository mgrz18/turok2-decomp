/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80405DC0_16.CPP, func_804098CC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
#ifdef TARGET_PORT
typedef __INTPTR_TYPE__ sptr;
typedef __UINTPTR_TYPE__ uptr;
#else
typedef s32 sptr;
typedef u32 uptr;
#endif
extern void Pak_AllocBuffer(void ***handle, void **data, s32 units);
extern void * memset(void *arg0, int arg1, uptr arg2);
extern char *D_800F70A8;
extern char D_800C13F0[];
extern void CCache__Flush(s32 a);
extern void CCache__Deallocate(void *arg0, void *arg1);
extern void **CCache__Allocate(s32, s32, s32, char *);
void CCache__Deallocate(void *, void *);
void CCache__Flush(s32);
void Pak_AllocBuffer(void ***handle, void **data, s32 units) {
    void **block;
    void *first;
    if (*handle != 0) {
        CCache__Flush(0);
        CCache__Deallocate(0, *handle);
    }
    if (units != 0) {
        units <<= 8;
    } else {
        units = (s32)(D_800F70A8 + 0x688);
    }
    block = CCache__Allocate(0, units, 0x33, D_800C13F0);
    *handle = block;
    first = *block;
    *data = first;
    memset(first, 0, units);
}
