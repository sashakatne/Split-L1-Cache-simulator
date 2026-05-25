#include "header.h"

// LRU update for pseudo-LRU per-set replacement.
// Invariant (under correct use): each LRU_bits value in a set is unique
// across used ways; promoting `way` to MRU shifts the others down by one.
// Decrements are guarded so that a broken invariant (e.g., multiple ways
// at LRU=0 during warmup) cannot wrap an unsigned counter to UINT_MAX.
void L1_LRU(unsigned int way, unsigned int set, bool empty_flag, char which_cache) {
	switch (which_cache) {
	case 'D': 	// L1_data cache
		if (empty_flag) {
			// New way being filled; decrement only ways below it that are non-zero.
			for (unsigned int i = 0; i < way; ++i) {
				if (L1_data[i][set].LRU_bits > 0) {
					L1_data[i][set].LRU_bits = L1_data[i][set].LRU_bits - 1;
				}
			}
		}
		else {
			// Promote `way` to MRU; decrement every way whose LRU is >= way's old LRU.
			for (int i = 0; i < DC_ASSOCIAVITY; ++i) {
				if (L1_data[way][set].LRU_bits <= L1_data[i][set].LRU_bits
				    && L1_data[i][set].LRU_bits > 0) {
					L1_data[i][set].LRU_bits = L1_data[i][set].LRU_bits - 1;
				}
			}
		}
		L1_data[way][set].LRU_bits = 0x7;	// MRU for 3-bit (8-way) counter
		break;

	case 'I': 	// L1_instruction cache
		if (empty_flag) {
			for (unsigned int i = 0; i < way; ++i) {
				if (L1_inst[i][set].LRU_bits > 0) {
					L1_inst[i][set].LRU_bits = L1_inst[i][set].LRU_bits - 1;
				}
			}
		}
		else {
			for (int i = 0; i < IC_ASSOCIAVITY; ++i) {
				if (L1_inst[way][set].LRU_bits <= L1_inst[i][set].LRU_bits
				    && L1_inst[i][set].LRU_bits > 0) {
					L1_inst[i][set].LRU_bits = L1_inst[i][set].LRU_bits - 1;
				}
			}
		}
		L1_inst[way][set].LRU_bits = 0x3;	// MRU for 2-bit (4-way) counter
		break;
	}
}
