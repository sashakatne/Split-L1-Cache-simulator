#include "header.h"

using namespace std;

void write(unsigned int addr) {

	unsigned int tag = addr >> (BYTE_OFFSET + CACHE_INDEX);				// Shift the address right by (6+14=20) to get the tag
	unsigned int set = (addr & MASK_CACHE_INDEX) >> BYTE_OFFSET;		// Mask the address with 0x000FFFFF and shift by 6 to get the cache index/set
	bool empty_flag = 0;												// boolean flag for empty way in the cache, if 1, we have an empty way
	int way = -1;														// way in the cache set. Initialized with an invalid way value
	statistics.data_write++;											// Increment the number of write for the data cache

	way = matching_tag(tag, set, 'D');				// Look for a matching tag already in the data cache
	if (way >= 0) {									// if we have a matching tag, then we have an data cache hit! (unless invalid MESI state)
		switch (L1_data[way][set].MESI_char) {
		case 'M':									// in we're in the modified state
			statistics.data_hit++;					// Increment the data hit counter (We have a new hit!)
			L1_data[way][set].MESI_char = 'M';		// stay in the modified state
			L1_data[way][set].tag_bits = tag;
			L1_data[way][set].set_bits = set;
			L1_data[way][set].address = addr;

			L1_LRU(way, set, empty_flag, 'D');		// Update the data cache LRU count
			break;

		case 'E':									// if we're writing in an exclusive state, the data will be modified
			statistics.data_hit++;					// Increment the data hit counter (We have a new hit!)
			L1_data[way][set].MESI_char = 'M';		// go to the modified state
			L1_data[way][set].tag_bits = tag;
			L1_data[way][set].set_bits = set;
			L1_data[way][set].address = addr;

			L1_LRU(way, set, empty_flag, 'D');		// Update the data cache LRU count
			break;

		case 'S':									// write hit on Shared: issue invalidate, move to Modified
			statistics.data_hit++;					// Increment the data hit counter (We have a new hit!)
			L1_data[way][set].MESI_char = 'M';		// go to the modified state
			L1_data[way][set].tag_bits = tag;
			L1_data[way][set].set_bits = set;
			L1_data[way][set].address = addr;

			L1_LRU(way, set, empty_flag, 'D');		// Update the data cache LRU count
			break;

		case 'I':									// write on Invalid (tag match but invalid): write miss, allocate Modified
			statistics.data_miss++;					// Increment the data miss counter
			L1_data[way][set].MESI_char = 'M';		// go to the modified state
			L1_data[way][set].tag_bits = tag;
			L1_data[way][set].set_bits = set;
			L1_data[way][set].address = addr;

			L1_LRU(way, set, empty_flag, 'D');		// Update the data cache LRU count
			break;
		}
	}

	else {											// Data Cache Miss
		statistics.data_miss++;						// Increment the data miss counter 

		if (mode == 1) {							// Simulating a cache Read For Ownership communication from L2
			cout << " [Data-Operation] Read for Ownership from L2 " << hex << addr << dec << endl;
		}

		for (int i = 0; way < 0 && i < 8; ++i) {		// First, check if we have any empty lines
			if (L1_data[i][set].tag_bits == EMPTY_TAG) {
				way = i;								// return the way that has an empty line
				empty_flag = 1;							// if we have an empty line, toggle the empty_flag to high
			}
		}

		if (way >= 0) {									// if we have an empty line, place the read data in it
			L1_data[way][set].MESI_char = 'M';			// go to the modified state
			L1_data[way][set].tag_bits = tag;
			L1_data[way][set].set_bits = set;
			L1_data[way][set].address = addr;

			L1_LRU(way, set, empty_flag, 'D');			// Update the data cache LRU order/count

			if (mode == 1) {							// Simulating an iniial write through communication from L2
				cout << " [Data Write_Through] Write to L2: we have a data Cache Miss " << hex << addr << dec << endl;
			}
		}

		else {											// if we don't have any empty lines, we need to evict the LRU line. 
			if (mode == 1) {							// Simulating an write back communication from L2
				cout << " [Data Write_Back] Write to L2: We have a data Cache Miss " << hex << addr << dec << endl;
			}

			way = -1;
			for (int n = 0; n < DC_ASSOCIAVITY; ++n) {	// find first invalid way to evict
				if (L1_data[n][set].MESI_char == 'I') {
					way = n;
					break;
				}
			}
			if (way < 0) {								//If we don't have any invalid lines,
				way = find_LRU(set, 'D');				// Find the LRU way in the data cache
				assert(way >= 0 && "LRU invariant violated in L1_data");
				L1_data[way][set].MESI_char = 'M';		// go to the modified state
				L1_data[way][set].tag_bits = tag;
				L1_data[way][set].set_bits = set;
				L1_data[way][set].address = addr;

				L1_LRU(way, set, empty_flag, 'D');		// update the L1 data cache LRU bits
			}
			else { 										// if we have an invalid way, we evict it 
				L1_data[way][set].MESI_char = 'M';		// go to the modified state
				L1_data[way][set].tag_bits = tag;
				L1_data[way][set].set_bits = set;
				L1_data[way][set].address = addr;

				L1_LRU(way, set, empty_flag, 'D');	// update the L1 data cache LRU bits
			}
		}
	}
	return;
}
