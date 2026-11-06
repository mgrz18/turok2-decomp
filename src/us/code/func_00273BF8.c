/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025A844_de_4.c, func_8025B8BC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void SndVoicePool_PlayVoice(SlotRecAtB8 *base, s16 index);
extern void alSndpPlay(void *);
void SndVoicePool_PlayVoice(SlotRecAtB8 *base, s16 index)
{
  unsigned int offset = index * (sizeof(SlotRecAtB8));
  char *entry = (char *) offset;
  SlotRecAtB8 *slots;
  SlotRecAtB8 *selected;
  entry += (unsigned int) base;
  slots = base;
 do { alSndpPlay(0x84 + ((SlotRecAtB8 *) entry)->owner); entry = (char *) slots; entry += offset; selected = (SlotRecAtB8 *) entry; } while (0);
  selected->flags |= 8;
}
