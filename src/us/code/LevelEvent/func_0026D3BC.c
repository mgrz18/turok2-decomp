
#ifdef TARGET_PORT
#include "common.h"
#include "structs/swoosh.h"

/* The port: the list words at their host size (structs/swoosh.h: an element links to its
 * previous element in its first word, to its next in its second). */
typedef struct T2ListNode {
    struct T2ListNode *prev;
    struct T2ListNode *next;
} T2ListNode;

/* Takes the free list's head into the used list's tail. */
s32 *NodePool_Alloc(void *arg0) {
    SwooshPool *p = arg0;
    T2ListNode *n = p->free.head;

    if (n != NULL) {
        if (n->next != NULL) n->next->prev = n->prev;
        if (n->prev != NULL) n->prev->next = n->next;
        if (p->free.head == n) p->free.head = n->next;
        if (p->free.tail == n) p->free.tail = n->prev;
        p->free.count--;
        if (p->used.count == 0) {
            p->used.head = n;
            p->used.tail = n;
            n->prev = NULL;
            n->next = NULL;
        } else {
            n->prev = p->used.tail;
            n->next = NULL;
            ((T2ListNode *)p->used.tail)->next = n;
            p->used.tail = n;
        }
        p->used.count++;
    }
    return (s32 *)n;
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
s32 *NodePool_Alloc(void *arg0)
{
  s32 *temp_a1;
  s32 *temp_a1_2;
  s32 *temp_v1;
  void *temp_a1_3;
  s32 *new_var;
  temp_v1 = *((s32 **) (((s8 *) arg0) + 0));
  if (temp_v1 != 0)
  {
    temp_a1 = *((s32 **) (((s8 *) temp_v1) + 4));
    if (temp_a1 != 0)
    {
      *temp_a1 = *((s32 **) (((s8 *) temp_v1) + 0));
    }
    temp_a1_2 = *((s32 **) (((s8 *) temp_v1) + 0));
    if (temp_a1_2 != 0)
    {
      *((s32 **) (((s8 *) temp_a1_2) + 4)) = (s32 *) (*((s32 **) (((s8 *) temp_v1) + 4)));
    }
    if ((*((s32 **) (((s8 *) arg0) + 0))) == temp_v1)
    {
      *((s32 **) (((s8 *) arg0) + 0)) = (s32 *) (*((s32 **) (((s8 *) temp_v1) + 4)));
    }
    if ((*((s32 **) (((s8 *) arg0) + 4))) == temp_v1)
    {
      *((s32 **) (((s8 *) arg0) + 4)) = (s32 *) (*((s32 **) (((s8 *) temp_v1) + 0)));
    }
    temp_a1_3 = arg0 + 0xC;
    *((s32 *) (((s8 *) arg0) + 8)) = (s32) ((*((s32 *) (((s8 *) arg0) + 8))) - 1);
    if ((*((s32 *) (((s8 *) temp_a1_3) + 8))) == 0)
    {
      *((s32 **) (((s8 *) arg0) + 0xC)) = temp_v1;
      *((s32 **) (((s8 *) temp_a1_3) + 4)) = temp_v1;
      *((s32 **) (((s8 *) temp_v1) + 0)) = 0;
      *((s32 **) (((s8 *) temp_v1) + 4)) = 0;
    }
    else
    {
      new_var = *((s32 **) (((s8 *) temp_a1_3) + 4));
      *((s32 **) (((s8 *) temp_v1) + 4)) = 0;
      *((s32 **) (((s8 *) temp_v1) + 0)) = new_var;
      *((s32 **) (((s8 *) (*((s32 **) (((s8 *) temp_a1_3) + 4)))) + 4)) = temp_v1;
      *((s32 **) (((s8 *) temp_a1_3) + 4)) = temp_v1;
    }
    *((s32 *) (((s8 *) temp_a1_3) + 8)) = (s32) ((*((s32 *) (((s8 *) temp_a1_3) + 8))) + 1);
  }
  return temp_v1;
}
#endif
