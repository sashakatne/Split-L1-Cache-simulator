#include "header.h"

void L1_LRU(unsigned int way, unsigned int set, bool empty_flag, char which_cache) {
	switch (which_cache) {
	case ('d'):
	case ('D'): 	// We're in the L1_data cache if which_chache is 'D' or 'd'
		// If the way is empty, we use it and decrement every way less than this way.
		if (empty_flag) {
			for (unsigned int i = 0; i < way; ++i) {
				L1_data[i][set].LRU_bits = L1_data[i][set].LRU_bits - 1; //pre-decrement the LRU bits
			}
		}
		// If a way is NOT empty, we compare LRU bits of the current set with the LRU bits of the other sets
		else {
			for (int i = 0; i < 8; ++i) {
				if (L1_data[way][set].LRU_bits > L1_data[i][set].LRU_bits) {
					L1_data[i][set].LRU_bits = L1_data[i][set].LRU_bits;    //no need to do anything, keep the same order
				}
				else
				{
					L1_data[i][set].LRU_bits = L1_data[i][set].LRU_bits - 1; //pre-decrement the LRU bits
				}
			}
		}
		L1_data[way][set].LRU_bits = 0x7;	// Set the current set to MRU 111 (0x7 in Hex)
		break;



	case ('i'):
	case ('I'): 	// We're in the L1_instruction cache if which_chache is 'I' or 'i'
		// If a way is empty, we use it and decrement every way less than this way
		if (empty_flag) {
			for (unsigned int i = 0; i < way; ++i) {
				L1_inst[i][set].LRU_bits = L1_inst[i][set].LRU_bits - 1;
			}
		}
		// If a way is NOT empty, we compare LRU bits of the current set with the LRU bits of the other sets
		else {
			for (int i = 0; i < 4; ++i) {
				if (L1_inst[way][set].LRU_bits > L1_inst[i][set].LRU_bits) {
					L1_inst[i][set].LRU_bits = L1_inst[i][set].LRU_bits;      //no need to do anything, keep the same order
				}
				else
				{
					--L1_inst[i][set].LRU_bits;                  //pre-decrement the LRU bits
				}
			}
		}
		L1_inst[way][set].LRU_bits = 0x3; // Set the current set to MRU 11 (0x3 in Hex)
		break;
	}
}
