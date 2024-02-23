#include "header.h"

void snoop(unsigned int addr) {

	unsigned int tag = addr >> (BYTE_OFFSET + CACHE_INDEX);				// Shift the address right by (6+14=20) to get the tag
	unsigned int set = (addr & MASK_CACHE_INDEX) >> BYTE_OFFSET;		// Mask the address with 0x000FFFFF and shift by 6 to get the cache index/set

	for (int i = 0; i < 8; ++i) {						// Look for a matching tag in the data cache
		if (L1_data[i][set].tag_bits == tag) {
			L1_data[i][set].MESI_char = 'I';			// Change MESI bit to Invalid
			L1_data[i][set].tag_bits = tag;
			L1_data[i][set].set_bits = set;
			L1_data[i][set].address = addr;

			if (mode == 1) {							// Simulating a snoop return data communication with L2
				cout << "[Snoop-operation] Return data to L2 " << hex << addr << endl;
			}
		}
	}
	return;
}
