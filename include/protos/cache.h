/* CCache, the memory / ROM cache.
 *
 * Canonical prototypes, generated from the definitions by tools/port/genprotos.py
 * (do not edit; re-run it). Types the header cannot name without a struct header
 * are struct tags or void *. */

#ifndef PROTOS_CACHE_H
#define PROTOS_CACHE_H

#include "common.h"

struct B12Node;
struct CCacheEntry;
struct CMemEntry;
struct EntryList;
struct EntryRecAt18V3;

void CCache__AddToAddressList(s32, struct CMemEntry *);
void CCache__Advance(void);
void *CCache__AllocCacheEntry(s32, s32);
struct CMemEntry *CCache__AllocMemEntry(s32, s32);
struct B12Node *CCache__Allocate(s32, s32, u32, sptr);
struct B12Node *CCache__AllocateAt(s32, sptr, s32);
struct CMemEntry *CCache__AllocateInternal(s32, s32, s32);
sptr CCache__AllocateMemoryForDecompression(s32, void *, s32);
sptr CCache__AllocateMemoryForDecompressionTop(s32, sptr, s32);
void CCache__BeginDisplayList(s32, s32);
void CCache__ClearCurrentDisplayList(void);
void CCache__ClearDisplayListLocks(s32, s32);
void CCache__Compact(s32, s32);
struct CCacheEntry *CCache__CompleteRequest(s32, struct CCacheEntry *, s32);
void CCache__Construct(s32, s32);
void CCache__DeallocAndReplaceCompressedData(s32, struct CMemEntry **, struct CMemEntry *);
void CCache__Deallocate(sptr, void *);
void CCache__DeallocateInternal(s32, void *);
void CCache__DetachCacheEntry(s32, void *);
void CCache__DrawDebug(s32, s32, s32, s32, s32);
s32 CCache__EndDisplayList(void);
struct EntryRecAt18V3 *CCache__FindOldestMemEntry(s32, struct EntryList *, struct EntryRecAt18V3 **);
void CCache__Flush(void);
void CCache__FreeCacheEntry(s32, void *);
void CCache__FreeMemEntry(s32, struct CMemEntry *);
s32 CCache__FreeOldestMemEntry(s32, s32, s32);
void CCache__FreeOldestMemEntryWithFlags(s32, s32, s32);
s32 CCache__GetFrameDelay(void);
s32 CCache__GetFreeCacheEntryCount(void);
s32 CCache__GetFreeMemEntryCount(void);
s32 CCache__GetMemEntryCount(void);
void *CCache__HeapAlloc(s32, u32, s32, u32);
void **CCache__LoadPersistantIndex(s32, void **, sptr, void *);
#ifdef TARGET_PORT
void *CCache__LoadPersistantSubBlock(s32, sptr, s32, s32, sptr, sptr, sptr); /* callers read the request it leaves in v0 */
#else
void CCache__LoadPersistantSubBlock(s32, sptr, s32, s32, sptr, sptr, sptr);
#endif
void CCache__LockForDisplayList(s32, struct B12Node *);
void *CCache__LookupAndLock(s32, s32);
void CCache__Main(void);
struct CMemEntry *CCache__PopFreeMemEntry(s32, s32);
void CCache__PostDisplayListDone(s32, s32);
void CCache__ReleaseAndReplaceData(s32, void *, struct CMemEntry *);
void CCache__ReleaseDisplayList(s32, sptr);
struct CMemEntry *CCache__Request(s32, uptr, u32, u32, sptr, void *, char *, u32);
struct CMemEntry *CCache__RequestDecompression(s32, s32, struct CMemEntry *, struct CMemEntry *, s32, void *, void *, void *, s32);
void CCache__SetFrameDelay(s32);
void *CCache__TakeFreeCacheEntry(void);
void CCache__Touch(s32, s32);
void CCache__TouchAndLock(s32, struct CMemEntry *);
void CCache__TouchMemEntry(s32, struct CMemEntry *);
void CCache__Unlock(s32, struct CMemEntry *);
void CCache__WaitForDisplayLists(void);

#endif
