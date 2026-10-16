/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025A844_de_4.c, func_8025B8BC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
struct SlotRecAtB8;
struct SlotRecAtB8 {
    char pad0[0xA8];
    s32 flags;
    char padaC[8];
    char *owner;
    char end[0x14];
};
struct SlotRecAtB8;
typedef struct SlotRecAtB8 SlotRecAtB8;
extern void func_00273BF8(SlotRecAtB8 *base, s16 index);
extern void func_00293CE0(void *);
void func_00273BF8(SlotRecAtB8 *base, s16 index)
{
  unsigned int offset = index * (sizeof(SlotRecAtB8));
  char *entry = (char *) offset;
  SlotRecAtB8 *slots;
  SlotRecAtB8 *selected;
  entry += (unsigned int) base;
  slots = base;
 do { func_00293CE0(0x84 + ((SlotRecAtB8 *) entry)->owner); entry = (char *) slots; entry += offset; selected = (SlotRecAtB8 *) entry; } while (0);
  selected->flags |= 8;
}
