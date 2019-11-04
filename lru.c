#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

typedef struct node {

	int frame;

	struct node* prev;
	struct node* next;

} list_node;

typedef struct {

	list_node* head;
	list_node* tail;
	int length;

} linked_list;

linked_list lru_list;

/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int lru_evict() {

	int tail_frame = lru_list.tail->frame;

	list_node* to_free = lru_list.tail;
	if (lru_list.head == lru_list.tail){
		lru_list.head = NULL;
		lru_list.tail = NULL;
	} else {
		lru_list.tail = lru_list.tail->prev;
		lru_list.tail->next = NULL;
	}
	free(to_free);

	coremap[tail_frame].node = NULL;
	(lru_list.length)--;

	return tail_frame;
}

/* This function is called on each access to a page to update any information
 * needed by the lru algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void lru_ref(pgtbl_entry_t *p) {
	if (coremap[(p->frame >> PAGE_SHIFT)].node == NULL){ // request was a miss
		// make new lru node
		list_node* new = malloc(sizeof(list_node));
		new->frame = (p->frame >> PAGE_SHIFT);
		new->prev = NULL;
		// insert new node into list
		if (lru_list.length == 0){ // list is empty
			new->next = NULL;
			lru_list.head = new;
			lru_list.tail = new;
		} else {
			new->next = lru_list.head;
			lru_list.head->prev = new;
			lru_list.head = new;
		}

		coremap[(p->frame >> PAGE_SHIFT)].node = new;
		lru_list.length++;
	} else { // request was a hit, so move node to head of list
		list_node* move = coremap[(p->frame >> PAGE_SHIFT)].node;
		// if request was to head, then no action needed
		if (!(move == lru_list.head)){
			if (move == lru_list.tail){
				move->prev->next = NULL;
				lru_list.tail = move->prev;
			} else {
				move->prev->next = move->next;
				move->next->prev = move->prev;
			}
			move->prev = NULL;
			move->next = lru_list.head;

			lru_list.head->prev = move;
			lru_list.head = move;
		}
	}
	return;
}

/* Initialize any data structures needed for this
 * replacement algorithm
 */
void lru_init() {
	lru_list.head = NULL;
	lru_list.tail = NULL;
	lru_list.length = 0;
	for (int i = 0; i < memsize; i++){
		coremap[i].node = NULL;
	}
}
