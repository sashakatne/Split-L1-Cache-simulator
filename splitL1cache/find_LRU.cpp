#include "header.h"

// Find the way in `set` whose LRU_bits == 0 (the Least Recently Used way).
// Returns -1 only if the LRU invariant is broken (every way decremented
// past 0 without a new MRU being assigned). Callers should treat -1 as
// a programming error and assert.
int find_LRU(unsigned int set, char which_cache) {
	switch (which_cache) {
	case 'D':
		for (int i = 0; i < DC_ASSOCIAVITY; ++i) {
			if (L1_data[i][set].LRU_bits == 0) {
				return i;
			}
		}
		break;

	case 'I':
		for (int i = 0; i < IC_ASSOCIAVITY; ++i) {
			if (L1_inst[i][set].LRU_bits == 0) {
				return i;
			}
		}
		break;
	}
	return -1;	// invariant violation — caller asserts
}
