#include "header.h"

int find_LRU(unsigned int set, char which_cache) {
	switch (which_cache) {
	case ('d'):
	case ('D'): 	// We're in the L1_data cache if which_chache is 'D' or 'd'
		for (int i = 0; i < 8; ++i) {
			if (L1_data[i][set].LRU_bits == 0) {			// find the set with the LRU bits of 0 (Least Recently Used)
				return i;									// return the way/index in which the LRU is
			}
		}
		break;

	case ('i'):
	case ('I'): 	// We're in the L1_instruction cache if which_chache is 'I' or 'i'

		for (int i = 0; i < 4; ++i) {
			if (L1_inst[i][set].LRU_bits == 0) {			// find the set with the LRU bits of 0 (Least Recently Used)
				return i;									// return the way/index in which the LRU is
			}
		}
		break;
	}
	return -1; // This means that something is wrong! We expected an LRU (0) but we couldn't find any
}
