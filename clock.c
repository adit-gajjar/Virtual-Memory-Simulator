#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

int clock_position;

/* Page to evict is chosen using the clock algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
int clock_evict() {
	while((coremap[clock_position].pte)->frame & PG_REF){ // page is referenced
		(coremap[clock_position].pte)->frame = (coremap[clock_position].pte)->frame & (~PG_REF);
		clock_position++;
		if (clock_position == memsize){
			clock_position = 0;
		}
	}

	int temp_clock = clock_position;
	clock_position++;
	if (clock_position == memsize){
		clock_position = 0;
	}
	return temp_clock;

}



/* This function is called on each access to a page to update any information
 * needed by the clock algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void clock_ref(pgtbl_entry_t *p) {
	p->frame = p->frame | PG_REF; // set reference bit to 1
	return;
}

/* Initialize any data structures needed for this replacement
 * algorithm.
 */
void clock_init() {
	clock_position = 0;
}
