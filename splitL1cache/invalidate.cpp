#include "header.h"

void invalidate(unsigned int addr) {

	unsigned int tag = addr >> (BYTE_OFFSET + CACHE_INDEX);				// Shift the address right by (6+14=20) to get the tag
	unsigned int set = (addr & MASK_CACHE_INDEX) >> BYTE_OFFSET;		// Mask the address with 0x000FFFFF and shift by 6 to get the cache index/set

	for (int i = 0; i < DC_ASSOCIAVITY; ++i) {
		if (L1_data[i][set].tag_bits == tag) {
			L1_data[i][set].MESI_char = 'I';	// change the MESI Protocol to the Invalid state
			break;								// only one way per set should match
		}
	}
}
