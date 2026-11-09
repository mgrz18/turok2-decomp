
#ifdef TARGET_PORT
#include "common.h"
#include "structs/swoosh.h"

/* The port: the list words at their host size (structs/swoosh.h: an element links to its
 * previous element in its first word, to its next in its second). */
typedef struct T2ListNode {
    struct T2ListNode *prev;
    struct T2ListNode *next;
} T2ListNode;

/* Unlinks arg1 from the used list, appends it to the free list. */
void NodePool_Free(void *arg0, void *arg1) {
    SwooshPool *p = arg0;
    T2ListNode *n = arg1;

    if (n->prev != NULL) n->prev->next = n->next;
    if (n->next != NULL) n->next->prev = n->prev;
    if (p->used.head == n) p->used.head = n->next;
    if (p->used.tail == n) p->used.tail = n->prev;
    p->used.count--;
    if (p->free.count == 0) {
        p->free.head = n;
        p->free.tail = n;
        n->prev = NULL;
        n->next = NULL;
    } else {
        n->prev = p->free.tail;
        n->next = NULL;
        ((T2ListNode *)p->free.tail)->next = n;
        p->free.tail = n;
    }
    p->free.count++;
}
#else
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
void NodePool_Free(void *arg0, void *arg1)
{
  void **temp_v1;
  void *temp_a2;
  s8 *new_var;
  void *temp_v1_2;
  void *new_var2;
  temp_v1 = *((void ***) (((s8 *) arg1) + 4));
  temp_a2 = arg0 + 0xC;
  if (temp_v1 != 0)
  {
    *temp_v1 = *((void **) (((s8 *) arg1) + 0));
  }
  temp_v1_2 = *((void **) (((s8 *) arg1) + 0));
  if (temp_v1_2 != 0)
  {
    *((void ***) (((s8 *) temp_v1_2) + 4)) = (void **) (*((void ***) (((s8 *) arg1) + 4)));
  }
  if ((*((void ***) (((s8 *) arg0) + 0xC))) == arg1)
  {
    *((void ***) (((s8 *) arg0) + 0xC)) = (void **) (*((void ***) (((s8 *) arg1) + 4)));
  }
  if ((*((void **) (((s8 *) temp_a2) + 4))) == arg1)
  {
    *((void **) (((s8 *) temp_a2) + 4)) = (void *) (*((void **) (((s8 *) arg1) + 0)));
  }
  *((s32 *) (((s8 *) temp_a2) + 8)) = (s32) ((*((s32 *) (((s8 *) temp_a2) + 8))) - 1);
  if ((*((s32 *) (((s8 *) arg0) + 8))) == 0)
  {
    *((void **) (((s8 *) arg0) + 0)) = arg1;
    *((void **) (((s8 *) arg0) + 4)) = arg1;
    *((void **) (((s8 *) arg1) + 0)) = 0;
    *((void ***) (((s8 *) arg1) + 4)) = 0;
  }
  else
  {
    new_var2 = *((void **) (((s8 *) arg0) + 4));
    *((void ***) (((s8 *) arg1) + 4)) = 0;
    *((void **) (((s8 *) arg1) + 0)) = (void *) new_var2;
    new_var = ((s8 *) arg0) + 4;
    *((void **) (((s8 *) (*((void **) new_var))) + 4)) = arg1;
    *((void **) new_var) = arg1;
  }
  *((s32 *) (((s8 *) arg0) + 8)) = (s32) ((*((s32 *) (((s8 *) arg0) + 8))) + 1);
}
#endif
