#include "header.h"

int main(int argc, char** argv) {

    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <mode> <file>\n";
        return 1;
    }

    // Convert the mode argument from string to int
    mode = atoi(argv[1]);

    // Check if the mode is valid
    if (mode < 0 || mode > 1) {
        cerr << "Invalid mode value. Please enter 0 or 1.\n";
        return 1;
    }

    // Initialize the cache at the beginning
    clear_cache();

    cout << "\t --- L1 Cache Simulation ---\n" << endl;
    cout << "  Mode 0: Display only the required summary of usage statistics and responses to 9s in the trace file." << endl;
    cout << "  Mode 1: Display everything from mode 0, as well as the communication messages to the L2 cache." << endl;

    cout << "\nSelected Mode: " << mode << "\n\n"; // Display the selected mode

    // Call the parser function, and give it the mode and the file name
    parser(mode, argv[2]);

    cout << "\n\t --- Simulation done! --- " << endl;

    return 0;
}
