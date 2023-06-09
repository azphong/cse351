/*
 * CSE 351 Lab 1b (Manipulating Bits in C)
 *
 * Name(s): Aaron Hong 
 * NetID(s): ahong02
 *
 * This is a file for managing a store of various aisles, represented by an
 * array of 64-bit integers. See aisle_manager.c for details on the aisle
 * layout and descriptions of the aisle functions that you may call here.
 *
 * Written by Porter Jones (pbjones@cs.washington.edu)
 */

#include <stddef.h>  // To be able to use NULL
#include "aisle_manager.h"
#include "store_client.h"
#include "store_util.h"

// Number of aisles in the store
#define NUM_AISLES 10

// Number of sections per aisle
#define SECTIONS_PER_AISLE 4

// Number of items in the stockroom (2^6 different id combinations)
#define NUM_ITEMS 64

// Number of items in each section
#define NUM_SPACES 10

// Global array of aisles in this store. Each unsigned long in the array
// represents one aisle.
unsigned long aisles[NUM_AISLES];

// Array used to stock items that can be used for later. The index of the array
// corresponds to the item id and the value at an index indicates how many of
// that particular item are in the stockroom.
int stockroom[NUM_ITEMS];


/* Starting from the first aisle, refill as many sections as possible using
 * items from the stockroom. A section can only be filled with items that match
 * the section's item id. Prioritizes and fills sections with lower addresses
 * first. Sections with lower addresses should be fully filled (if possible)
 * before moving onto the next section.
 */
void refill_from_stockroom() {
  // TODO: implement this function
  for(int a = 0; a < NUM_AISLES; a++){
	for(int s = 0; s < SECTIONS_PER_AISLE; s++){
		int spaces = NUM_SPACES - num_items(&aisles[a], s);
		int* stock = &stockroom[get_id(&aisles[a], s)];
		if(*stock > spaces){
			add_items(&aisles[a], s, NUM_SPACES);
			*stock -= spaces;
		} else {
			add_items(&aisles[a], s, *stock);
			*stock = 0;
		}
	}
  }
}

/* Remove at most num items from sections with the given item id, starting with
 * sections with lower addresses, and return the total number of items removed.
 * Multiple sections can store items of the same item id. If there are not
 * enough items with the given item id in the aisles, first remove all the
 * items from the aisles possible and then use items in the stockroom of the
 * given item id to finish fulfilling an order. If the stockroom runs out of
 * items, you should remove as many items as possible.
 */
int fulfill_order(unsigned short id, int num) {
  // TODO: implement this function
  int to_remove = num;
  for(int a = 0; a < NUM_AISLES; a++){
	for(int s = 0; s < SECTIONS_PER_AISLE; s++){
		if(get_id(&aisles[a], s) == id){
			int section_items = num_items(&aisles[a], s);
			if(to_remove > section_items){
				remove_items(&aisles[a], s, section_items);
				to_remove -= section_items;
			} else {
				remove_items(&aisles[a], s, to_remove);
				to_remove = 0;
			}
		}
	}
  }
  if(to_remove > 0){
	if(to_remove > stockroom[id]){
		to_remove -= stockroom[id];
		stockroom[id] = 0;
	} else {
		stockroom[id] -= to_remove;
		to_remove = 0;
	}
  }
  return num - to_remove;
}

/* Return a pointer to the first section in the aisles with the given item id
 * that has no items in it or NULL if no such section exists. Only consider
 * items stored in sections in the aisles (i.e., ignore anything in the
 * stockroom). Break ties by returning the section with the lowest address.
 */
unsigned short* empty_section_with_id(unsigned short id) {
  // TODO: implement this function
  for(int a = 0; a < NUM_AISLES; a++){
	for(int s = 0; s < SECTIONS_PER_AISLE; s++){
		if((get_id(&aisles[a], s) == id) && (num_items(&aisles[a], s) == 0)){
			return (short*)&aisles[a] + s;
		}
	}
  }
  return NULL;
}

/* Return a pointer to the section with the most items in the store. Only
 * consider items stored in sections in the aisles (i.e., ignore anything in
 * the stockroom). Break ties by returning the section with the lowest address.
 */
unsigned short* section_with_most_items() {
  // TODO: implement this function
  int max = num_items(&aisles[0], 0);
  short* result = (short*)&aisles[0];
  for(int a = 0; a < NUM_AISLES; a++){
	for(int s = 0; s < SECTIONS_PER_AISLE; s++){
		if(num_items(&aisles[a], s) > max){
			max = num_items(&aisles[a], s);
			result = (short*)&aisles[a] + s;
		}
	}
  }
  return result;
}
