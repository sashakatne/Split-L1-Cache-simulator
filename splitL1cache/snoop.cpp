#include "header.h"

using namespace std;

// Operation 4 in the trace = bus-invalidate snoop. Any matching line
// transitions to Invalid regardless of prior MESI state. Bus-read snoops
// (which would transition M/E -> S) are not modeled.
// Harvard L1 — instruction cache is read-only and not snooped on the bus.
void snoop_invalidate(unsigned int addr) {

	unsigned int tag = addr >> (BYTE_OFFSET + CACHE_INDEX);				// Shift the address right by (6+14=20) to get the tag
	unsigned int set = (addr & MASK_CACHE_INDEX) >> BYTE_OFFSET;		// Mask the address with 0x000FFFFF and shift by 6 to get the cache index/set

	for (int i = 0; i < DC_ASSOCIAVITY; ++i) {							// Look for a matching tag in the data cache
		if (L1_data[i][set].tag_bits == tag) {
			L1_data[i][set].MESI_char = 'I';							// Change MESI bit to Invalid
			if (mode == 1) {											// Simulating a snoop return data communication with L2
				cout << "[Snoop-operation] Return data to L2 " << hex << addr << dec << endl;
			}
			break;														// only one way per set should match
		}
	}
}
