#include "header.h"

void parser(int mode, const char* filename) {
    char trace_line[1024];
    char trace_operation[2];
	unsigned int operation;
    unsigned int address;
    FILE* fp;

    fp = fopen(filename, "r");
    if (fp == nullptr) {
        cerr << "\nFailed to open the trace file: " << filename << endl;
        exit(1);
    }

	while (fgets(trace_line, 1024, fp)) {

		sscanf(trace_line, "%c %x", trace_operation, &address);
		if (!strcmp(trace_line, "\n") || !strcmp(trace_line, " ")) {
		}
		else {
			operation = atoi(trace_operation);
			switch (operation) {
			case 0:	read(address);			break;
			case 1:	write(address);			break;
			case 2:	fetch_inst(address);	break;
			case 3:	invalidate(address);	break;
			case 4:	snoop(address);			break;
			case 8:	clear_cache();			break;
			case 9:	print_stats();			break;
			default:
				cout << "\n the value of n (the operation) is not valid \n" << endl;
				break;
			}
		}
	}

    fclose(fp);
}
