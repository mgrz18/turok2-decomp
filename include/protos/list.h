/* CList, the intrusive list (CList__Construct..).
 *
 * Canonical prototypes, generated from the definitions by tools/port/genprotos.py
 * (do not edit; re-run it). Types the header cannot name without a struct header
 * are struct tags or void *. */

#ifndef PROTOS_LIST_H
#define PROTOS_LIST_H

#include "common.h"

struct CList;

void CList__AddHead(struct CList *, void *);
void CList__AddTail(struct CList *, void *);
void CList__Construct(struct CList *, s32, s32);
int CList__Contains(struct CList *, void *);
void *CList__GetRandom(void *);
void CList__InsertAfter(struct CList *, void *, void *);
void CList__InsertBefore(struct CList *, void *, void *);
void CList__MoveToHead(struct CList *, void *);
void CList__MoveToTail(struct CList *, void *);
void CList__Remove(struct CList *, void *);
void CList__RemoveHead(struct CList *);
void CList__RemoveTail(struct CList *);

#endif
