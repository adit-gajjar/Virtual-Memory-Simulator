#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

int fifo_index;

/* Page to evict is chosen using the fifo algorithm.
 * Returns the page frame number (which is also the fifo_index in the coremap)
 * for the page that is to be evicted.
 */
int fifo_evict() {
	int to_ret = fifo_index;
	fifo_index++;
	if (fifo_index == memsize){
		fifo_index = 0;
	}
	return to_ret;
}

/* This function is called on each access to a page to update any information
 * needed by the fifo algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void fifo_ref(pgtbl_entry_t *p) {

	return;
}

/* Initialize any data structures needed for this
 * replacement algorithm
 */
void fifo_init() {
	fifo_index = 0;
}
