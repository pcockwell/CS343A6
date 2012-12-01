#include <uC++.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include "MPRNG.h"
#include "config.h"

using namespace std;

MPRNG mprng;  // random number generator

/*
 * Convert buffer to an unsigned int
 */
bool convert( unsigned int &val, char *buffer ) {
    int int_val = -1;
    stringstream ss( buffer );
    ss >> dec >> int_val;
    if (int_val < 0) {
        return false;
    }
    val = (unsigned int)int_val;
    return !ss.fail() &&
        string( buffer ).find_first_not_of( " ", ss.tellg() ) == string::npos;
}

/*
 * Prints usages message and exits
 */
void print_usage (char *cmd)
{
    cout <<
        "Usage: " <<
        cmd <<
        " [ config-file [ random-seed (> 0) ] ]" <<
        endl;
    exit(EXIT_FAILURE);
}

void uMain::main()
{
    string config_file("soda.config");
    unsigned int seed = (unsigned int)getpid();
    ifstream *fin;

    switch(argc)
    {
        case 3:
            if (!convert(seed, argv[2]) || seed == 0) print_usage(argv[0]);
        case 2:
            config_file = argv[1];
        case 1:
            try {
                fin = new ifstream(config_file.c_str(), ifstream::in);
            } catch (uFile::Failure) {
                cerr << "Error! could not open input file \"" << config_file << "\"" << endl;
                exit(EXIT_FAILURE);
            }
            break;
        default:
            print_usage(argv[0]);
    }
    delete fin;

    // reading config
    ConfigParms params;
    processConfigFile(config_file.c_str(), params);

}
