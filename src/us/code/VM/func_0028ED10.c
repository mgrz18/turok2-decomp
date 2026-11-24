/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SCHED/CODE_8023CBF8.CPP, Sched_FinishPageJob.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. Over
 * globals/pager.h (round 29, part r2: Rage Wars' PagerState is Pager, ResourcePageMapping
 * PageMapping, ResourcePageJob PageJob, ResourcePageThread FaultedThread). */

#include "common.h"
#include "ultra_types.h"
#include "structs/pager.h"
#include "globals/pager.h"
#include "protos/ultra.h"
#include "protos/vm.h"

static __inline__ void writeLink(PageMapping **where, PageMapping *value) { *where = value; }

/* A page read in: maps it and resumes the faulting thread (or, when the page was mapped meanwhile,
 * returns the mapping to the head of the chain), then frees the job. */
void VM_FinishPageJob(PageJob *req) {
    PageMapping *link;
    if (*req->slot == 0xFF) {
        link = req->mapping;
        writeLink(&link->next, 0);
        writeLink(&link->prev, gPager.chain.tail);
        if (gPager.chain.tail != 0) {
            gPager.chain.tail->next = link;
        }
        gPager_chain_tail = link;
        if (gPager_chain_head == 0) {
            gPager_chain_head = link;
        }
        VM_MapTLBEntry(req->page, req->mapping, ((FaultedThread *)req->thread)->slot, req->slot);
        osInvalDCache((void *)(req->page << 12), 0x1000);
        osInvalICache((void *)(req->page << 12), 0x1000);
    } else {
        link = req->mapping;
        writeLink(&link->prev, 0);
        writeLink(&link->next, gPager.chain.head);
        if (gPager.chain.head != 0) {
            gPager.chain.head->prev = link;
        }
        gPager_chain_head = link;
        if (gPager_chain_tail == 0) {
            gPager_chain_tail = link;
        }
    }
    ((FaultedThread *)req->thread)->state = 2;
    osStartThread(req->thread);
    osSendMesg(&gPager_jobs, req, 1);
}
